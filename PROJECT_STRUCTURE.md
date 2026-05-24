# 项目结构指南

## 目录树

```
zzr-/
├── ex01/                      # 作业1 - C语言基础
│   ├── helloworld.c
│   ├── README.md
│   └── helloworld.exe
│
├── lab02/                     # 实验2 - Arduino 基础与 LED 控制
│   ├── README.md             # 实验说明
│   └── led_blink.ino         # 使用 delay() 实现 2Hz 闪烁
│
├── lab03/                     # 实验3 - millis() 函数与精确定时
│   ├── README.md             # 实验说明
│   └── millis_study.ino      # 非阻塞延时与多 LED 控制演示
│
├── ex02/                      # 作业2 - 1Hz LED 稳定闪烁
│   ├── README.md
│   ├── led_1hz_blink.ino                    # 基础版本
│   ├── led_1hz_blink_enhanced.ino           # 增强版本（计数、统计）
│   ├── led_multi_blink.ino                  # 多 LED 版本（演示优势）
│   └── led_configurable.ino                 # 可配置版本（串口控制）
│
├── ex03/                      # 作业3 - SOS 闪烁信号
│   ├── README.md
│   ├── sos_signal.ino                       # 原始版本
│   ├── sos_signal_enhanced.ino              # 改进版本（详细日志）
│   ├── sos_signal_statemachine.ino          # 状态机版本（设计模式）
│   └── sos_signal_programmable.ino          # 可编程版本（可扩展）
│
├── RESULTS.md                 # 实验总结与演示结果
├── PROJECT_STRUCTURE.md       # 本文件 - 项目结构指南
└── .git/                      # Git 版本控制目录
```

## 文件说明

### 核心实验代码

| 文件 | 功能 | 难度 | 提交次数 |
|------|------|------|---------|
| `lab02/led_blink.ino` | 基础 LED 闪烁 | ⭐ | 1 |
| `lab03/millis_study.ino` | millis() 学习 | ⭐⭐ | 1 |
| `ex02/led_1hz_blink.ino` | 1Hz LED 闪烁基础版 | ⭐⭐ | 3 |
| `ex02/led_1hz_blink_enhanced.ino` | 1Hz LED 闪烁增强版 | ⭐⭐ | 3 |
| `ex02/led_multi_blink.ino` | 多 LED 同步控制 | ⭐⭐⭐ | 3 |
| `ex02/led_configurable.ino` | 可配置频率控制 | ⭐⭐⭐ | 3 |
| `ex03/sos_signal.ino` | SOS 信号原始版 | ⭐⭐⭐ | 3 |
| `ex03/sos_signal_enhanced.ino` | SOS 信号改进版 | ⭐⭐⭐ | 3 |
| `ex03/sos_signal_statemachine.ino` | SOS 信号状态机版 | ⭐⭐⭐⭐ | 3 |
| `ex03/sos_signal_programmable.ino` | SOS 信号可编程版 | ⭐⭐⭐⭐ | 3 |

### 文档文件

| 文件 | 内容 |
|------|------|
| `README.md` | 项目根目录说明（原始） |
| `RESULTS.md` | 完整的实验总结和成果展示 |
| `PROJECT_STRUCTURE.md` | 本文件 - 项目结构和使用指南 |
| `lab02/README.md` | 实验2详细说明 |
| `lab03/README.md` | 实验3详细说明 |
| `ex02/README.md` | 作业2详细说明 |
| `ex03/README.md` | 作业3详细说明 |

## 快速开始

### 1. 查看项目结构
```bash
# 列出所有 Arduino 代码文件
dir *.ino /s

# 查看 Git 提交历史
git log --oneline
```

### 2. 上传代码到 Arduino

#### 步骤：
1. **准备硬件**
   - 连接 Arduino 开发板到计算机
   - 接上 LED 电路（推荐使用 Pin 13）

2. **打开 Arduino IDE**
   - 选择正确的开发板：`Tools > Board > Arduino Uno`
   - 选择正确的串口：`Tools > Port > COM*`

3. **选择要上传的文件**
   - 例如：`ex02/led_1hz_blink.ino`

4. **上传并监视**
   - 点击上传按钮
   - 打开串口监视器：`Tools > Serial Monitor`
   - 设置波特率为 **9600**

### 3. 观察现象

