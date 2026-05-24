// Ex02 - 1Hz LED 闪烁（可配置版本）
// 支持通过串口调整闪烁频率

volatile unsigned long frequency = 1000;  // 闪烁周期（毫秒）
unsigned long previousMillis = 0;
unsigned long halfInterval = 500;  // 频率的一半
int ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  Serial.println("\n========================================");
  Serial.println("Ex02: Configurable 1Hz LED Blink");
  Serial.println("========================================");
  Serial.println("Current Frequency: 1Hz (1000ms period)");
  Serial.println("Commands:");
  Serial.println("  + : Increase frequency");
  Serial.println("  - : Decrease frequency");
  Serial.println("  r : Reset to 1Hz");
  Serial.println("  s : Show current status");
  Serial.println("========================================\n");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // LED 闪烁逻辑
  if (currentMillis - previousMillis >= halfInterval) {
    previousMillis = currentMillis;
    ledState = (ledState == LOW) ? HIGH : LOW;
    digitalWrite(13, ledState);
    
    // 输出状态
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print("ms] LED: ");
    Serial.print(ledState == HIGH ? "ON " : "OFF");
    Serial.print(" | Freq: ");
    Serial.print(1000 / frequency);
    Serial.print("Hz | Period: ");
    Serial.print(frequency);
    Serial.println("ms");
  }
  
  // 串口命令处理
  if (Serial.available()) {
    char command = Serial.read();
    
    switch (command) {
      case '+':
      case 'a':  // Add
        if (frequency > 100) {
          frequency -= 100;  // 缩短周期 = 增加频率
          halfInterval = frequency / 2;
          Serial.print("Frequency increased to ");
          Serial.print(1000.0 / frequency);
          Serial.println("Hz");
        } else {
          Serial.println("Frequency already at maximum!");
        }
        break;
        
      case '-':
      case 'd':  // Decrease
        if (frequency < 5000) {
          frequency += 100;  // 延长周期 = 降低频率
          halfInterval = frequency / 2;
          Serial.print("Frequency decreased to ");
          Serial.print(1000.0 / frequency);
          Serial.println("Hz");
        } else {
          Serial.println("Frequency already at minimum!");
        }
        break;
        
      case 'r':  // Reset
        frequency = 1000;
        halfInterval = 500;
        Serial.println("Reset to 1Hz");
        break;
        
      case 's':  // Show status
        Serial.println("\n--- Current Status ---");
        Serial.print("Frequency: ");
        Serial.print(1000.0 / frequency);
        Serial.println("Hz");
        Serial.print("Period: ");
        Serial.print(frequency);
        Serial.println("ms");
        Serial.print("Current time: ");
        Serial.print(millis());
        Serial.println("ms");
        Serial.print("LED State: ");
        Serial.println(ledState == HIGH ? "ON" : "OFF");
        Serial.println("---\n");
        break;
        
      case '\n':
      case '\r':
        // 忽略换行符
        break;
        
      default:
        if (command >= 32 && command <= 126) {  // 可打印字符
          Serial.println("Unknown command!");
        }
        break;
    }
  }
}
