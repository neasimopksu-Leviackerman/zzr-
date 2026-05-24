// Ex03 - SOS 闪烁信号（改进版）
// 更清晰的代码结构和更详细的状态输出

// 时间常量
const unsigned long DOT_ON = 100;      // 点亮时长（短闪）
const unsigned long DASH_ON = 500;     // 点亮时长（长闪）
const unsigned long SYMBOL_GAP = 100;  // 符号间隔
const unsigned long LETTER_GAP = 300;  // 字母间隔
const unsigned long WORD_GAP = 1000;   // 单词间隔

// 状态定义
enum Symbol { DOT = 0, DASH = 1 };

// SOS 序列（S-O-S）
const Symbol sos_pattern[] = {
  // S: · · ·
  DOT, DOT, DOT,
  // O: — — —
  DASH, DASH, DASH,
  // S: · · ·
  DOT, DOT, DOT
};

const int PATTERN_LENGTH = sizeof(sos_pattern) / sizeof(sos_pattern[0]);

// 状态变量
unsigned long lastChangeTime = 0;
int currentSymbolIndex = 0;
bool ledOn = false;
unsigned long blinkCount = 0;
unsigned long sosCount = 0;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  Serial.println("\n============================================");
  Serial.println("Ex03: SOS Signal with millis() - Enhanced");
  Serial.println("============================================");
  Serial.println("Morse Code:");
  Serial.println("  S: · · ·");
  Serial.println("  O: — — —");
  Serial.println("  S: · · ·");
  Serial.println("\nTiming:");
  Serial.println("  Dot duration: 100ms");
  Serial.println("  Dash duration: 500ms");
  Serial.println("  Symbol gap: 100ms");
  Serial.println("  Word gap: 1000ms");
  Serial.println("============================================\n");
  
  lastChangeTime = millis();
  playNextSymbol();
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long elapsedTime = currentMillis - lastChangeTime;
  
  if (ledOn) {
    // LED 当前亮着，检查是否应该关闭
    unsigned long duration = (sos_pattern[currentSymbolIndex] == DOT) ? DOT_ON : DASH_ON;
    
    if (elapsedTime >= duration) {
      digitalWrite(13, LOW);
      ledOn = false;
      lastChangeTime = currentMillis;
      
      // 检查是否是最后一个符号
      if (currentSymbolIndex == PATTERN_LENGTH - 1) {
        // 最后一个符号，进入长停顿
        Serial.print("[");
        Serial.print(currentMillis);
        Serial.println("ms] ~~~~ Word Gap (1000ms) ~~~~");
      }
    }
  } else {
    // LED 关闭状态，检查是否应该点亮
    unsigned long gapDuration = (currentSymbolIndex == PATTERN_LENGTH - 1) ? WORD_GAP : SYMBOL_GAP;
    
    if (elapsedTime >= gapDuration) {
      if (currentSymbolIndex >= PATTERN_LENGTH) {
        // 完成一个 SOS 周期，重新开始
        currentSymbolIndex = 0;
        sosCount++;
        
        Serial.print("\n[");
        Serial.print(currentMillis);
        Serial.print("ms] ===== SOS #");
        Serial.print(sosCount);
        Serial.println(" =====\n");
      }
      
      playNextSymbol();
    }
  }
}

void playNextSymbol() {
  if (currentSymbolIndex < PATTERN_LENGTH) {
    digitalWrite(13, HIGH);
    ledOn = true;
    lastChangeTime = millis();
    
    unsigned long currentMillis = millis();
    
    // 输出符号信息
    Serial.print("[");
    Serial.print(currentMillis);
    Serial.print("ms] Symbol ");
    Serial.print(currentSymbolIndex + 1);
    Serial.print("/");
    Serial.print(PATTERN_LENGTH);
    Serial.print(": ");
    
    if (sos_pattern[currentSymbolIndex] == DOT) {
      Serial.print("· (DOT)");
    } else {
      Serial.print("— (DASH)");
    }
    
    // 显示字母位置
    if (currentSymbolIndex < 3) {
      Serial.print(" [S]");
    } else if (currentSymbolIndex < 6) {
      Serial.print(" [O]");
    } else {
      Serial.print(" [S]");
    }
    
    Serial.println();
    blinkCount++;
  } else {
    currentSymbolIndex++;  // 进入间隔状态
  }
}
