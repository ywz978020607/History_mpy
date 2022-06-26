https://github.com/T-vK/ESP32-BLE-Mouse
https://www.codenong.com/cs106212159/

arduino-项目-添加zip库

ESP32鼠标模拟库，依赖于ESP32_BLE_Arduino这个库，地址：https://github.com/T-vK/ESP32-BLE-Mouse

# 示例程序
## 模拟鼠标按键测试
请注意：建议打开几个网页，在网页中测试！网页在前进/后退，右键菜单......
```
/**
 * 时间：2020/5/18
 * 作者：刘泽文
 * 功能：使用ESP32的蓝牙功能模拟蓝牙鼠标-按键操作
 */
#include <BleMouse.h>
#include <WiFi.h>

BleMouse bleMouse("ESP32蓝牙鼠标","Espressif",100);//其中“ESP32蓝牙键盘”为键盘名称；"Espressif"为制造商

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleMouse.begin();
}

void loop() {
  if(bleMouse.isConnected()) {
    Serial.println("左键点击");
    bleMouse.click(MOUSE_LEFT);
    delay(500);

    Serial.println("右键单击");
    bleMouse.click(MOUSE_RIGHT);
    delay(500);

    Serial.println("滚轮点击");
    bleMouse.click(MOUSE_MIDDLE);
    delay(500);

    Serial.println("后退按钮点击");
    bleMouse.click(MOUSE_BACK);
    delay(500);

    Serial.println("前进按钮点击");
    bleMouse.click(MOUSE_FORWARD);
    delay(500);

    Serial.println("同时单击鼠标左键和鼠标右键");
    bleMouse.click(MOUSE_LEFT | MOUSE_RIGHT);
    delay(500);

    Serial.println("同时单击鼠标左键+鼠标右键和滚轮");
    bleMouse.click(MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE);
    delay(500);

  }
}
```

## 模拟鼠标移动/移动 - 调整值/delay都可以改变速度
测试代码时，请将鼠标光标放在你的代码上！你将会看到你的代码在上下翻滚，鼠标光标在画正方形。
```
/**
 * 时间：2020/5/18
 * 作者：刘泽文
 * 功能：使用ESP32的蓝牙功能模拟蓝牙鼠标-鼠标-滚轮
 */
#include <BleMouse.h>
#include <WiFi.h>

BleMouse bleMouse("ESP32蓝牙鼠标","Espressif",100);//其中“ESP32蓝牙键盘”为键盘名称；"Espressif"为制造商

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleMouse.begin();
}

void loop() {
  if(bleMouse.isConnected()) {

    unsigned long startTime;

    Serial.println("向上滚动");
    startTime = millis();
    while(millis()<startTime+2000) {
      bleMouse.move(0,0,1);
      delay(100);
    }
    delay(500);

    Serial.println("向下滚动");
    startTime = millis();
    while(millis()<startTime+2000) {
      bleMouse.move(0,0,-1);
      delay(100);
    }
    delay(500);

    Serial.println("向左滚动");
    startTime = millis();
    while(millis()<startTime+2000) {
      bleMouse.move(0,0,0,-1);
      delay(100);
    }
    delay(500);

    Serial.println("向右滚动");
    startTime = millis();
    while(millis()<startTime+2000) {
      bleMouse.move(0,0,0,1);
      delay(100);
    }
    delay(500);

    Serial.println("向上移动鼠标指针");
    startTime = millis();
    while(millis()<startTime+2000) {
      bleMouse.move(0,-1);
      delay(20);
    }
    delay(500);

    Serial.println("下移鼠标指针");
    startTime = millis();
    while(millis()<startTime+2000) {
      bleMouse.move(0,1);
      delay(20);
    }
    delay(500);

    Serial.println("左移鼠标指针");
    startTime = millis();
    while(millis()<startTime+2000) {
      bleMouse.move(-1,0);
      delay(20);
    }
    delay(500);

    Serial.println("向右移动鼠标指针");
    startTime = millis();
    while(millis()<startTime+2000) {
      bleMouse.move(1,0);
      delay(20);
    }
    delay(500);

  }
}
```

