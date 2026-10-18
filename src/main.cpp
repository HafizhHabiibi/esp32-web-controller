#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <math.h>

#include "controller_html.h"

#ifndef ROBOT_ID
#define ROBOT_ID 1
#endif

namespace Config {
#if ROBOT_ID == 1
constexpr char WIFI_SSID[] = "SOCCER-BOT-01";
constexpr char WIFI_PASSWORD[] = "robot12345";
constexpr uint8_t WIFI_CHANNEL = 1;
constexpr char ROBOT_NAME[] = "ROBOT A";
#else
constexpr char WIFI_SSID[] = "SOCCER-BOT-02";
constexpr char WIFI_PASSWORD[] = "robot12345";
constexpr uint8_t WIFI_CHANNEL = 6;
constexpr char ROBOT_NAME[] = "ROBOT B";
#endif

// L298N: motor kiri (channel A) dan motor kanan (channel B).
constexpr uint8_t PIN_ENA = 14;
constexpr uint8_t PIN_IN1 = 26;
constexpr uint8_t PIN_IN2 = 27;
constexpr uint8_t PIN_ENB = 33;
constexpr uint8_t PIN_IN3 = 18;
constexpr uint8_t PIN_IN4 = 19;

constexpr uint16_t PWM_FREQUENCY = 20000;
constexpr uint8_t PWM_RESOLUTION = 8;
constexpr uint16_t PWM_MAX = 255;
constexpr uint8_t PWM_CHANNEL_LEFT = 0;
constexpr uint8_t PWM_CHANNEL_RIGHT = 1;

constexpr uint16_t COMMAND_TIMEOUT_MS = 300;
constexpr uint16_t TELEMETRY_INTERVAL_MS = 100;
constexpr uint8_t DRIVE_SPEED_PERCENT = 100;
constexpr int8_t LEFT_DIRECTION = 1;
constexpr int8_t RIGHT_DIRECTION = 1;
}  // namespace Config

WebServer httpServer(80);
WebSocketsServer webSocket(81);

struct RobotState {
  bool armed = false;
  int controllerId = -1;
  int leftPwm = 0;
  int rightPwm = 0;
  int lastLoggedX = 1000;
  int lastLoggedY = 1000;
  unsigned long lastCommandAt = 0;
  unsigned long lastTelemetryAt = 0;
} robot;

void setMotor(int value, uint8_t pin1, uint8_t pin2, uint8_t pwmChannel) {
  value = constrain(value, -static_cast<int>(Config::PWM_MAX),
                    static_cast<int>(Config::PWM_MAX));

  // Hilangkan PWM sebelum mengganti polaritas H-bridge.
  ledcWrite(pwmChannel, 0);

  if (value > 0) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    ledcWrite(pwmChannel, value);
  } else if (value < 0) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    ledcWrite(pwmChannel, -value);
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
}

void applyMotorOutput() {
  setMotor(robot.leftPwm * Config::LEFT_DIRECTION,
           Config::PIN_IN1, Config::PIN_IN2, Config::PWM_CHANNEL_LEFT);
  setMotor(robot.rightPwm * Config::RIGHT_DIRECTION,
           Config::PIN_IN3, Config::PIN_IN4, Config::PWM_CHANNEL_RIGHT);
}

void stopMotors() {
  robot.leftPwm = 0;
  robot.rightPwm = 0;
  applyMotorOutput();
}

void driveRobot(int x, int y) {
  x = constrain(x, -100, 100);
  y = constrain(y, -100, 100);

  float left = static_cast<float>(y + x);
  float right = static_cast<float>(y - x);
  const float largest = max(100.0f, max(fabsf(left), fabsf(right)));
  const float pwmScale =
      (Config::DRIVE_SPEED_PERCENT / 100.0f) * Config::PWM_MAX;

  robot.leftPwm = roundf((left / largest) * pwmScale);
  robot.rightPwm = roundf((right / largest) * pwmScale);
  applyMotorOutput();
}

