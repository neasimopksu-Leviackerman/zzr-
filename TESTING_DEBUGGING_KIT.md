# 实验验证与调试工具包

## 📋 预实验检查清单

### 硬件检查 ✓

- [ ] ESP32 开发板
- [ ] USB 数据线（能数据传输，不只是充电线）
- [ ] LED × 1
- [ ] 220Ω 电阻 × 2-3
- [ ] 杜邦线若干
- [ ] 面包板（可选但推荐）
- [ ] 手机或电脑（用于访问网页）

### 软件检查 ✓

- [ ] Arduino IDE 已安装
- [ ] ESP32 开发板库已安装：
  ```
  Arduino IDE > 文件 > 首选项 > 附加开发板管理器网址
  添加：https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  工具 > 开发板 > 开发板管理器 > 搜索 ESP32 > 安装
  ```

### 网络检查 ✓

- [ ] 知道 WiFi 名称
- [ ] 知道 WiFi 密码
- [ ] WiFi 频率为 2.4GHz（某些 ESP32 不支持 5GHz）
- [ ] 设备能正常访问互联网

---

## 🔍 实时调试工具

### 工具 1：WiFi 连接诊断

在串口监视器中查看：

```
✅ 成功连接：
Connecting to WiFi....
Connected. IP: 192.168.1.xxx
HTTP server started

❌ 连接失败：
Connecting to WiFi....
Connecting to WiFi....
Connecting to WiFi.... (持续)
```

**修复方法：**
1. 检查 WiFi 名称/密码是否正确
2. 检查 WiFi 是否为 2.4GHz
3. 重启路由器
4. 重新上传程序

---

### 工具 2：网页访问诊断

```cpp
// 在 setup() 后添加此代码以输出完整 URL：

void printAccessInfo() {
  Serial.println("\n========== ACCESS INFO ==========");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Open in browser: http://");
  Serial.println(WiFi.localIP());
  Serial.println("=================================\n");
}
```

在 `setup()` 中调用：
```cpp
printAccessInfo();
```

---

### 工具 3：触摸传感器调试

添加此代码以监控触摸值：

```cpp
// 在 loop() 中添加此段代码

if (millis() % 1000 == 0) {  // 每秒打印一次
  int touchValue = touchRead(4);
  Serial.print("Touch Value: ");
  Serial.println(touchValue);
}
```

**应对值：**
- 手指接近：通常 0-20
- 手指离开：通常 30-100+
- 环境有异：> 100

**调整触摸阈值：**
```cpp
const int touchThreshold = 40;  // 根据实际情况调整
```

---

### 工具 4：PWM 输出诊断

添加此代码以验证 PWM 输出：

```cpp
// Ex07 中的 handleSetPWM 函数修改为：

void handleSetPWM() {
  if (server.hasArg("value")) {
    int value = server.arg("value").toInt();
    brightness = constrain(value, 0, 255);
    ledcWrite(pwmChannel, brightness);
    
    // 调试输出
    Serial.print("PWM Value Set: ");
    Serial.println(brightness);
    
    server.send(200, "text/plain", String(brightness));
  } else {
    server.send(400, "text/plain", "Missing value");
  }
}
```

**预期输出：**
```
PWM Value Set: 0
PWM Value Set: 50
PWM Value Set: 100
PWM Value Set: 200
PWM Value Set: 255
```

---

### 工具 5：报警状态监控（Ex08）

添加此代码以调试状态变化：

```cpp
void handleArm() {
  armed = true;
  alarmState = false;
  Serial.println("System ARMED");
  server.send(200, "text/plain", "ARMED");
}

void handleDisarm() {
  armed = false;
  alarmState = false;
  digitalWrite(ledPin, LOW);
  Serial.println("System DISARMED");
  server.send(200, "text/plain", "DISARMED");
}

// 在 loop() 中检测变化时打印
if (armed && !alarmState && touched && !lastTouch) {
  alarmState = true;
  Serial.println(">>> ALARM TRIGGERED <<<");
}
```

---

## 🧪 逐步测试流程

### 第1步：硬件测试（不需要网页）

创建测试文件 `hardware_test.ino`：

```cpp
// 测试 LED
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(4, INPUT);
}

void loop() {
  // 测试 LED 亮灭
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
  
  // 测试触摸值
  int val = touchRead(4);
  Serial.println(val);
}
```

**验证：**
- [ ] LED 每秒闪烁一次
- [ ] 串口显示触摸值变化

---

### 第2步：WiFi 连接测试

```cpp
#include <WiFi.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 20) {
    delay(500);
    Serial.print(".");
    tries++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Connected");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n✗ WiFi Failed");
  }
}

void loop() {}
```

**验证：**
- [ ] 串口显示 `✓ WiFi Connected`
- [ ] 看到 IP 地址

---

### 第3步：Web 服务器测试

使用完整的 `web_dimmer.ino` 或其他，然后：

