# Lab02 - Arduino 基础与 LED 控制

## 实验目标
1. 理解 Arduino 编程基础
2. 掌握数字输出控制 LED
3. 学习延时函数的使用
4. 实现 LED 基本闪烁效果

## 实验要求
- 使用 digitalWrite() 函数控制 LED 引脚
- 使用 delay() 函数控制闪烁间隔
- 实现 LED 以 2Hz 频率闪烁（500ms 亮，500ms 暗）

## 硬件连接
- LED 正极：5V
- LED 负极：通过 220Ω 电阻连接到引脚 13

## 代码示例
```cpp
void setup() {
  pinMode(13, OUTPUT);  // 设置 13 引脚为输出模式
}

void loop() {
  digitalWrite(13, HIGH);  // 点亮 LED
  delay(500);               // 延时 500ms
  digitalWrite(13, LOW);    // 熄灭 LED
  delay(500);               // 延时 500ms
}
```

## 学习要点
- Arduino 程序基本结构：setup() 和 loop()
- pinMode()、digitalWrite()、delay() 等基本函数
- LED 电路原理与保护（限流电阻）
