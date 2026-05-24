// Ex04 - 触摸自锁开关
// 说明：在检测到触摸按下的瞬间翻转 LED 状态，松开后保持当前状态。

const int TOUCH_PIN = 2;    // 触摸模块输出连接到数字引脚
const int LED_PIN = 13;     // LED 连接到数字 13
const unsigned long DEBOUNCE_DELAY = 50;  // 软件防抖时间

bool ledState = false;
bool buttonState = false;
bool lastButtonState = false;
bool lastReading = false;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("====================================");
  Serial.println("Ex04: Touch Latch Switch");
  Serial.println("====================================");
  Serial.println("Touch once to toggle LED ON and hold it.");
  Serial.println("Touch again to toggle LED OFF.");
  Serial.println("====================================\n");
}

void loop() {
  bool reading = digitalRead(TOUCH_PIN) == HIGH;

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == true) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
        Serial.print("Touch detected. LED is now ");
        Serial.println(ledState ? "ON" : "OFF");
      }
    }
  }

  lastReading = reading;
}
