// Ex05 - 多档位触摸调速呼吸灯
// 说明：每次触摸切换速度档位，LED 持续呼吸。

const int TOUCH_PIN = 2;
const int LED_PIN = 9;     // PWM 引脚
const unsigned long DEBOUNCE_DELAY = 50;

int speedLevel = 1;          // 1, 2, 3 档
int brightness = 0;
int fadeAmount = 4;

bool buttonState = false;
bool lastButtonState = false;
bool lastReading = false;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("====================================");
  Serial.println("Ex05: Touch-Controlled Breathing LED");
  Serial.println("====================================");
  printSpeedInfo();
}

void loop() {
  bool reading = digitalRead(TOUCH_PIN) == HIGH;

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == true && lastButtonState == false) {
        speedLevel = speedLevel % 3 + 1;
        printSpeedInfo();
      }
    }
  }

  lastButtonState = buttonState;
  lastReading = reading;

  analogWrite(LED_PIN, brightness);

  brightness += fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  delay(getSpeedDelay());
}

int getSpeedDelay() {
  switch (speedLevel) {
    case 1: return 16;  // 慢速
    case 2: return 8;   // 中速
    case 3: return 4;   // 快速
    default: return 10;
  }
}

void printSpeedInfo() {
  Serial.print("Touch detected. Current breathing speed: ");
  Serial.print(speedLevel);
  Serial.println(" (1=slow, 2=medium, 3=fast)");
}
