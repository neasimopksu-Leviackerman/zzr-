// Ex02 - 使用 millis() 实现 1Hz LED 稳定闪烁
// 目标：LED 以 1Hz 频率稳定闪烁（500ms 亮，500ms 灭）

unsigned long previousMillis = 0;
const unsigned long interval = 500;  // 500ms = 1Hz 闪烁周期的一半
int ledState = LOW;

void setup() {
  // 初始化串口用于调试和状态显示
  Serial.begin(9600);
  
  // 设置 13 号引脚为输出模式
  pinMode(13, OUTPUT);
  
  // 初始化 LED 为熄灭状态
  digitalWrite(13, ledState);
  
  // 输出启动信息
  Serial.println("====================================");
  Serial.println("Ex02: 1Hz LED Blink with millis()");
  Serial.println("====================================");
  Serial.println("LED will blink at 1Hz frequency");
  Serial.println("ON: 500ms, OFF: 500ms");
  Serial.println("====================================\n");
}

void loop() {
  // 获取当前系统运行时间
  unsigned long currentMillis = millis();
  
  // 检查是否达到定时间隔
  if (currentMillis - previousMillis >= interval) {
    // 更新上一次执行的时间戳
    previousMillis = currentMillis;
    
    // 切换 LED 状态
    ledState = (ledState == LOW) ? HIGH : LOW;
    digitalWrite(13, ledState);
    
    // 输出当前状态信息
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print("ms] LED: ");
    if (ledState == HIGH) {
      Serial.println("ON  ■■■");
    } else {
      Serial.println("OFF ■□□");
    }
  }
  
  // 注意：loop() 会持续快速执行，millis() 计时的优势体现于此
  // 在实际应用中，可以在这里执行其他不阻塞的操作
  // 例如：读取按钮、处理通信等
}
