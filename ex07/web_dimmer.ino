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
<html>
<head>
  <meta charset="utf-8" />
  <title>ESP32 Web Dimmer</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; padding: 24px; }
    input[type=range] { width: 80%; margin-top: 24px; }
    .value { font-size: 2rem; margin-top: 16px; }
  </style>
</head>
<body>
  <h1>ESP32 Web 无极调光器</h1>
  <p>拖动滑动条调整 LED 亮度。</p>
  <input type="range" id="slider" min="0" max="255" value="128" />
  <div class="value">亮度: <span id="level">128</span></div>
  <script>
    const slider = document.getElementById('slider');
    const level = document.getElementById('level');

    function sendBrightness(value) {
      fetch('/setPWM?value=' + value)
        .catch(console.error);
    }

    slider.oninput = function() {
      level.textContent = this.value;
      sendBrightness(this.value);
    };
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

  pinMode(ledPin, OUTPUT);
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