#### Ex02 - 1Hz LED 闪烁
- LED 以 1Hz 频率闪烁（500ms 亮 + 500ms 灭）
- 串口会输出实时状态信息
- 每个版本有不同的功能扩展

#### Ex03 - SOS 闪烁信号
- LED 产生 SOS 摩尔斯码信号
- 模式：· · · — — — · · · （重复）
- 串口会输出详细的符号执行日志

### 4. 使用串口命令（可配置版）

针对 `ex02/led_configurable.ino`：

```
命令        说明              当前频率
+           增加频率          ↑
-           降低频率          ↓
r           重置为 1Hz        1Hz
s           显示当前状态      状态信息
```

### 5. 提交新的修改

```bash
# 查看状态
git status

# 添加修改
git add .

# 提交修改
git commit -m "描述修改内容"

# 推送到远程
git push origin main
```

## 代码版本演进

### Ex02 演进路线

```
led_1hz_blink.ino (基础版)
    ↓
led_1hz_blink_enhanced.ino (增强版 - 添加计数和统计)
    ↓
led_multi_blink.ino (多 LED 版 - 展示 millis() 优势)
    ↓
led_configurable.ino (可配置版 - 增加交互性)
```

### Ex03 演进路线

```
sos_signal.ino (原始版)
    ↓
sos_signal_enhanced.ino (改进版 - 更好的日志输出)
    ↓
sos_signal_statemachine.ino (状态机版 - 设计模式)
    ↓
sos_signal_programmable.ino (可编程版 - 完整的摩尔斯码库)
```

## 关键技术点

### 1. delay() vs millis()

**delay() 方式**（Lab02）
```cpp
digitalWrite(13, HIGH);
delay(500);              // 阻塞等待 500ms
digitalWrite(13, LOW);
delay(500);              // 阻塞等待 500ms
// 此时无法做其他事
```

**millis() 方式**（Lab03+）
```cpp
if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // 执行操作（非阻塞）
}
// 可以继续执行其他代码
```

### 2. 摩尔斯码编码

**S** = · · · (三个点，每个 100ms)
**O** = — — — (三个划，每个 500ms)
**S** = · · · (三个点，每个 100ms)

每个信号间有 100ms 停顿，SOS 完成后停顿 1000ms

### 3. 状态机设计

```
[FLASH_ON] ──(点亮时长)──> [FLASH_OFF]
    ↑                            ↓
    └────(停顿时长)─────────────┘
```

## 故障排除

### 问题 1：Arduino 找不到串口
- **解决**：确保 USB 驱动程序已安装，尝试重新连接

### 问题 2：LED 不亮
- **检查**：
  - LED 极性是否正确（正极到 5V，负极到 GND via 电阻）
  - 引脚号是否正确（代码和硬件一致）
  - 是否有上传成功的提示信息

### 问题 3：闪烁频率不对
- **原因**：millis() 精度或代码逻辑
- **调试**：打开串口监视器查看时间戳

### 问题 4：串口命令无响应
- **检查**：
  - 波特率是否设置为 9600
  - 是否使用了 `Serial.available()` 的版本代码

## 推荐学习路线

1. **基础阶段** → Lab02
   - 了解 Arduino 基本语法
   - 掌握 delay() 的用法

2. **提升阶段** → Lab03
   - 学习 millis() 函数
   - 理解非阻塞编程的好处

3. **实践阶段** → Ex02（按顺序）
   - 基础版：掌握 millis() 应用
   - 增强版：添加功能（计数）
   - 多 LED 版：并行控制
   - 可配置版：动态控制

4. **高级阶段** → Ex03（按顺序）
   - 原始版：实现基本功能
   - 改进版：优化代码结构
   - 状态机版：学习设计模式
   - 可编程版：通用框架设计

## 开发环境

**推荐配置**：
- Arduino IDE 1.8.19+
- Arduino Uno 开发板
- USB 电缆
- LED x 3
- 220Ω 电阻 x 3
- 面包板和杜邦线

## 进一步扩展

### 可能的扩展项目：
1. 多个 LED 彩灯闪烁
2. 通过按钮切换闪烁模式
3. 使用 EEPROM 存储配置
4. 通过 WiFi 远程控制（需要模块）
5. 蜂鸣器与 LED 协调显示
6. 自定义摩尔斯码消息发送

---

**最后更新**：2026-05-24
**维护者**：2024117197
