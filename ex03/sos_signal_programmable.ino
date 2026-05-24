// Ex03 - SOS 闪烁信号（可编程版本）
// 支持自定义摩尔斯码模式，可扩展性更强

// 摩尔斯码库
typedef struct {
  const char* symbol;          // 符号字符（中文：· —）
  unsigned long duration;      // 点亮时长
} MorseUnit;

// 定义摩尔斯码字母
typedef struct {
  const char letter;
  const MorseUnit* units;
  int unit_count;
} MorseChar;

// S: · · ·
const MorseUnit morse_S[] = {
  {"·", 100}, {"·", 100}, {"·", 100}
};

// O: — — —
const MorseUnit morse_O[] = {
  {"—", 500}, {"—", 500}, {"—", 500}
};

// SOS 消息
const MorseChar sos_message[] = {
  {'S', morse_S, 3},
  {'O', morse_O, 3},
  {'S', morse_S, 3}
};

const int MESSAGE_LENGTH = sizeof(sos_message) / sizeof(sos_message[0]);

// 时间参数
const unsigned long UNIT_GAP = 100;      // 单元间隔
const unsigned long CHAR_GAP = 300;      // 字符间隔
const unsigned long WORD_GAP = 1000;     // 单词间隔

// 状态变量
unsigned long lastStateChange = 0;
int messageIndex = 0;          // 当前字符索引
int unitIndex = 0;             // 当前单元索引
bool ledOn = false;            // LED 当前状态
unsigned long totalSOS = 0;    // 完整 SOS 次数
unsigned long systemStartTime = 0;

enum State {
  CHAR_ON,      // 字符点亮
  CHAR_OFF,     // 字符关闭
  IDLE          // 空闲等待
};

State currentState = CHAR_ON;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  systemStartTime = millis();
  
  Serial.println("\n=================================================");
  Serial.println("Ex03: SOS Signal - Programmable Morse Code");
  Serial.println("=================================================");
  Serial.println("Architecture:");
  Serial.println("  - Expandable morse code library");
  Serial.println("  - Message-based approach");
  Serial.println("  - Easy to add new characters/messages");
  Serial.println("\nCurrent Pattern: SOS (···|———|···)");
  Serial.println("=================================================\n");
  
  lastStateChange = millis();
  playNextUnit();
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long elapsed = currentMillis - lastStateChange;
  
  switch (currentState) {
    case CHAR_ON:
      // 检查点亮时长是否已满足
      if (elapsed >= sos_message[messageIndex].units[unitIndex].duration) {
        // 关闭 LED
        digitalWrite(13, LOW);
        ledOn = false;
        currentState = CHAR_OFF;
        lastStateChange = currentMillis;
      }
      break;
      
    case CHAR_OFF:
      // 决定关闭时长
      unsigned long gapDuration;
      
      if (unitIndex < sos_message[messageIndex].unit_count - 1) {
        // 同字符内的单元间隔
        gapDuration = UNIT_GAP;
      } else if (messageIndex < MESSAGE_LENGTH - 1) {
        // 字符间隔
        gapDuration = CHAR_GAP;
      } else {
        // 消息末尾，长停顿
        gapDuration = WORD_GAP;
      }
      
      if (elapsed >= gapDuration) {
        // 移动到下一个单元
        unitIndex++;
        
        if (unitIndex >= sos_message[messageIndex].unit_count) {
          // 当前字符完成，移动到下一个字符
          messageIndex++;
          unitIndex = 0;
          
          if (messageIndex >= MESSAGE_LENGTH) {
            // 整个消息完成，重新开始
            messageIndex = 0;
            unitIndex = 0;
            totalSOS++;
            currentState = CHAR_ON;
            lastStateChange = currentMillis;
            
            unsigned long uptimeSeconds = (currentMillis - systemStartTime) / 1000;
            Serial.print("\n[");
            Serial.print(currentMillis);
            Serial.print("ms - System Uptime: ");
            Serial.print(uptimeSeconds);
            Serial.print("s] ===== SOS #");
            Serial.print(totalSOS);
            Serial.println(" Complete =====\n");
            
            playNextUnit();
          } else {
            // 继续下一个字符
            currentState = CHAR_ON;
            lastStateChange = currentMillis;
            playNextUnit();
          }
        } else {
          // 继续同字符的下一个单元
          currentState = CHAR_ON;
          lastStateChange = currentMillis;
          playNextUnit();
        }
      }
      break;
      
    case IDLE:
      // 备用状态，目前未使用
      break;
  }
}

void playNextUnit() {
  if (messageIndex < MESSAGE_LENGTH && unitIndex < sos_message[messageIndex].unit_count) {
    // 点亮 LED
    digitalWrite(13, HIGH);
    ledOn = true;
    currentState = CHAR_ON;
    lastStateChange = millis();
    
    unsigned long currentMillis = millis();
    const MorseUnit* unit = &sos_message[messageIndex].units[unitIndex];
    
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print("ms] Morse: ");
    Serial.print(sos_message[messageIndex].letter);
    Serial.print(" (");
    Serial.print(unitIndex + 1);
    Serial.print("/");
    Serial.print(sos_message[messageIndex].unit_count);
    Serial.print(") -> ");
    Serial.print(unit->symbol);
    Serial.print(" (");
    Serial.print(unit->duration);
    Serial.println("ms)");
  }
}

// 扩展函数：可以添加自定义摩尔斯码
void printMorseGuide() {
  Serial.println("\nMorse Code Quick Reference:");
  Serial.println("· = DOT   (100ms)");
  Serial.println("— = DASH  (500ms)");
  Serial.println("\nExample characters:");
  Serial.println("A = ·—   (dit-dah)");
  Serial.println("B = —··· (dah-dit-dit-dit)");
  Serial.println("C = —·—· (dah-dit-dah-dit)");
}
