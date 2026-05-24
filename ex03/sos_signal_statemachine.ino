// Ex03 - SOS 闪烁信号（状态机版本）
// 使用状态机模式实现，代码更模块化

enum FlashState {
  FLASH_ON,        // LED 点亮
  FLASH_OFF,       // LED 熄灭
  PAUSE            // 停顿
};

// 摩尔斯码定义
struct MorseSymbol {
  const char* code;    // "·" 或 "—"
  unsigned long onDuration;
  unsigned long offDuration;
};

// 时间配置
const unsigned long SHORT_DURATION = 100;   // 短闪（·）
const unsigned long LONG_DURATION = 500;    // 长闪（—）
const unsigned long INTER_SYMBOL = 100;     // 符号间隔
const unsigned long INTER_LETTER = 300;     // 字母间隔
const unsigned long INTER_SOS = 1000;       // SOS 间隔

// SOS 序列数据
struct Pattern {
  const char* symbol;
  unsigned long duration;
};

const Pattern sos_sequence[] = {
  // S: · · ·
  {"·", SHORT_DURATION},
  {"·", SHORT_DURATION},
  {"·", SHORT_DURATION},
  // O: — — —
  {"—", LONG_DURATION},
  {"—", LONG_DURATION},
  {"—", LONG_DURATION},
  // S: · · ·
  {"·", SHORT_DURATION},
  {"·", SHORT_DURATION},
  {"·", SHORT_DURATION}
};

const int SEQUENCE_LENGTH = sizeof(sos_sequence) / sizeof(sos_sequence[0]);

// 全局状态
unsigned long stateStartTime = 0;
int currentIndex = 0;
FlashState currentState = FLASH_ON;
bool inPause = false;
unsigned long totalFlashes = 0;
unsigned long totalSOS = 0;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  Serial.println("\n================================================");
  Serial.println("Ex03: SOS Signal - State Machine Implementation");
  Serial.println("================================================");
  Serial.println("Pattern: S(···) O(———) S(···)");
  Serial.println("\nState Machine:");
  Serial.println("  FLASH_ON -> FLASH_OFF -> FLASH_ON (repeat)");
  Serial.println("  After complete sequence -> PAUSE");
  Serial.println("  PAUSE -> Back to start");
  Serial.println("================================================\n");
  
  stateStartTime = millis();
  startFlash();
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long elapsed = currentMillis - stateStartTime;
  
  switch (currentState) {
    case FLASH_ON:
      if (elapsed >= sos_sequence[currentIndex].duration) {
        // 关闭 LED，进入 OFF 状态
        digitalWrite(13, LOW);
        currentState = FLASH_OFF;
        stateStartTime = currentMillis;
      }
      break;
      
    case FLASH_OFF:
      // 确定关闭时长
      unsigned long offDuration;
      if (currentIndex >= SEQUENCE_LENGTH - 1) {
        // 最后一个符号，进入长停顿
        offDuration = INTER_SOS;
      } else if ((currentIndex + 1) % 3 == 0) {
        // 字母间隔（3 个符号后）
        offDuration = INTER_LETTER;
      } else {
        // 符号间隔
        offDuration = INTER_SYMBOL;
      }
      
      if (elapsed >= offDuration) {
        currentIndex++;
        
        if (currentIndex >= SEQUENCE_LENGTH) {
          // 完成一个 SOS，进入停顿
          currentState = FLASH_OFF;
          inPause = true;
          currentIndex = 0;
          totalSOS++;
          
          stateStartTime = currentMillis;
          Serial.print("[");
          Serial.print(currentMillis);
          Serial.print("ms] ===== SOS Cycle #");
          Serial.print(totalSOS);
          Serial.println(" Complete =====\n");
        } else {
          // 继续下一个符号
          startFlash();
        }
      }
      break;
      
    case PAUSE:
      if (elapsed >= INTER_SOS) {
        // 停顿结束，重新开始
        inPause = false;
        currentState = FLASH_ON;
        currentIndex = 0;
        stateStartTime = currentMillis;
        startFlash();
      }
      break;
  }
}

void startFlash() {
  if (currentIndex < SEQUENCE_LENGTH) {
    digitalWrite(13, HIGH);
    currentState = FLASH_ON;
    stateStartTime = millis();
    totalFlashes++;
    
    unsigned long currentMillis = millis();
    
    // 确定所属字母
    const char* letter = "";
    if (currentIndex < 3) {
      letter = "S";
    } else if (currentIndex < 6) {
      letter = "O";
    } else {
      letter = "S";
    }
    
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print("ms] Flash #");
    Serial.print(totalFlashes);
    Serial.print(" | Symbol ");
    Serial.print(currentIndex + 1);
    Serial.print("/");
    Serial.print(SEQUENCE_LENGTH);
    Serial.print(" [");
    Serial.print(letter);
    Serial.print("]: ");
    Serial.print(sos_sequence[currentIndex].symbol);
    Serial.print(" (");
    Serial.print(sos_sequence[currentIndex].duration);
    Serial.println("ms)");
  }
}
