#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const int touchPin = 4;
const int ledPin = 2;
const int touchThreshold = 40;  // 根据环境调整，越小越灵敏

WebServer server(80);
bool armed = false;
bool alarmState = false;
bool lastTouch = false;
unsigned long lastFlashTime = 0;
bool flashLed = false;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>ESP32 安防报警器</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; padding: 24px; }
    button { font-size: 1rem; padding: 12px 24px; margin: 8px; }
    .status { font-size: 1.5rem; margin-top: 20px; }
  </style>
</head>
<body>
  <h1>ESP32 物联网安防报警器</h1>
  <button onclick="fetch('/arm')">布防 Arm</button>
  <button onclick="fetch('/disarm')">撤防 Disarm</button>
  <div class="status">当前状态: <span id="state">DISARMED</span></div>
  <script>
    function refreshStatus() {
      fetch('/status')
        .then(response => response.text())
        .then(text => document.getElementById('state').textContent = text)
        .catch(console.error);
    }
    setInterval(refreshStatus, 1000);
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
}

void loop() {
  server.handleClient();

  int touchValue = touchRead(touchPin);
  bool touched = touchValue < touchThreshold;

  if (armed && !alarmState && touched && !lastTouch) {
    alarmState = true;
    Serial.println("Alarm triggered!");
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
