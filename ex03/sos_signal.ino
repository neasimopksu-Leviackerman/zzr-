// Ex03 - 使用 millis() 实现 SOS 闪烁信号
// 目标：通过 LED 闪烁产生国际求救信号 SOS
// SOS = · · · — — — · · · （三短三长三短）

// 时间常量定义
const unsigned long SHORT_DURATION = 100;  // 短闪时长 (ms)
const unsigned long LONG_DURATION = 500;   // 长闪时长 (ms)
const unsigned long INTERVAL_SHORT = 100;  // 闪烁间隔 (ms)
const unsigned long PAUSE_DURATION = 1000; // 两个 SOS 之间的停顿 (ms)

// SOS 信号序列定义
// true = 短闪，false = 长闪
const bool sos_sequence[] = {
  // 第一个 S（三个短闪）
  true, true, true,
  // 第二个 O（三个长闪）
  false, false, false,
  // 第三个 S（三个短闪）
  true, true, true
};

const int sos_length = sizeof(sos_sequence) / sizeof(sos_sequence[0]);

// 状态变量
unsigned long previousMillis = 0;
unsigned long stateStartTime = 0;
int currentFlashIndex = 0;
int currentPhase = 0;  // 0: 亮，1: 暗
bool ledOn = false;
bool inPause = false;

// 枚举定义状态
enum State {
  FLASHING,      // LED 闪烁中
  INTERVAL,      // 闪烁间隔
  PAUSE          // SOS 结束停顿
};

State currentState = FLASHING;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);  // 初始 LED 关闭
  
  // 输出启动信息
  Serial.println("====================================");
  Serial.println("Ex03: SOS LED Signal with millis()");
  Serial.println("====================================");
  Serial.println("S: · · ·");
  Serial.println("O: — — —");
  Serial.println("S: · · ·");
  Serial.println("====================================\n");
  
  stateStartTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (inPause) {
    // 在暂停阶段
    if (currentMillis - stateStartTime >= PAUSE_DURATION) {
      // 暂停结束，重新开始 SOS 序列
      inPause = false;
      currentFlashIndex = 0;
      currentPhase = 0;
      stateStartTime = currentMillis;
      digitalWrite(13, HIGH);
      ledOn = true;
      printStatusMessage(currentMillis, "Pause End, Starting new SOS");
    }
  } else {
    // 正常的 SOS 闪烁阶段
    bool isShortFlash = sos_sequence[currentFlashIndex];
    unsigned long flashDuration = isShortFlash ? SHORT_DURATION : LONG_DURATION;
    
    // 当前阶段执行中
    if (currentPhase == 0) {
      // LED 点亮阶段
      if (currentMillis - stateStartTime >= flashDuration) {
        // 闪烁时长到达，关闭 LED
        digitalWrite(13, LOW);
        ledOn = false;
        currentPhase = 1;  // 切换到暗阶段
        stateStartTime = currentMillis;
        
        // 输出闪烁完成信息
        char flashType = isShortFlash ? '·' : '—';
        printStatusMessage(currentMillis, flashType);
      }
    } else {
      // LED 关闭阶段
      if (currentMillis - stateStartTime >= INTERVAL_SHORT) {
        // 间隔时长到达
        currentFlashIndex++;
        
        if (currentFlashIndex >= sos_length) {
          // 所有闪烁完成，进入长停顿
          inPause = true;
          stateStartTime = currentMillis;
          printStatusMessage(currentMillis, "SOS Complete, Pausing");
        } else {
          // 继续下一个闪烁
          digitalWrite(13, HIGH);
          ledOn = true;
          currentPhase = 0;
          stateStartTime = currentMillis;
        }
      }
    }
  }
}

// 输出状态信息函数
void printStatusMessage(unsigned long currentMillis, const char* message) {
  Serial.print("[");
  Serial.print(currentMillis);
  Serial.print("ms] ");
  Serial.println(message);
}

// 重载函数处理字符参数
void printStatusMessage(unsigned long currentMillis, char flashChar) {
  Serial.print("[");
  Serial.print(currentMillis);
  Serial.print("ms] Flash: ");
  Serial.println(flashChar);
}
