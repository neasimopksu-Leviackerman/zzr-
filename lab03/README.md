# Lab03 - millis() 函数与精确定时

## 实验目标
1. 理解 millis() 函数的工作原理
2. 掌握使用 millis() 实现非阻塞延时
3. 相比 delay() 的优势
4. 为后续复杂项目打基础

## 实验要求
- 使用 millis() 函数获取系统运行时间
- 实现非阻塞延时控制
- 对比 delay() 和 millis() 的差异
- 实现多个 LED 独立闪烁（演示 millis() 优势）

## millis() 函数说明
```cpp
unsigned long millis()
```
- 返回值：Arduino 单片机从启动到现在运行的毫秒数
- 返回值类型：unsigned long（最大值约 49.7 天）
- 精度：毫秒级
- 不会阻塞程序执行

## 使用场景
1. 需要同时控制多个时间间隔的场景
2. 需要响应串口或其他中断输入的场景
3. 需要精确测量时间间隔的场景

## 基本用法
```cpp
unsigned long previousMillis = 0;
unsigned long interval = 500;  // 间隔（毫秒）

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // 执行需要定时的操作
  }
}
```
