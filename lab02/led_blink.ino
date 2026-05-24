// Lab02 - LED 基本闪烁控制
// 使用 delay() 函数实现 LED 以 2Hz 频率闪烁

void setup() {
  // 初始化串口，用于调试
  Serial.begin(9600);
  
  // 设置 13 号引脚为输出模式
  pinMode(13, OUTPUT);
  
  Serial.println("Lab02: LED Blink Started");
  Serial.println("LED blinking at 2Hz");
}

void loop() {
  // 点亮 LED
  digitalWrite(13, HIGH);
  Serial.println("LED ON");
  delay(500);  // 延时 500ms（亮）
  
  // 熄灭 LED
  digitalWrite(13, LOW);
  Serial.println("LED OFF");
  delay(500);  // 延时 500ms（暗）
}
