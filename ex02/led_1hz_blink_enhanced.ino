// Ex02 - 1Hz LED 闪烁（改进版）
// 添加更多调试信息和灵活性

unsigned long previousMillis = 0;
const unsigned long interval = 500;  // 500ms
int ledState = LOW;
unsigned long blinkCount = 0;  // 记录闪烁次数

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, ledState);
  
  Serial.println("\n====================================");
  Serial.println("Ex02: 1Hz LED Blink with millis()");
  Serial.println("Enhanced Version with Counter");
  Serial.println("====================================\n");
  Serial.println("Start Time: 0ms");
  Serial.println("LED Blink Frequency: 1Hz (500ms ON, 500ms OFF)");
  Serial.println("=====================================\n");
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    ledState = (ledState == LOW) ? HIGH : LOW;
    digitalWrite(13, ledState);
    
    blinkCount++;
    
    // 格式化输出信息
    Serial.print("[");
    printTime(currentMillis);
    Serial.print("] Count: ");
    Serial.print(blinkCount);
    Serial.print(" | LED: ");
    Serial.print(ledState == HIGH ? "ON " : "OFF");
    Serial.print(" | Duration: ");
    Serial.print(currentMillis / 1000);
    Serial.println("s");
  }
  
  // 每 10 秒输出一条统计信息
  static unsigned long lastStats = 0;
  if (currentMillis - lastStats >= 10000) {
    lastStats = currentMillis;
    Serial.print("--- Statistics: ");
    Serial.print(blinkCount);
    Serial.print(" toggles in ");
    Serial.print(currentMillis / 1000);
    Serial.println(" seconds ---\n");
  }
}

// 辅助函数：格式化打印时间
void printTime(unsigned long ms) {
  unsigned long seconds = ms / 1000;
  unsigned long milliseconds = ms % 1000;
  
  Serial.print(seconds);
  Serial.print(".");
  
  if (milliseconds < 100) Serial.print("0");
  if (milliseconds < 10) Serial.print("0");
  Serial.print(milliseconds);
  Serial.print("s");
}
