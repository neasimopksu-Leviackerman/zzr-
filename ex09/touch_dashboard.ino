#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const int touchPin = 4;

WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>ESP32 Touch Dashboard</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; padding: 24px; }
    .box { margin-top: 40px; padding: 24px; border: 2px solid #333; display: inline-block; }
    .value { font-size: 4rem; margin-top: 16px; }
  </style>
</head>
<body>
  <h1>ESP32 实时传感器 Web 仪表盘</h1>
  <div class="box">
    <div>触摸传感器数值</div>
    <div class="value" id="sensorValue">--</div>
  </div>
  <script>
    function updateSensor() {
      fetch('/sensor')
        .then(response => response.text())
        .then(value => document.getElementById('sensorValue').textContent = value)
        .catch(console.error);
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
}

void loop() {
  server.handleClient();
}