1. 上传程序
2. 获取 IP 地址
3. 在命令行测试：

```powershell
# Windows PowerShell

# 测试网页
curl.exe http://192.168.1.100/

# 测试 PWM 设置（Ex07）
curl.exe http://192.168.1.100/setPWM?value=128

# 测试状态获取（Ex08）
curl.exe http://192.168.1.100/status

# 测试传感器（Ex09）
curl.exe http://192.168.1.100/sensor
```

**预期响应：**
- 网页请求：返回 HTML 代码
- PWM 请求：返回 "128"
- 状态请求：返回 "ARMED" 或 "DISARMED"
- 传感器请求：返回数字（如 "45"）

---

## 🆘 故障排除决策树

```
问题发生？
│
├─ ESP32 无反应
│  ├─ USB 连接？ 否 → 检查数据线
│  └─ 是 → 按 Reset 按钮
│
├─ 上传失败
│  ├─ 板型选对？ 否 → 选择 ESP32 Dev Module
│  ├─ 串口选对？ 否 → 找到 ESP32 对应的 COM
│  └─ 都对 → 重新上传
│
├─ WiFi 连接失败
│  ├─ 名密正确？ 否 → 检查代码中的 SSID/Password
│  ├─ 是 2.4GHz？ 否 → 切换到 2.4GHz
│  └─ 都对 → 重启 ESP32 + 路由器
│
├─ 网页无法访问
│  ├─ 同一网络？ 否 → 连接到同一 WiFi
│  ├─ IP 正确？ 否 → 在串口查看正确的 IP
│  └─ 都对 → 检查防火墙设置
│
├─ LED 不亮
│  ├─ 接线正确？ 否 → 检查极性和接线
│  ├─ 代码有错？ 否 → 在 loop() 中直接 digitalWrite(2, HIGH)
│  └─ 都对 → 换 LED 或引脚测试
│
└─ 触摸无反应
   ├─ 值在合理范围？ 否 → 调整 touchThreshold
   ├─ 手指干净？ 否 → 擦干手指
   └─ 都对 → 尝试其他触摸引脚
```

---

## 📊 性能监测

### 检查响应时间

在网页控制台中运行（F12 打开开发者工具）：

```javascript
// 测试网页响应时间
console.time('request');
fetch('/sensor')
  .then(r => r.text())
  .then(d => console.timeEnd('request'))
  .catch(console.error);
```

**预期：** < 100ms

---

### 检查内存使用

在 `setup()` 后添加：

```cpp
void printMemoryInfo() {
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
}
```

**预期：** > 100KB

---

## 💡 优化建议

### 如果网页响应缓慢

```cpp
// 使用 PROGMEM 存储 HTML（已在代码中实现）
const char index_html[] PROGMEM = R"rawliteral(...)";

// 使用 send_P 发送
server.send_P(200, "text/html", index_html);
```

### 如果触摸不稳定

```cpp
// 使用滑动平均
int smoothedValue = 0;
const float alpha = 0.3;  // 平滑因子

void loop() {
  int raw = touchRead(4);
  smoothedValue = smoothedValue * (1 - alpha) + raw * alpha;
  Serial.println(smoothedValue);
}
```

### 如果 LED 闪烁不稳定

```cpp
// 使用 millis() 替代 delay()
unsigned long lastFlash = 0;
const int flashInterval = 100;

void loop() {
  unsigned long now = millis();
  if (now - lastFlash >= flashInterval) {
    digitalWrite(2, !digitalRead(2));
    lastFlash = now;
  }
}
```

---

## 📝 测试报告模板

```
日期：______
实验：Ex07 / Ex08 / Ex09（选择一个）

环境信息：
- WiFi 名称：________________
- WiFi 密码：________________
- 设备型号：________________
- 操作系统：________________

步骤1：硬件连接
结果：成功 □ 失败 □
问题：_____________________

步骤2：程序上传
结果：成功 □ 失败 □
问题：_____________________

步骤3：WiFi 连接
结果：成功 □ 失败 □
IP 地址：________________
问题：_____________________

步骤4：网页访问
结果：成功 □ 失败 □
问题：_____________________

步骤5：功能测试
结果：成功 □ 失败 □
详细描述：_________________

整体评价：🌟🌟🌟🌟🌟
（1-5 星）

改进建议：
_________________________
```

---

## 🎓 学习补充

### 为什么要用 PROGMEM？
- 减少 RAM 占用
- HTML 代码存储在 Flash 中
- 对大型网页很重要

### 为什么 PWM 频率是 5000Hz？
- 人眼无法察觉 5kHz 的闪烁
- 看起来像连续亮度变化
- 常见的 LED 驱动频率

### 为什么触摸值会变化？
- 静电容传感
- 手指接近改变电容
- 值小 = 接近，值大 = 远离

---

**记住：** 遇到问题时，先查看序列监视器的输出！ 🔍

