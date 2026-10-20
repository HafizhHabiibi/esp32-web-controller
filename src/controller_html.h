#pragma once

#include <Arduino.h>

#ifndef ROBOT_ID
#define ROBOT_ID 1
#endif

#if ROBOT_ID == 2
#define ROBOT_THEME_VARS \
  "      --accent: #43b5ff;\n" \
  "      --accent-dark: #176eae;\n" \
  "      --accent-rgb: 67,181,255;\n"
#else
#define ROBOT_THEME_VARS \
  "      --accent: #ff4fa3;\n" \
  "      --accent-dark: #b91f68;\n" \
  "      --accent-rgb: 255,79,163;\n"
#endif

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="id">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no,viewport-fit=cover">
  <meta name="mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-capable" content="yes">
  <title>Soccer Bot Control</title>
  <style>
    :root {
      --bg: #060708;
      --panel: #111315;
      --panel-light: #1b1e21;
      --line: #30353a;
      --text: #f5f6f7;
      --muted: #8b9299;
)rawliteral" ROBOT_THEME_VARS R"rawliteral(
      --green: #35d08b;
      --amber: #f5b94b;
      --red: #ff5365;
    }

    * { box-sizing: border-box; -webkit-tap-highlight-color: transparent; }
    html, body {
      width: 100%; height: 100%; margin: 0; overflow: hidden;
      background: radial-gradient(circle at 50% 45%, rgba(var(--accent-rgb), .08), transparent 45%), var(--bg);
      color: var(--text);
      font-family: system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
      touch-action: none; user-select: none;
    }
    button { font: inherit; }
    .app {
      display: grid; grid-template-rows: auto 1fr; min-height: 100dvh;
      padding: env(safe-area-inset-top) env(safe-area-inset-right) env(safe-area-inset-bottom) env(safe-area-inset-left);
    }
    .topbar {
      display: flex; align-items: center; justify-content: space-between;
      min-height: 48px; padding: 7px 16px; border-bottom: 1px solid var(--line);
      background: rgba(8, 9, 10, .96);
    }
    .brand-title { font-size: 14px; font-weight: 900; letter-spacing: .1em; }
    .brand-subtitle { margin-top: 2px; color: var(--muted); font-size: 8px; letter-spacing: .16em; }
    .top-actions { display: flex; align-items: center; gap: 7px; }
    .badge {
      display: flex; align-items: center; gap: 7px; min-height: 30px; padding: 0 10px;
      border: 1px solid var(--line); border-radius: 999px; background: var(--panel);
      color: var(--muted); font-size: 9px; font-weight: 900; letter-spacing: .05em;
    }
    .dot { width: 7px; height: 7px; border-radius: 50%; background: var(--red); }
    .badge.online .dot { background: var(--green); box-shadow: 0 0 9px var(--green); }
    .badge.armed .dot { background: var(--amber); box-shadow: 0 0 9px var(--amber); }
    .badge.disarmed .dot { background: var(--accent); }
    .badge.busy .dot { background: var(--amber); }
    .badge.timeout .dot { background: var(--red); box-shadow: 0 0 9px var(--red); }
    .fullscreen-btn {
      min-width: 42px; height: 30px; border: 1px solid var(--line); border-radius: 9px;
      background: var(--panel-light); color: var(--text); font-size: 9px; font-weight: 900;
    }
    .controller {
      display: grid;
      grid-template-columns: minmax(210px, 1fr) minmax(170px, 250px) minmax(210px, 1fr);
      align-items: stretch; gap: clamp(12px, 2.5vw, 30px); min-height: 0;
      padding: clamp(10px, 2.5vh, 20px) clamp(12px, 3vw, 34px);
    }
    .control-zone { display: grid; align-content: center; gap: 9px; min-width: 0; }
    .zone-label {
      color: var(--muted); font-size: 9px; font-weight: 900;
      letter-spacing: .2em; text-align: center;
    }
    .steering-pad {
      display: grid; grid-template-columns: 1fr 1fr; gap: 10px;
      height: min(48vh, 250px);
    }
    .throttle-pad {
      display: grid; grid-template-rows: 1fr 1fr; gap: 10px;
      width: min(100%, 310px); height: min(48vh, 250px); justify-self: center;
    }
    .drive-btn {
      display: grid; place-items: center; min-width: 0; min-height: 0; padding: 0;
      border: 1px solid #41474d; border-radius: 20px;
      background: linear-gradient(145deg, #25292d, #111315); color: var(--text);
      box-shadow: inset 0 1px 0 rgba(255,255,255,.07), 0 12px 28px rgba(0,0,0,.32);
      font-size: clamp(34px, 9vh, 68px); font-weight: 900; cursor: pointer;
    }
    .drive-btn.active {
      transform: scale(.96); border-color: var(--accent);
      background: linear-gradient(145deg, var(--accent), var(--accent-dark)); color: #fff;
      box-shadow: 0 0 0 3px rgba(var(--accent-rgb), .13), 0 0 30px rgba(var(--accent-rgb), .25);
    }
    .drive-btn:disabled { opacity: .3; filter: grayscale(.65); }
    .center-panel { display: grid; align-content: center; gap: 9px; min-width: 0; }
    .action {
      width: min(100%, 200px); min-height: 48px; padding: 7px 10px;
      justify-self: center; border: 0; border-radius: 12px; color: #fff;
      font-size: 10px; font-weight: 900; letter-spacing: .04em;
      box-shadow: 0 10px 24px rgba(0,0,0,.26);
    }
    #armBtn { background: linear-gradient(145deg, #179960, #117344); }
    .telemetry {
      display: grid; grid-template-columns: 1fr 1fr; gap: 6px; padding: 8px;
      border: 1px solid var(--line); border-radius: 14px; background: var(--panel);
    }
    .metric { padding: 7px 3px; border-radius: 9px; background: #090a0b; text-align: center; }
    .metric span { display: block; font-size: 15px; font-weight: 900; font-variant-numeric: tabular-nums; }
    .metric small { display: block; margin-top: 2px; color: var(--muted); font-size: 7px; font-weight: 800; letter-spacing: .06em; }
    .hint { color: var(--muted); font-size: 8px; line-height: 1.45; text-align: center; }
    .rotate-overlay {
      display: none;
      position: fixed;
      inset: 0;
      z-index: 99;
      place-items: center;
      padding: max(22px, env(safe-area-inset-top)) 22px max(22px, env(safe-area-inset-bottom));
      background:
        radial-gradient(circle at 50% 22%, rgba(var(--accent-rgb),.2), transparent 28%),
        radial-gradient(circle at 12% 86%, rgba(255,255,255,.035), transparent 27%),
        linear-gradient(160deg, #030303 0%, #111214 55%, #050505 100%);
      text-align: center;
    }
    .rotate-card {
      width: min(100%, 390px);
      padding: 24px 22px 22px;
      border: 1px solid rgba(var(--accent-rgb),.28);
      border-radius: 28px;
      background: linear-gradient(145deg, rgba(29,31,33,.96), rgba(8,9,10,.98));
      box-shadow: inset 0 1px 0 rgba(255,255,255,.06), 0 28px 70px rgba(0,0,0,.38);
    }
    .rotate-brand { color: var(--accent); font-size: 9px; font-weight: 900; letter-spacing: .24em; }
    .phone-scene {
      position: relative; display: grid; place-items: center;
      width: 150px; height: 122px; margin: 15px auto 9px;
    }
    .phone-scene::before {
      content: ""; position: absolute; width: 112px; height: 112px;
      border: 1px dashed rgba(var(--accent-rgb),.4); border-radius: 50%;
      animation: orbit 8s linear infinite;
    }
    .phone-shape {
      position: relative; width: 52px; height: 92px; padding: 6px;
      border: 3px solid #d8edff; border-radius: 11px; background: #07111f;
      box-shadow: 0 0 25px rgba(var(--accent-rgb),.22);
      animation: phoneTurn 2.8s ease-in-out infinite;
    }
    .phone-shape::before {
      content: ""; position: absolute; top: 3px; left: 50%; width: 15px; height: 2px;
      border-radius: 2px; background: #7895ae; transform: translateX(-50%);
    }
    .phone-screen {
      display: grid; place-items: center; width: 100%; height: 100%; border-radius: 5px;
      background: linear-gradient(145deg, #303236, #111214);
      color: var(--accent); font-size: 25px; font-weight: 900;
    }
    .rotate-overlay h2 { margin: 8px 0 7px; font-size: 22px; line-height: 1.15; }
    .rotate-overlay p { margin: 0 auto 15px; max-width: 310px; color: var(--muted); font-size: 13px; line-height: 1.5; }
    .portrait-status {
      display: inline-flex; align-items: center; gap: 7px; margin-bottom: 16px;
      padding: 6px 10px; border: 1px solid var(--line); border-radius: 999px;
      background: #0b0c0d; color: var(--muted);
      font-size: 9px; font-weight: 900; letter-spacing: .07em;
    }
    .portrait-dot { width: 7px; height: 7px; border-radius: 50%; background: var(--red); }
    .portrait-status.online .portrait-dot { background: var(--green); box-shadow: 0 0 9px var(--green); }
    .rotate-steps { display: grid; grid-template-columns: repeat(3, 1fr); gap: 7px; margin-bottom: 16px; }
    .rotate-step {
      padding: 8px 4px; border-radius: 9px; background: rgba(5,5,5,.72);
      color: var(--muted); font-size: 8px; font-weight: 800; letter-spacing: .05em;
    }
    .rotate-step b { display: block; margin-bottom: 3px; color: var(--accent); font-size: 11px; }
    .rotate-fullscreen {
      width: 100%; min-height: 48px; padding: 0 18px; border: 0; border-radius: 14px;
      background: linear-gradient(145deg, var(--accent), var(--accent-dark));
      color: white; font-size: 12px; font-weight: 900; letter-spacing: .04em;
      box-shadow: 0 12px 28px rgba(var(--accent-rgb),.3);
    }
    @keyframes phoneTurn {
      0%, 28% { transform: rotate(0deg); }
      55%, 86% { transform: rotate(90deg); }
      100% { transform: rotate(0deg); }
    }
    @keyframes orbit { to { transform: rotate(360deg); } }
    @media (prefers-reduced-motion: reduce) {
      .phone-shape, .phone-scene::before { animation: none; }
      .phone-shape { transform: rotate(90deg); }
    }
    @media (orientation: portrait) { .rotate-overlay { display: grid; } }
    @media (max-height: 430px) and (orientation: landscape) {
      .topbar { min-height: 42px; }
      .controller { padding-top: 7px; padding-bottom: 7px; }
      .steering-pad, .throttle-pad { height: min(55vh, 220px); }
      .action { min-height: 44px; }
      .hint { display: none; }
    }
    @media (max-width: 740px) and (orientation: landscape) {
      .controller {
        grid-template-columns: minmax(160px, 1fr) minmax(145px, 190px) minmax(160px, 1fr);
        gap: 9px; padding-left: 9px; padding-right: 9px;
      }
      .brand-subtitle, #armBadge { display: none; }
      .steering-pad, .throttle-pad { gap: 7px; }
      .drive-btn { border-radius: 15px; }
    }
  </style>
</head>
<body>
  <div class="app">
    <header class="topbar">
      <div>
        <div class="brand-title" id="robotName">SOCCER BOT</div>
        <div class="brand-subtitle">DUAL TOUCH DRIVE</div>
      </div>
      <div class="top-actions">
        <div class="badge" id="connectionBadge"><span class="dot"></span><span id="connectionText">DISCONNECTED</span></div>
        <div class="badge disarmed" id="armBadge"><span class="dot"></span><span id="armStatusText">DISARMED</span></div>
        <button class="fullscreen-btn" id="fullscreenBtn" aria-label="Layar penuh">FULL</button>
      </div>
    </header>

    <main class="controller">
      <section class="control-zone" aria-label="Kontrol maju dan mundur">
        <div class="zone-label">MAJU / MUNDUR</div>
        <div class="throttle-pad">
          <button class="drive-btn" data-axis="y" data-value="100" aria-label="Maju">&#8593;</button>
          <button class="drive-btn" data-axis="y" data-value="-100" aria-label="Mundur">&#8595;</button>
        </div>
      </section>

      <section class="center-panel" aria-label="Kontrol utama">
        <button class="action" id="armBtn">ARM ROBOT</button>
        <div class="telemetry">
          <div class="metric"><span id="xValue">0</span><small>KEMUDI</small></div>
          <div class="metric"><span id="yValue">0</span><small>GAS</small></div>
          <div class="metric"><span id="motorL">0</span><small>MOTOR L</small></div>
          <div class="metric"><span id="motorR">0</span><small>MOTOR R</small></div>
        </div>
        <div class="hint">Tahan satu tombol kemudi dan satu tombol gas secara bersamaan.</div>
      </section>

      <section class="control-zone" aria-label="Kontrol belok">
        <div class="zone-label">KEMUDI</div>
        <div class="steering-pad">
          <button class="drive-btn" data-axis="x" data-value="-70" aria-label="Belok kiri">&#8592;</button>
          <button class="drive-btn" data-axis="x" data-value="70" aria-label="Belok kanan">&#8594;</button>
        </div>
      </section>
    </main>
  </div>

  <div class="rotate-overlay">
    <div class="rotate-card">
      <div class="rotate-brand">SOCCER BOT CONTROLLER</div>
      <div class="phone-scene" aria-hidden="true">
        <div class="phone-shape"><div class="phone-screen">&#8596;</div></div>
      </div>
      <h2>Putar HP ke horizontal</h2>
      <p>Area kontrol membutuhkan ruang landscape agar semua tombol mudah dijangkau dan robot tetap aman.</p>
      <div class="portrait-status" id="portraitStatus">
        <span class="portrait-dot"></span>
        <span id="portraitConnection">MENGHUBUNGKAN</span>
      </div>
      <div class="rotate-steps">
        <div class="rotate-step"><b>01</b>CONNECT</div>
        <div class="rotate-step"><b>02</b>ROTATE</div>
        <div class="rotate-step"><b>03</b>ARM</div>
      </div>
      <button class="rotate-fullscreen" id="rotateFullscreenBtn">PUTAR &amp; BUKA FULLSCREEN</button>
    </div>
  </div>

  <script>
    const armBtn = document.getElementById("armBtn");
    const fullscreenBtn = document.getElementById("fullscreenBtn");
    const rotateFullscreenBtn = document.getElementById("rotateFullscreenBtn");
    const driveButtons = [...document.querySelectorAll(".drive-btn")];
    const connectionBadge = document.getElementById("connectionBadge");
    const connectionText = document.getElementById("connectionText");
    const armBadge = document.getElementById("armBadge");
    const armStatusText = document.getElementById("armStatusText");
    const portraitStatus = document.getElementById("portraitStatus");
    const portraitConnection = document.getElementById("portraitConnection");

    let ws = null;
    let connected = false;
    let armed = false;
    let x = 0;
    let y = 0;
    let reconnectTimer = null;
    const activeControl = { x: null, y: null };
    const UPDATE_INTERVAL = 40;

    function setConnectionStatus(label, mode = "") {
      connectionText.textContent = label;
      connectionBadge.className = `badge ${mode}`;
      portraitConnection.textContent = label;
      portraitStatus.className = `portrait-status ${mode}`;
    }
    function setRobotStatus(label, mode = "disarmed") {
      armStatusText.textContent = label;
      armBadge.className = `badge ${mode}`;
    }
    function updateTelemetry() {
      document.getElementById("xValue").textContent = x;
      document.getElementById("yValue").textContent = y;
    }
    function clearControls(sendNow = true) {
      x = 0;
      y = 0;
      activeControl.x = null;
      activeControl.y = null;
      driveButtons.forEach(button => button.classList.remove("active"));
      updateTelemetry();
      if (sendNow && connected && armed) sendDrive();
    }
    function setArmed(value) {
      armed = value;
      armBtn.textContent = armed ? "DISARM ROBOT" : "ARM ROBOT";
      armBtn.style.background = armed
        ? "linear-gradient(145deg,#d18b25,#9b6215)"
        : "linear-gradient(145deg,#179960,#117344)";
      driveButtons.forEach(button => button.disabled = !armed);
      setRobotStatus(armed ? "ARMED" : "DISARMED", armed ? "armed" : "disarmed");
      if (!armed) clearControls(false);
    }
    function send(data) {
      if (ws?.readyState === WebSocket.OPEN) ws.send(JSON.stringify(data));
    }
    function sendDrive() { send({ type: "drive", x, y }); }
    function setAxis(axis, value, button) {
      const previous = activeControl[axis];
      if (previous && previous !== button) previous.classList.remove("active");
      activeControl[axis] = button;
      button.classList.add("active");
      if (axis === "x") x = value;
      else y = value;
      updateTelemetry();
      sendDrive();
    }
    function releaseAxis(button) {
      const axis = button.dataset.axis;
      if (activeControl[axis] !== button) return;
      activeControl[axis] = null;
      button.classList.remove("active");
      if (axis === "x") x = 0;
      else y = 0;
      updateTelemetry();
      if (connected && armed) sendDrive();
    }
    function isFullscreen() {
      return Boolean(document.fullscreenElement || document.webkitFullscreenElement);
    }
    async function enterFullscreen() {
      try {
        if (!isFullscreen()) {
          const root = document.documentElement;
          const request = root.requestFullscreen || root.webkitRequestFullscreen;
          if (request) await request.call(root);
        }
        if (screen.orientation?.lock) await screen.orientation.lock("landscape");
      } catch (_) {}
    }
    async function toggleFullscreen() {
      if (!isFullscreen()) return enterFullscreen();
      const exit = document.exitFullscreen || document.webkitExitFullscreen;
      if (exit) await exit.call(document);
    }
    function connectWebSocket() {
      clearTimeout(reconnectTimer);
      setConnectionStatus("CONNECTING");
      ws = new WebSocket(`ws://${location.hostname}:81/`);
      ws.onopen = () => {
        connected = true;
        setArmed(false);
        setConnectionStatus("CONNECTED", "online");
      };
      ws.onclose = () => {
        connected = false;
        setArmed(false);
        setConnectionStatus("DISCONNECTED");
        reconnectTimer = setTimeout(connectWebSocket, 1500);
      };
      ws.onerror = () => ws.close();
      ws.onmessage = event => {
        let data;
        try { data = JSON.parse(event.data); } catch (_) { return; }
        if (data.robot) document.getElementById("robotName").textContent = data.robot;
        if (data.type !== "status") return;
        if (typeof data.armed === "boolean") setArmed(data.armed);
        document.getElementById("motorL").textContent = data.left ?? 0;
        document.getElementById("motorR").textContent = data.right ?? 0;
        if (data.message === "BUSY") setRobotStatus("BUSY", "busy");
        else if (data.message === "TIMEOUT") setRobotStatus("TIMEOUT", "timeout");
        else if (data.message === "ARMED") setRobotStatus("ARMED", "armed");
        else if (data.message === "DISARMED" || data.message === "STOPPED") {
          setRobotStatus("DISARMED", "disarmed");
        }
      };
    }

    driveButtons.forEach(button => {
      button.disabled = true;
      button.addEventListener("pointerdown", event => {
        if (!armed) return;
        event.preventDefault();
        button.setPointerCapture(event.pointerId);
        setAxis(button.dataset.axis, Number(button.dataset.value), button);
      });
      button.addEventListener("pointerup", () => releaseAxis(button));
      button.addEventListener("pointercancel", () => releaseAxis(button));
      button.addEventListener("lostpointercapture", () => releaseAxis(button));
    });

    armBtn.addEventListener("click", () => {
      if (!connected) return;
      if (armed) {
        clearControls();
        send({ type: "disarm" });
        setArmed(false);
      } else {
        clearControls(false);
        send({ type: "arm" });
      }
    });
    fullscreenBtn.addEventListener("click", toggleFullscreen);
    rotateFullscreenBtn.addEventListener("click", enterFullscreen);
    document.addEventListener("fullscreenchange", () => {
      fullscreenBtn.textContent = isFullscreen() ? "EXIT" : "FULL";
    });
    setInterval(() => {
      if (connected && armed) sendDrive();
    }, UPDATE_INTERVAL);
    function safetyDisarm() {
      if (!connected || !armed) return;
      clearControls(false);
      setArmed(false);
      send({ type: "disarm" });
    }
    document.addEventListener("visibilitychange", () => {
      if (document.hidden) safetyDisarm();
    });
    window.addEventListener("pagehide", () => {
      clearControls(false);
      send({ type: "stop" });
    });
    window.addEventListener("orientationchange", () => {
      if (window.matchMedia("(orientation: portrait)").matches) safetyDisarm();
    });
    window.addEventListener("contextmenu", event => event.preventDefault());
    connectWebSocket();
  </script>
</body>
</html>
)rawliteral";

#undef ROBOT_THEME_VARS
