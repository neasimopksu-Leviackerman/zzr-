#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const int ledPin = 2;
const int pwmChannel = 0;
const int pwmFrequency = 5000;
const int pwmResolution = 8;

WebServer server(80);
int brightness = 128;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ESP32 Web 无极调光器</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: 'Segoe UI', Arial, sans-serif;
      background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
      color: #eee; min-height: 100vh;
      display: flex; justify-content: center; align-items: center;
    }
    .card {
      background: #222; border-radius: 16px; padding: 32px 40px;
      box-shadow: 0 8px 32px rgba(0,0,0,0.4);
      text-align: center; max-width: 420px; width: 90%;
    }
    h1 { font-size: 1.4rem; margin-bottom: 8px; color: #ffd700; }
    .sub { font-size: 0.85rem; color: #888; margin-bottom: 28px; }
    .brightness-bar {
      width: 100%; height: 12px; border-radius: 6px;
      background: linear-gradient(to right, #333, #ffd700);
      margin-bottom: 20px; transition: box-shadow 0.15s;
    }
    input[type=range] {
      width: 100%; height: 8px; -webkit-appearance: none;
      background: linear-gradient(to right, #333 0%, #ffd700 100%);
      border-radius: 4px; outline: none; cursor: pointer;
    }
    input[type=range]::-webkit-slider-thumb {
      -webkit-appearance: none; width: 28px; height: 28px;
      border-radius: 50%; background: #ffd700;
      box-shadow: 0 0 12px rgba(255,215,0,0.5); cursor: pointer;
    }
    .value-display {
      margin-top: 20px; font-size: 3rem; font-weight: bold;
      color: #ffd700; transition: color 0.2s;
    }
    .percent { font-size: 1rem; color: #888; }
    .led-indicator {
      display: inline-block; width: 16px; height: 16px;
      border-radius: 50%; margin-right: 6px; vertical-align: middle;
      transition: background 0.15s;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>ESP32 Web 无极调光器</h1>
    <p class="sub">拖动滑动条实时调节 LED 亮度</p>
    <div class="brightness-bar" id="bar"></div>
    <input type="range" id="slider" min="0" max="255" value="128" />
    <div class="value-display">
      <span class="led-indicator" id="led"></span>
      <span id="level">128</span>
      <span class="percent" id="pct">(50%)</span>
    </div>
  </div>

  <script>
    const slider = document.getElementById('slider');
    const level = document.getElementById('level');
    const pct = document.getElementById('pct');
    const bar = document.getElementById('bar');
    const led = document.getElementById('led');
    let pendingValue = null, timer = null;

    function updateUI(v) {
      const p = Math.round((v / 255) * 100);
      level.textContent = v;
      pct.textContent = '(' + p + '%)';
      const c = 'rgb(' + (255 - v) + ',' + Math.round(v * 0.6) + ',0)';
      led.style.background = c;
      bar.style.boxShadow = '0 0 ' + (p / 15).toFixed(1) + 'px ' + c;
    }

    function sendBrightness(value) {
      fetch('/setPWM?value=' + value).catch(function(){});
    }

    slider.oninput = function() {
      const v = parseInt(this.value);
      updateUI(v);
      pendingValue = v;
      if (timer) clearTimeout(timer);
      timer = setTimeout(function() {
        sendBrightness(pendingValue);
        timer = null;
      }, 30);
    };

    slider.onchange = function() {
      if (timer) { clearTimeout(timer); timer = null; }
      sendBrightness(parseInt(this.value));
    };

    updateUI(128);
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleSetPWM() {
  if (server.hasArg("value")) {
    int value = server.arg("value").toInt();
    brightness = constrain(value, 0, 255);
    ledcWrite(pwmChannel, brightness);
    server.send(200, "text/plain", String(brightness));
  } else {
    server.send(400, "text/plain", "Missing value");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(ledPin, pwmChannel);
  ledcWrite(pwmChannel, brightness);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/setPWM", handleSetPWM);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
