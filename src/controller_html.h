#pragma once

#include <Arduino.h>

#ifndef ROBOT_ID
#define ROBOT_ID 1
#endif

#if ROBOT_ID == 2
#define ROBOT_THEME_VARS \
  "      --blue: #43b5ff;\n" \
  "      --accent-light: #76ccff;\n" \
  "      --accent-dark: #176eae;\n" \
  "      --accent-text: #d9f2ff;\n" \
  "      --accent-rgb: 67,181,255;\n" \
  "      --accent-shadow-rgb: 23,110,174;\n"
#else
#define ROBOT_THEME_VARS \
  "      --blue: #ff4fa3;\n" \
  "      --accent-light: #ff70b8;\n" \
  "      --accent-dark: #b91f68;\n" \
  "      --accent-text: #ffd9ec;\n" \
  "      --accent-rgb: 255,79,163;\n" \
  "      --accent-shadow-rgb: 185,31,104;\n"
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
      --bg: #050505;
      --panel: #151719;
      --panel-dark: #0b0c0d;
      --line: #34383d;
      --text: #f5f5f2;
      --muted: #94989d;
)rawliteral" ROBOT_THEME_VARS R"rawliteral(
      --green: #35d08b;
      --amber: #f5b94b;
      --red: #ff5365;
    }

    * {
      box-sizing: border-box;
      -webkit-tap-highlight-color: transparent;
    }

    html, body {
      width: 100%;
      height: 100%;
      margin: 0;
      overflow: hidden;
      background:
        radial-gradient(circle at 22% 40%, rgba(var(--accent-rgb),.08), transparent 30%),
        linear-gradient(145deg, #050505, #0d0e0f 58%, #030303);
      color: var(--text);
      font-family: system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
      touch-action: none;
      user-select: none;
    }

    button, input { font: inherit; }

    .app {
      display: grid;
      grid-template-rows: auto 1fr;
      min-height: 100dvh;
      padding: env(safe-area-inset-top) env(safe-area-inset-right) env(safe-area-inset-bottom) env(safe-area-inset-left);
    }

    .topbar {
      display: flex;
      align-items: center;
      justify-content: space-between;
      min-height: 52px;
      padding: 8px 18px;
      border-bottom: 1px solid var(--line);
      background: rgba(8, 8, 8, .96);
      box-shadow: 0 12px 35px rgba(0,0,0,.32);
    }

    .brand { display: flex; align-items: center; }
    .title { font-size: 15px; font-weight: 900; letter-spacing: .08em; }
    .subtitle { margin-top: 2px; color: var(--muted); font-size: 9px; letter-spacing: .14em; }

    .connection {
      display: flex;
      align-items: center;
      gap: 8px;
      padding: 7px 11px;
      border: 1px solid var(--line);
      border-radius: 999px;
      background: #101112;
      color: var(--muted);
      font-size: 10px;
      font-weight: 900;
      letter-spacing: .06em;
    }
    .dot { width: 8px; height: 8px; border-radius: 50%; background: var(--red); }
    .connection.online .dot { background: var(--green); box-shadow: 0 0 10px var(--green); }
    .connection.armed .dot { background: var(--amber); box-shadow: 0 0 10px var(--amber); }
    .connection.disarmed .dot { background: var(--blue); box-shadow: 0 0 10px var(--blue); }
    .connection.busy .dot { background: var(--amber); box-shadow: 0 0 10px var(--amber); }
    .connection.timeout .dot { background: var(--red); box-shadow: 0 0 10px var(--red); }
    .top-actions { display: flex; align-items: center; gap: 8px; }
    .fullscreen-btn {
      display: grid;
      place-items: center;
      min-width: 36px;
      height: 32px;
      padding: 0 9px;
      border: 1px solid var(--line);
      border-radius: 9px;
      background: linear-gradient(145deg, #24272a, #111213);
      color: var(--accent-text);
      font-size: 10px;
      font-weight: 900;
      letter-spacing: .05em;
    }

    .main {
      display: grid;
      grid-template-columns: minmax(320px, 1.2fr) minmax(270px, .8fr);
      align-items: center;
      gap: clamp(18px, 4vw, 52px);
      min-height: 0;
      padding: 12px clamp(16px, 4vw, 52px);
    }

    .drive-panel { display: grid; place-items: center; min-width: 0; }
    .dpad {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      grid-template-rows: repeat(3, 1fr);
      gap: clamp(7px, 1.5vh, 12px);
      width: min(59vh, 440px, 48vw);
      aspect-ratio: 1;
    }

    .dir, .center-stop {
      display: grid;
      place-items: center;
      min-width: 0;
      min-height: 0;
      padding: 0;
      border: 1px solid #3b3f43;
      border-radius: clamp(12px, 2.5vh, 20px);
      background: linear-gradient(145deg, #25282b, #101112);
      color: #e7e4dc;
      box-shadow: inset 0 1px 0 rgba(255,255,255,.08), 0 10px 24px rgba(0,0,0,.38);
      font-size: clamp(27px, 6vh, 49px);
      font-weight: 900;
      cursor: pointer;
      transition: transform .08s, background .08s, border-color .08s;
    }
    .dir:active, .dir.active {
      transform: scale(.94);
      border-color: var(--blue);
      background: linear-gradient(145deg, var(--accent-light), var(--accent-dark));
      color: #090909;
      box-shadow: 0 0 0 2px rgba(var(--accent-rgb),.14), 0 0 26px rgba(var(--accent-rgb),.32);
    }
    .dir:disabled { opacity: .35; filter: grayscale(.5); }
    .center-stop {
      border-color: #743844;
      background: linear-gradient(145deg, #632b38, #3c1c26);
      color: #ffd7dc;
      font-size: clamp(10px, 2vh, 15px);
      letter-spacing: .08em;
    }
    .center-stop:active { transform: scale(.94); background: #8e2d3e; }

    .control-panel { display: grid; gap: 10px; width: 100%; max-width: 380px; }
    .card {
      padding: 13px 15px;
      border: 1px solid var(--line);
      border-radius: 15px;
      background: linear-gradient(145deg, rgba(28,30,32,.96), rgba(10,11,12,.97));
      box-shadow: inset 0 1px 0 rgba(255,255,255,.04), 0 14px 32px rgba(0,0,0,.25);
    }
    .actions { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
    .action {
      min-height: 52px;
      border: 0;
      border-radius: 13px;
      color: white;
      font-size: 11px;
      font-weight: 900;
      letter-spacing: .04em;
    }
    #armBtn { background: linear-gradient(145deg, #179960, #117344); }
    #stopBtn { background: linear-gradient(145deg, #e14254, #a91f35); }

    .telemetry { display: grid; grid-template-columns: repeat(4, 1fr); gap: 7px; }
    .metric { padding: 9px 4px; border-radius: 10px; background: #090a0b; text-align: center; }
    .metric span { display: block; font-size: 16px; font-weight: 900; }
    .metric small { display: block; margin-top: 3px; color: var(--muted); font-size: 8px; font-weight: 800; letter-spacing: .07em; }
    .hint { color: var(--muted); font-size: 9px; line-height: 1.4; text-align: center; }

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
    .rotate-brand {
      color: var(--blue);
      font-size: 9px;
      font-weight: 900;
      letter-spacing: .24em;
    }
    .phone-scene {
      position: relative;
      display: grid;
      place-items: center;
      width: 150px;
      height: 122px;
      margin: 15px auto 9px;
    }
    .phone-scene::before {
      content: "";
      position: absolute;
      width: 112px;
      height: 112px;
      border: 1px dashed rgba(var(--accent-rgb),.4);
      border-radius: 50%;
      animation: orbit 8s linear infinite;
    }
    .phone-shape {
      position: relative;
      width: 52px;
      height: 92px;
      padding: 6px;
      border: 3px solid #d8edff;
      border-radius: 11px;
      background: #07111f;
      box-shadow: 0 0 25px rgba(var(--accent-rgb),.22);
      animation: phoneTurn 2.8s ease-in-out infinite;
    }
    .phone-shape::before {
      content: "";
      position: absolute;
      top: 3px;
      left: 50%;
      width: 15px;
      height: 2px;
      border-radius: 2px;
      background: #7895ae;
      transform: translateX(-50%);
    }
    .phone-screen {
      display: grid;
      place-items: center;
      width: 100%;
      height: 100%;
      border-radius: 5px;
      background: linear-gradient(145deg, #303236, #111214);
      color: var(--blue);
      font-size: 25px;
      font-weight: 900;
    }
    .rotate-overlay h2 { margin: 8px 0 7px; font-size: 22px; line-height: 1.15; }
    .rotate-overlay p { margin: 0 auto 15px; max-width: 310px; color: var(--muted); font-size: 13px; line-height: 1.5; }
    .portrait-status {
      display: inline-flex;
      align-items: center;
      gap: 7px;
      margin-bottom: 16px;
      padding: 6px 10px;
      border: 1px solid var(--line);
      border-radius: 999px;
      background: #0b0c0d;
      color: var(--muted);
      font-size: 9px;
      font-weight: 900;
      letter-spacing: .07em;
    }
    .portrait-dot { width: 7px; height: 7px; border-radius: 50%; background: var(--red); }
    .portrait-status.online .portrait-dot { background: var(--green); box-shadow: 0 0 9px var(--green); }
    .rotate-steps { display: grid; grid-template-columns: repeat(3, 1fr); gap: 7px; margin-bottom: 16px; }
    .rotate-step {
      padding: 8px 4px;
      border-radius: 9px;
      background: rgba(5,5,5,.72);
      color: var(--muted);
      font-size: 8px;
      font-weight: 800;
      letter-spacing: .05em;
    }
    .rotate-step b { display: block; margin-bottom: 3px; color: var(--blue); font-size: 11px; }
    .rotate-fullscreen {
      width: 100%;
      min-height: 48px;
      padding: 0 18px;
      border: 0;
      border-radius: 14px;
      background: linear-gradient(145deg, var(--blue), var(--accent-dark));
      color: white;
      font-size: 12px;
      font-weight: 900;
      letter-spacing: .04em;
      box-shadow: 0 12px 28px rgba(var(--accent-shadow-rgb),.3);
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

    /* Tactical match dashboard
    body {
      background:
        radial-gradient(circle at 17% 48%, rgba(38, 211, 118, .14), transparent 28%),
        radial-gradient(circle at 88% 20%, rgba(86, 217, 255, .09), transparent 28%),
        linear-gradient(135deg, #06100f 0%, #091816 52%, #050c0c 100%);
    }

    body::before {
      content: "";
      position: fixed;
      inset: 0;
      pointer-events: none;
      opacity: .13;
      background-image:
        linear-gradient(rgba(128,255,190,.16) 1px, transparent 1px),
        linear-gradient(90deg, rgba(128,255,190,.16) 1px, transparent 1px);
      background-size: 42px 42px;
      mask-image: linear-gradient(90deg, black, transparent 72%);
    }

    .app { position: relative; isolation: isolate; }
    .topbar {
      min-height: 58px;
      border-bottom-color: rgba(88,242,154,.2);
      background: rgba(5, 17, 15, .84);
      box-shadow: 0 12px 40px rgba(0,0,0,.22);
      backdrop-filter: blur(16px);
    }
    .brand-mark {
      border-color: rgba(88,242,154,.55);
      border-radius: 50%;
      background: radial-gradient(circle at 35% 30%, #244d3d, #0a1915 68%);
      color: var(--green);
      box-shadow: inset 0 0 14px rgba(88,242,154,.12), 0 0 18px rgba(88,242,154,.12);
    }
    .title { color: #f6fff9; }
    .subtitle { color: #6d9d8f; }
    .connection, .fullscreen-btn {
      border-color: rgba(88,242,154,.24);
      background: rgba(9,28,23,.78);
    }
    .connection.online { color: #b7f8d2; }
    .connection.armed { color: #ffe6a7; border-color: rgba(255,200,87,.38); }
    .connection.online .dot, .connection.armed .dot { animation: statusPulse 1.8s infinite; }

    .main {
      grid-template-columns: minmax(340px, 1.18fr) minmax(280px, .82fr);
      gap: clamp(24px, 5vw, 72px);
    }
    .drive-panel {
      position: relative;
      grid-template-rows: auto 1fr auto;
      gap: 7px;
    }
    .section-kicker {
      color: #65a893;
      font-size: 9px;
      font-weight: 900;
      letter-spacing: .28em;
    }
    .drive-shell {
      position: relative;
      display: grid;
      place-items: center;
      padding: clamp(8px, 1.8vh, 14px);
      border: 1px solid rgba(88,242,154,.18);
      border-radius: 50%;
      background:
        radial-gradient(circle, rgba(18,58,43,.8) 0 27%, transparent 28%),
        radial-gradient(circle, transparent 0 55%, rgba(88,242,154,.1) 56% 57%, transparent 58%),
        rgba(7,20,17,.52);
      box-shadow: inset 0 0 50px rgba(22,120,72,.08), 0 22px 55px rgba(0,0,0,.32);
    }
    .drive-shell::before,
    .drive-shell::after {
      content: "";
      position: absolute;
      pointer-events: none;
      background: rgba(88,242,154,.13);
    }
    .drive-shell::before { width: 1px; height: 92%; }
    .drive-shell::after { width: 92%; height: 1px; }
    .app.moving .drive-shell {
      border-color: rgba(88,242,154,.48);
      box-shadow: inset 0 0 55px rgba(22,120,72,.13), 0 0 38px rgba(32,190,106,.12), 0 22px 55px rgba(0,0,0,.32);
    }
    .dpad { position: relative; z-index: 1; width: min(53vh, 410px, 43vw); }
    .dir, .center-stop {
      border-color: rgba(94,168,144,.36);
      background: linear-gradient(145deg, rgba(27,60,51,.96), rgba(10,28,23,.98));
      color: #c7fce0;
      box-shadow: inset 0 1px 0 rgba(255,255,255,.08), 0 9px 22px rgba(0,0,0,.28);
    }
    .dir::after {
      content: "";
      position: absolute;
      inset: 8px;
      border: 1px solid rgba(117,211,177,.08);
      border-radius: inherit;
      pointer-events: none;
    }
    .dir { position: relative; overflow: hidden; }
    .dir:active, .dir.active {
      border-color: var(--green);
      background: linear-gradient(145deg, #25c976, #12874c);
      color: white;
      box-shadow: 0 0 0 3px rgba(88,242,154,.11), 0 0 28px rgba(43,226,129,.34);
    }
    .dir:disabled { opacity: .28; }
    .center-stop {
      border-color: rgba(255,82,104,.55);
      background: radial-gradient(circle, #8f2639, #461520 72%);
      box-shadow: inset 0 0 18px rgba(255,130,145,.15), 0 8px 22px rgba(0,0,0,.32);
    }

    .motion-state {
      display: flex;
      align-items: center;
      gap: 8px;
      min-width: 180px;
      padding: 6px 12px;
      border: 1px solid rgba(88,242,154,.2);
      border-radius: 999px;
      background: rgba(7,23,19,.74);
      justify-content: center;
    }
    .motion-state span { color: var(--muted); font-size: 8px; font-weight: 800; letter-spacing: .12em; }
    .motion-state strong { color: var(--green); font-size: 10px; letter-spacing: .08em; }

    .control-panel { gap: 9px; }
    .card {
      border-color: rgba(90,171,145,.24);
      background: linear-gradient(145deg, rgba(17,43,35,.9), rgba(7,23,19,.92));
      box-shadow: inset 0 1px 0 rgba(255,255,255,.035), 0 12px 30px rgba(0,0,0,.18);
      backdrop-filter: blur(12px);
    }
    .panel-eyebrow {
      display: block;
      margin-bottom: 4px;
      color: #5e8f81;
      font-size: 8px;
      font-weight: 900;
      letter-spacing: .2em;
    }
    .speed-value { color: var(--green); text-shadow: 0 0 16px rgba(88,242,154,.25); }
    input[type="range"] { accent-color: var(--green); }
    .action { position: relative; overflow: hidden; box-shadow: 0 10px 22px rgba(0,0,0,.2); }
    #armBtn { background: linear-gradient(135deg, #20bf70, #0e7544); }
    #stopBtn { background: linear-gradient(135deg, #ef4560, #951d31); }
    .telemetry { padding: 8px; }
    .metric {
      position: relative;
      overflow: hidden;
      border: 1px solid rgba(91,165,142,.13);
      background: rgba(3,15,12,.62);
    }
    .metric::after {
      content: "";
      position: absolute;
      right: 5px;
      bottom: 5px;
      width: 3px;
      height: 3px;
      border-radius: 50%;
      background: var(--green);
      box-shadow: 0 0 8px var(--green);
    }
    .metric span { color: #eafff2; font-variant-numeric: tabular-nums; }
    .hint::before { content: "LIVE CONTROL  /  "; color: var(--green); font-weight: 900; }

    .rotate-overlay {
      background:
        radial-gradient(circle at 50% 35%, rgba(38,211,118,.16), transparent 28%),
        #06100f;
    }
    .rotate-icon { color: var(--green); }
    .rotate-fullscreen { background: linear-gradient(135deg, #20bf70, #0e7544); }

    @keyframes statusPulse {
      0%, 100% { transform: scale(1); opacity: 1; }
      50% { transform: scale(.72); opacity: .55; }
    }
    */

    @media (orientation: portrait) {
      .rotate-overlay { display: grid; }
    }

    @media (max-height: 430px) and (orientation: landscape) {
      .topbar { min-height: 44px; }
      .main { padding-top: 7px; padding-bottom: 7px; }
      .card { padding: 8px 11px; }
      .action { min-height: 42px; }
      .hint { display: none; }
    }

    @media (max-width: 720px) and (orientation: landscape) {
      .main { grid-template-columns: 1fr 1fr; gap: 12px; padding-left: 12px; padding-right: 12px; }
      .dpad { width: min(68vh, 46vw); }
      .subtitle { display: none; }
    }
  </style>
</head>
<body>
  <div class="app">
    <header class="topbar">
      <div class="brand">
        <div>
          <div class="title" id="robotName">SOCCER BOT</div>
          <div class="subtitle">ESP32 DRIVE CONTROL</div>
        </div>
      </div>
      <div class="top-actions">
        <div class="connection" id="connectionBadge"><span class="dot"></span><span id="connectionText">DISCONNECTED</span></div>
        <div class="connection disarmed" id="armBadge"><span class="dot"></span><span id="armStatusText">DISARMED</span></div>
        <button class="fullscreen-btn" id="fullscreenBtn" aria-label="Layar penuh">FULL</button>
      </div>
    </header>

    <main class="main">
      <section class="drive-panel" aria-label="Kontrol arah">
        <div class="dpad">
          <button class="dir" data-x="-30" data-y="100" aria-label="Maju kiri">&#8598;</button>
          <button class="dir" data-x="0" data-y="100" aria-label="Maju">&#8593;</button>
          <button class="dir" data-x="30" data-y="100" aria-label="Maju kanan">&#8599;</button>
          <button class="dir" data-x="-100" data-y="0" aria-label="Putar kiri">&#8592;</button>
          <button class="center-stop" id="centerStop" aria-label="Berhenti">STOP</button>
          <button class="dir" data-x="100" data-y="0" aria-label="Putar kanan">&#8594;</button>
          <button class="dir" data-x="-30" data-y="-100" aria-label="Mundur kiri">&#8601;</button>
          <button class="dir" data-x="0" data-y="-100" aria-label="Mundur">&#8595;</button>
          <button class="dir" data-x="30" data-y="-100" aria-label="Mundur kanan">&#8600;</button>
        </div>
      </section>

      <aside class="control-panel">
        <div class="actions">
          <button class="action" id="armBtn">ARM ROBOT</button>
          <button class="action" id="stopBtn">EMERGENCY STOP</button>
        </div>

        <section class="card telemetry">
          <div class="metric"><span id="xValue">0</span><small>ARAH X</small></div>
          <div class="metric"><span id="yValue">0</span><small>ARAH Y</small></div>
          <div class="metric"><span id="motorL">0</span><small>MOTOR L</small></div>
          <div class="metric"><span id="motorR">0</span><small>MOTOR R</small></div>
        </section>

        <div class="hint">Tahan tombol arah untuk bergerak. Lepaskan untuk berhenti.</div>
      </aside>
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
    const stopBtn = document.getElementById("stopBtn");
    const centerStop = document.getElementById("centerStop");
    const fullscreenBtn = document.getElementById("fullscreenBtn");
    const rotateFullscreenBtn = document.getElementById("rotateFullscreenBtn");
    const directionButtons = [...document.querySelectorAll(".dir")];
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
    let activeButton = null;
    let reconnectTimer = null;
    const UPDATE_INTERVAL = 40;

    function setConnectionStatus(label, mode = "") {
      connectionText.textContent = label;
      connectionBadge.className = `connection ${mode}`;
      portraitConnection.textContent = label;
      portraitStatus.className = `portrait-status ${mode}`;
    }

    function setRobotStatus(label, mode = "disarmed") {
      armStatusText.textContent = label;
      armBadge.className = `connection ${mode}`;
    }

    function setArmed(value) {
      armed = value;
      armBtn.textContent = armed ? "DISARM ROBOT" : "ARM ROBOT";
      armBtn.style.background = armed
        ? "linear-gradient(145deg,#d18b25,#9b6215)"
        : "linear-gradient(145deg,#179960,#117344)";
      directionButtons.forEach(button => button.disabled = !armed);
      setRobotStatus(armed ? "ARMED" : "DISARMED", armed ? "armed" : "disarmed");
    }

    function updateDirection(nextX, nextY, button = null) {
      if (activeButton && activeButton !== button) activeButton.classList.remove("active");
      activeButton = button;
      x = nextX;
      y = nextY;
      if (activeButton) activeButton.classList.add("active");
      document.getElementById("xValue").textContent = x;
      document.getElementById("yValue").textContent = y;
    }

    function send(data) {
      if (ws?.readyState === WebSocket.OPEN) ws.send(JSON.stringify(data));
    }

    function sendDrive() {
      send({ type: "drive", x, y });
    }

    function stopDirection(sendNow = true) {
      updateDirection(0, 0);
      if (sendNow && connected && armed) sendDrive();
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
      } catch (_) {
        // Beberapa browser/OS menolak orientation lock; kontrol tetap dapat dipakai.
      }
    }

    async function toggleFullscreen() {
      if (!isFullscreen()) {
        await enterFullscreen();
        return;
      }
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
        stopDirection(false);
        setConnectionStatus("CONNECTED", "online");
      };

      ws.onclose = () => {
        connected = false;
        setArmed(false);
        stopDirection(false);
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
        else if (data.message === "TIMEOUT") {
          stopDirection(false);
          setRobotStatus("TIMEOUT", "timeout");
        } else if (data.message === "ARMED") setRobotStatus("ARMED", "armed");
        else if (data.message === "DISARMED" || data.message === "STOPPED") {
          stopDirection(false);
          setRobotStatus("DISARMED", "disarmed");
        }
      };
    }

    armBtn.addEventListener("click", () => {
      if (!connected) return;
      if (armed) {
        stopDirection();
        send({ type: "disarm" });
        setArmed(false);
      } else {
        stopDirection(false);
        send({ type: "arm" });
      }
    });

    fullscreenBtn.addEventListener("click", toggleFullscreen);
    rotateFullscreenBtn.addEventListener("click", enterFullscreen);
    document.addEventListener("fullscreenchange", () => {
      fullscreenBtn.textContent = isFullscreen() ? "EXIT" : "FULL";
    });

    function emergencyStop() {
      stopDirection(false);
      setArmed(false);
      send({ type: "stop" });
    }

    stopBtn.addEventListener("click", emergencyStop);
    centerStop.addEventListener("pointerdown", event => {
      event.preventDefault();
      stopDirection();
    });

    directionButtons.forEach(button => {
      button.disabled = true;

      button.addEventListener("pointerdown", event => {
        if (!armed) return;
        event.preventDefault();
        button.setPointerCapture(event.pointerId);
        updateDirection(Number(button.dataset.x), Number(button.dataset.y), button);
        sendDrive();
      });

      const release = () => {
        if (activeButton === button) stopDirection();
      };
      button.addEventListener("pointerup", release);
      button.addEventListener("pointercancel", release);
      button.addEventListener("lostpointercapture", release);
    });

    setInterval(() => {
      if (connected && armed) sendDrive();
    }, UPDATE_INTERVAL);

    function safetyDisarm() {
      if (!connected || !armed) return;
      stopDirection(false);
      setArmed(false);
      send({ type: "disarm" });
    }

    document.addEventListener("visibilitychange", () => {
      if (document.hidden) safetyDisarm();
    });
    window.addEventListener("pagehide", emergencyStop);
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