void sendStatus(uint8_t clientId, const char* message) {
  JsonDocument document;
  document["type"] = "status";
  document["robot"] = Config::ROBOT_NAME;
  document["armed"] = robot.armed && robot.controllerId == clientId;
  document["left"] = robot.leftPwm;
  document["right"] = robot.rightPwm;
  document["message"] = message;

  String json;
  serializeJson(document, json);
  webSocket.sendTXT(clientId, json);
}

bool isValidDriveCommand(const JsonDocument& document) {
  if (!document["x"].is<int>() || !document["y"].is<int>()) {
    return false;
  }

  const int x = document["x"];
  const int y = document["y"];
  return x >= -100 && x <= 100 &&
         y >= -100 && y <= 100;
}

void armRobot(uint8_t clientId) {
  // Pulihkan ownership yang mungkin tertinggal setelah reconnect tidak bersih.
  if (!robot.armed && robot.controllerId >= 0) {
    Serial.printf("[DEBUG] Membersihkan owner lama #%d sebelum ARM client #%u\n",
                  robot.controllerId, clientId);
    robot.controllerId = -1;
  }

  if (robot.controllerId >= 0 && robot.controllerId != clientId) {
    Serial.printf("[CMD] Client #%u ARM ditolak: robot sedang digunakan\n",
                  clientId);
    sendStatus(clientId, "BUSY");
    return;
  }

  stopMotors();
  robot.controllerId = clientId;
  robot.armed = true;
  robot.lastCommandAt = millis();
  robot.lastLoggedX = 1000;
  robot.lastLoggedY = 1000;
  Serial.printf("[CMD] Client #%u ARM -> robot aktif\n", clientId);
  sendStatus(clientId, "ARMED");
}

void disarmRobot(uint8_t clientId, const char* message) {
  stopMotors();
  robot.armed = false;
  robot.controllerId = -1;
  Serial.printf("[CMD] Client #%u %s -> motor berhenti\n", clientId, message);
  sendStatus(clientId, message);
}

void handleWebSocketEvent(uint8_t clientId, WStype_t eventType,
                          uint8_t* payload, size_t length) {
  if (eventType == WStype_CONNECTED) {
    const IPAddress remoteIp = webSocket.remoteIP(clientId);
    Serial.printf(
        "[WS] Client #%u CONNECTED dari %s | armed=%d owner=%d\n",
        clientId, remoteIp.toString().c_str(), robot.armed, robot.controllerId);
    sendStatus(clientId, "CONNECTED");
    return;
  }

  if (eventType == WStype_DISCONNECTED) {
    Serial.printf("[WS] Client #%u DISCONNECTED | armed=%d owner=%d\n",
                  clientId, robot.armed, robot.controllerId);
    if (robot.controllerId == clientId) {
      stopMotors();
      robot.armed = false;
      robot.controllerId = -1;
      Serial.println("[SAFETY] Controller terputus -> motor berhenti");
    }
    return;
  }

  if (eventType != WStype_TEXT || length == 0 || length > 256) return;

  JsonDocument document;
  if (deserializeJson(document, payload, length)) return;

  const char* command = document["type"] | "";

  if (strcmp(command, "drive") != 0) {
    Serial.printf("[RX] Client #%u command=%s | armed=%d owner=%d\n",
                  clientId, command, robot.armed, robot.controllerId);
  }

  if (strcmp(command, "arm") == 0) {
    armRobot(clientId);
    return;
  }

  // Emergency stop boleh digunakan oleh client mana pun.
  if (strcmp(command, "stop") == 0) {
    disarmRobot(clientId, "STOPPED");
    return;
  }

  if (robot.controllerId != clientId) {
    sendStatus(clientId, "BUSY");
    return;
  }

  if (strcmp(command, "disarm") == 0) {
    disarmRobot(clientId, "DISARMED");
    return;
  }

  if (strcmp(command, "drive") != 0 || !robot.armed ||
      !isValidDriveCommand(document)) {
    return;
  }

  const int x = document["x"];
  const int y = document["y"];

  robot.lastCommandAt = millis();
  driveRobot(x, y);

  // Browser mengulang command setiap 40 ms. Log hanya saat arah berubah.
  if (x != robot.lastLoggedX || y != robot.lastLoggedY) {
    Serial.printf(
        "[DRIVE] Client #%u x=%d y=%d power=%u%% -> L=%d R=%d\n",
        clientId, x, y, Config::DRIVE_SPEED_PERCENT,
        robot.leftPwm, robot.rightPwm);
    robot.lastLoggedX = x;
    robot.lastLoggedY = y;
  }

  if (millis() - robot.lastTelemetryAt >= Config::TELEMETRY_INTERVAL_MS) {
    robot.lastTelemetryAt = millis();
    sendStatus(clientId, "DRIVING");
  }
}

