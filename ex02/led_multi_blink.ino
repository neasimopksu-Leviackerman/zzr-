// Ex02 - 1Hz LED 闪烁（多LED版本）
// 展示 millis() 的优势：同时控制多个 LED

// LED 引脚和状态定义
const int LED_PIN_1 = 13;   // 红色 LED - 1Hz 闪烁
const int LED_PIN_2 = 12;   // 绿色 LED - 2Hz 闪烁

// 时间变量
unsigned long previousMillis_1 = 0;
unsigned long previousMillis_2 = 0;
unsigned long interval_1 = 500;   // 1Hz (500ms)
unsigned long interval_2 = 250;   // 2Hz (250ms)

int ledState_1 = LOW;
int ledState_2 = LOW;

void setup() {
  Serial.begin(9600);
  
  // 初始化 LED 引脚
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  
  // 初始状态：LED 关闭
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);
  
  Serial.println("\n==========================================");
  Serial.println("Ex02: Multi-LED Control with millis()");
  Serial.println("Demonstrating millis() Advantages");
  Serial.println("==========================================");
  Serial.println("LED 1 (Pin 13 - Red):   1Hz blink");
  Serial.println("LED 2 (Pin 12 - Green): 2Hz blink");
  Serial.println("==========================================\n");
  Serial.println("Key Advantage: Both LEDs run independently");
  Serial.println("without blocking each other!\n");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 控制 LED 1 - 1Hz
  if (currentMillis - previousMillis_1 >= interval_1) {
    previousMillis_1 = currentMillis;
    ledState_1 = (ledState_1 == LOW) ? HIGH : LOW;
    digitalWrite(LED_PIN_1, ledState_1);
    
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print("ms] LED 1: ");
    Serial.println(ledState_1 == HIGH ? "ON " : "OFF");
  }
  
  // 控制 LED 2 - 2Hz
  if (currentMillis - previousMillis_2 >= interval_2) {
    previousMillis_2 = currentMillis;
    ledState_2 = (ledState_2 == LOW) ? HIGH : LOW;
    digitalWrite(LED_PIN_2, ledState_2);
    
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print("ms] LED 2: ");
    Serial.println(ledState_2 == HIGH ? "ON " : "OFF");
  }
  
  // 无阻塞 - 可以在这里执行其他操作
  // delay(5);  // 可选：防止串口过度占用
}
