// Lab03 - millis() 函数学习与应用
// 演示 millis() 函数的基本用法和优势

unsigned long previousMillis = 0;
unsigned long interval = 500;  // 闪烁间隔（毫秒）
int ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  
  Serial.println("Lab03: millis() Function Study");
  Serial.println("Using millis() for non-blocking timing");
  Serial.println("LED will blink every 500ms");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 检查是否到达定时时间
  if (currentMillis - previousMillis >= interval) {
    // 更新上一次执行时间
    previousMillis = currentMillis;
    
    // 切换 LED 状态
    ledState = (ledState == LOW) ? HIGH : LOW;
    digitalWrite(13, ledState);
    
    // 输出当前时间和 LED 状态
    Serial.print("Time: ");
    Serial.print(currentMillis);
    Serial.print("ms, LED: ");
    Serial.println(ledState == HIGH ? "ON" : "OFF");
  }
  
  // 注意：这里可以执行其他代码，不会被延时阻塞
  // 例如读取传感器、处理用户输入等
  delay(10);  // 短延时防止串口输出过于频繁
}