void setupMotors() {
  pinMode(Config::PIN_IN1, OUTPUT);
  pinMode(Config::PIN_IN2, OUTPUT);
  pinMode(Config::PIN_IN3, OUTPUT);
  pinMode(Config::PIN_IN4, OUTPUT);

  digitalWrite(Config::PIN_IN1, LOW);
  digitalWrite(Config::PIN_IN2, LOW);
  digitalWrite(Config::PIN_IN3, LOW);
  digitalWrite(Config::PIN_IN4, LOW);

  ledcSetup(Config::PWM_CHANNEL_LEFT,
            Config::PWM_FREQUENCY, Config::PWM_RESOLUTION);
  ledcSetup(Config::PWM_CHANNEL_RIGHT,
            Config::PWM_FREQUENCY, Config::PWM_RESOLUTION);
  ledcAttachPin(Config::PIN_ENA, Config::PWM_CHANNEL_LEFT);
  ledcAttachPin(Config::PIN_ENB, Config::PWM_CHANNEL_RIGHT);
  stopMotors();
}

void setupWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.setSleep(false);

  const bool started = WiFi.softAP(
      Config::WIFI_SSID,
      Config::WIFI_PASSWORD,
      Config::WIFI_CHANNEL,
      false,
      2);

  Serial.println();
  Serial.println("================================");
  Serial.println(Config::ROBOT_NAME);
  Serial.printf("Wi-Fi: %s\n", started ? "READY" : "FAILED");
  Serial.printf("SSID: %s\n", Config::WIFI_SSID);
  Serial.print("URL: http://");
  Serial.println(WiFi.softAPIP());
  Serial.println("WebSocket: port 81");
  Serial.println("================================");
}

void setupHttpServer() {
  httpServer.on("/", HTTP_GET, []() {
    Serial.println("[HTTP] Halaman controller diminta oleh client");
    httpServer.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate");
    httpServer.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
  });

  httpServer.onNotFound([]() {
    httpServer.sendHeader("Location", "/");
    httpServer.send(302, "text/plain", "");
  });

  httpServer.begin();
}

void setup() {
  Serial.begin(115200);
  setupMotors();
  setupWiFi();
  setupHttpServer();

  webSocket.begin();
  webSocket.onEvent(handleWebSocketEvent);

  stopMotors();
  Serial.println("SYSTEM READY");
}

void loop() {
  httpServer.handleClient();
  webSocket.loop();

  if (robot.armed &&
      millis() - robot.lastCommandAt > Config::COMMAND_TIMEOUT_MS) {
    const int timedOutClient = robot.controllerId;
    stopMotors();
    robot.armed = false;
    robot.controllerId = -1;

    if (timedOutClient >= 0) {
      Serial.printf("[SAFETY] Client #%d timeout -> motor berhenti\n",
                    timedOutClient);
      sendStatus(static_cast<uint8_t>(timedOutClient), "TIMEOUT");
    }
  }

  delay(1);
}
