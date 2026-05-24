# 🚀 快速开始指南 - ESP32 物联网实验

## ⏱️ 5 分钟快速设置

### 1️⃣ WiFi 配置（必做）

打开任何一个 `.ino` 文件，修改前两行：

```cpp
const char* ssid = "你的WiFi名称";        // ← 改这里
const char* password = "你的WiFi密码";     // ← 改这里
```

### 2️⃣ 连接硬件

根据实验选择接线：

| 实验 | LED | 触摸 |
|------|-----|------|
| Ex07 | 引脚2 | ❌ |
| Ex08 | 引脚2 | 引脚4 |
| Ex09 | ❌ | 引脚4 |

### 3️⃣ 上传程序

1. Arduino IDE > Tools > Board > **ESP32 Dev Module**
2. Tools > Port > **COM?**
3. 按 **Upload** 上传
4. 打开 **Serial Monitor**（波特率 115200）

### 4️⃣ 访问网页

在串口监视器中找到：
```
Connected. IP: 192.168.1.xxx
```

在浏览器输入：`http://192.168.1.xxx`

---

## 🎯 预期现象

### Ex07
拖动滑动条 → LED 亮度实时变化

### Ex08
1. 点击"布防" → 状态变为 ARMED
2. 触摸引脚 → LED 高频闪烁
3. 点击"撤防" → LED 熄灭

### Ex09
手指靠近引脚 → 网页数值实时减小

---

## ⚠️ 常见问题

| 问题 | 解决方案 |
|------|--------|
| 无法连接WiFi | 检查WiFi名密码，用2.4GHz |
| 网页无法访问 | 确认手机/电脑与ESP32同网 |
| LED不亮 | 检查极性、电阻接线 |
| 触摸无反应 | 调整 `touchThreshold` 参数 |

---

## 📚 详细指南

需要更多帮助？查看 [IOT_EXPERIMENT_GUIDE.md](./IOT_EXPERIMENT_GUIDE.md)

