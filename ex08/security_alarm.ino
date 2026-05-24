#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const int touchPin = 4;
const int ledPin = 2;
const int touchThreshold = 40;

WebServer server(80);
bool armed = false;
bool alarmState = false;
bool lastTouch = false;
unsigned long lastFlashTime = 0;
bool flashLed = false;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ESP32 安防报警器</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: 'Segoe UI', Arial, sans-serif;
      background: #1a1a2e; color: #eee; min-height: 100vh;
      display: flex; justify-content: center; align-items: center;
    }
    .card {
      background: #222; border-radius: 16px; padding: 32px 40px;
      box-shadow: 0 8px 32px rgba(0,0,0,0.4);
      text-align: center; max-width: 400px; width: 90%;
    }
    h1 { font-size: 1.4rem; margin-bottom: 8px; }
    .sub { font-size: 0.85rem; color: #888; margin-bottom: 24px; }
    .buttons { display: flex; gap: 12px; justify-content: center; flex-wrap: wrap; }
    button {
      font-size: 1.1rem; padding: 14px 28px; border: none; border-radius: 10px;
      cursor: pointer; font-weight: bold; transition: transform 0.1s, box-shadow 0.2s;
    }
    button:active { transform: scale(0.96); }
    .btn-arm    { background: #c0392b; color: #fff; }
    .btn-arm:hover    { box-shadow: 0 0 18px rgba(192,57,43,0.6); }
    .btn-disarm { background: #27ae60; color: #fff; }
    .btn-disarm:hover { box-shadow: 0 0 18px rgba(39,174,96,0.6); }
    .status-box {
      margin-top: 28px; padding: 20px; border-radius: 12px;
      background: #1a1a1a; transition: background 0.3s, box-shadow 0.3s;
    }
    .status-label { font-size: 0.85rem; color: #888; margin-bottom: 6px; }
    .status-value { font-size: 2rem; font-weight: bold; transition: color 0.3s; }
    .status-DISARMED { color: #27ae60; }
    .status-ARMED    { color: #f39c12; }
    .status-ALARM     { color: #e74c3c; animation: pulse 0.5s infinite alternate; }
    @keyframes pulse { to { opacity: 0.4; } }
    .dot {
      display: inline-block; width: 14px; height: 14px; border-radius: 50%;
      margin-right: 8px; vertical-align: middle; transition: background 0.3s;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>ESP32 物联网安防报警器</h1>
    <p class="sub">布防后触碰传感器将触发报警</p>
    <div class="buttons">
      <button class="btn-arm" onclick="fetch('/arm')">布防 Arm</button>
      <button class="btn-disarm" onclick="fetch('/disarm')">撤防 Disarm</button>
    </div>
    <div class="status-box" id="statusBox">
      <div class="status-label">系统状态</div>
      <div class="status-value" id="state">
        <span class="dot" id="dot"></span><span id="stateText">DISARMED</span>
      </div>
    </div>
  </div>

  <script>
    var stateEl = document.getElementById('state');
    var dotEl = document.getElementById('dot');
    var boxEl = document.getElementById('statusBox');
    var textEl = document.getElementById('stateText');

    function refreshStatus() {
      fetch('/status')
        .then(function(r) { return r.text(); })
        .then(function(status) {
          textEl.textContent = status;
          stateEl.className = 'status-value status-' + status;
          if (status === 'DISARMED') {
            dotEl.style.background = '#27ae60';
            boxEl.style.boxShadow = 'none';
          } else if (status === 'ARMED') {
            dotEl.style.background = '#f39c12';
            boxEl.style.boxShadow = '0 0 20px rgba(243,156,18,0.3)';
          } else if (status === 'ALARM') {
            dotEl.style.background = '#e74c3c';
            boxEl.style.boxShadow = '0 0 30px rgba(231,76,60,0.6)';
          }
        })
        .catch(function(){});
    }
    setInterval(refreshStatus, 500);
    refreshStatus();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleArm() {
  armed = true;
  alarmState = false;
  server.send(200, "text/plain", "ARMED");
}

void handleDisarm() {
  armed = false;
  alarmState = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "DISARMED");
}

void handleStatus() {
  String status = armed ? (alarmState ? "ALARM" : "ARMED") : "DISARMED";
  server.send(200, "text/plain", status);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.on("/status", handleStatus);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  int touchValue = touchRead(touchPin);
  bool touched = touchValue < touchThreshold;

  if (armed && !alarmState && touched && !lastTouch) {
    alarmState = true;
    Serial.println("ALARM triggered!");
  }

  lastTouch = touched;

  if (alarmState) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastFlashTime > 100) {
      lastFlashTime = currentMillis;
      flashLed = !flashLed;
      digitalWrite(ledPin, flashLed ? HIGH : LOW);
    }
  }
}
