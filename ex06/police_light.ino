// Ex06 - 警车双闪灯效（双通道 PWM）
// 说明：两个 LED 亮度呈反相变化，过渡柔和。

const int LED_A_PIN = 9;   // PWM 通道 A
const int LED_B_PIN = 10;  // PWM 通道 B

int brightness = 0;
int fadeAmount = 4;

void setup() {
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  Serial.begin(9600);

  Serial.println("====================================");
  Serial.println("Ex06: Police Light Dual PWM");
  Serial.println("====================================");
  Serial.println("LED A and LED B will fade in opposite phases.");
}

void loop() {
  analogWrite(LED_A_PIN, brightness);
  analogWrite(LED_B_PIN, 255 - brightness);

  brightness += fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
    Serial.print("Phase change: brightness = ");
    Serial.println(brightness);
  }

  delay(12);
}
