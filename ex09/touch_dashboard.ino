#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const int touchPin = 4;

WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ESP32 实时传感器仪表盘</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: 'Segoe UI', Arial, sans-serif;
      background: #0d1b2a; color: #e0e0e0; min-height: 100vh;
      display: flex; justify-content: center; align-items: center;
    }
    .dashboard {
      background: #1b2838; border-radius: 20px; padding: 36px 44px;
      box-shadow: 0 12px 40px rgba(0,0,0,0.5);
      text-align: center; max-width: 440px; width: 90%;
    }
    h1 { font-size: 1.3rem; color: #5dade2; margin-bottom: 6px; }
    .sub { font-size: 0.8rem; color: #666; margin-bottom: 24px; }
    .gauge-container {
      position: relative; width: 220px; height: 120px;
      margin: 0 auto 10px; overflow: hidden;
    }
    .gauge-bg {
      position: absolute; bottom: 0; left: 10px; right: 10px;
      height: 100px; border-radius: 100px 100px 0 0;
      background: conic-gradient(from 180deg,
        #e74c3c 0deg, #f39c12 30deg, #2ecc71 60deg,
        #2ecc71 120deg, #f39c12 150deg, #e74c3c 180deg);
      opacity: 0.3;
    }
    .gauge-fill {
      position: absolute; bottom: 0; left: 10px; right: 10px;
      height: 100px; border-radius: 100px 100px 0 0;
      background: conic-gradient(from 180deg,
        #e74c3c 0deg, #f39c12 30deg, #2ecc71 60deg,
        #2ecc71 120deg, #f39c12 150deg, #e74c3c 180deg);
      mask: conic-gradient(from 180deg, #000 0deg, #000 var(--angle, 90deg),
            transparent var(--angle, 90deg), transparent 180deg);
      -webkit-mask: conic-gradient(from 180deg, #000 0deg, #000 var(--angle, 90deg),
            transparent var(--angle, 90deg), transparent 180deg);
      transition: --angle 0.3s;
    }
    .gauge-value {
      position: absolute; bottom: 12px; left: 50%;
      transform: translateX(-50%);
      font-size: 3rem; font-weight: bold; color: #5dade2;
      text-shadow: 0 0 20px rgba(93,173,226,0.4);
      transition: color 0.3s;
    }
    .bar-container {
      width: 100%; height: 16px; background: #111;
      border-radius: 8px; margin: 16px 0; overflow: hidden;
    }
    .bar-fill {
      height: 100%; border-radius: 8px;
      transition: width 0.3s, background 0.3s;
    }
    .proximity-label {
      font-size: 0.85rem; color: #888; margin-top: 4px;
    }
    .proximity {
      font-size: 1.1rem; font-weight: bold; transition: color 0.3s;
    }
    .stats {
      display: flex; justify-content: space-around;
      margin-top: 20px; font-size: 0.8rem; color: #666;
    }
    .stats span { color: #aaa; }
  </style>
</head>
<body>
  <div class="dashboard">
    <h1>ESP32 实时传感器 Web 仪表盘</h1>
    <p class="sub">触摸传感器模拟量实时监控</p>

    <div class="gauge-container">
      <div class="gauge-bg"></div>
      <div class="gauge-value" id="sensorValue">--</div>
    </div>

    <div class="bar-container">
      <div class="bar-fill" id="bar" style="width:50%"></div>
    </div>

    <div class="proximity-label">
      接近程度: <span class="proximity" id="proximity">等待数据...</span>
    </div>

    <div class="stats">
      <div>远离 <span>≈ 80-100</span></div>
      <div>接近 <span>≈ 20-40</span></div>
      <div>触碰 <span>≈ &lt;20</span></div>
    </div>
  </div>

  <script>
    var valEl = document.getElementById('sensorValue');
    var barEl = document.getElementById('bar');
    var proxEl = document.getElementById('proximity');

    function updateSensor() {
      fetch('/sensor')
        .then(function(r) { return r.text(); })
        .then(function(v) {
          var val = parseInt(v);
          if (isNaN(val)) return;
          valEl.textContent = val;

          var pct = Math.min(100, Math.max(0, Math.round((val / 100) * 100)));
          barEl.style.width = pct + '%';

          var r, g, label, color;
          if (val < 20) {
            r = 231; g = 76; label = '触碰中!'; color = '#e74c3c';
          } else if (val < 40) {
            r = 243; g = 156; label = '非常接近'; color = '#f39c12';
          } else if (val < 60) {
            r = 241; g = 196; label = '接近'; color = '#f1c40f';
          } else {
            r = 46; g = 204; label = '远离'; color = '#2ecc71';
          }
          barEl.style.background = 'linear-gradient(to right, #1a472a, ' + color + ')';
          proxEl.textContent = label;
          proxEl.style.color = color;
          valEl.style.color = color;
          valEl.style.textShadow = '0 0 24px ' + color;
        })
        .catch(function(){});
    }
    setInterval(updateSensor, 200);
    updateSensor();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleSensor() {
  int value = touchRead(touchPin);
  server.send(200, "text/plain", String(value));
}

void setup() {
  Serial.begin(115200);
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
  server.on("/sensor", handleSensor);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
