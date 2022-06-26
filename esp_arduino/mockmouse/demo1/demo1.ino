/**
 * This example turns the ESP32 into a Bluetooth LE mouse that scrolls down every 2 seconds.
 */
#include <BleMouse.h>
BleMouse bleMouse("ESP32蓝牙鼠标","Espressif",100); //其中“ESP32蓝牙键盘”为键盘名称；"Espressif"为制造商
//BleMouse bleMouse;
bool flag_once = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleMouse.begin();
}

void loop() {
  if(bleMouse.isConnected()) {

    unsigned long startTime;
    startTime = millis();

    if(!flag_once){
      Serial.println("下移鼠标指针");
      startTime = millis();
      while(millis()<startTime+4000) {
        bleMouse.move(0,1);
        delay(20);
      }
      delay(500);
      flag_once = true;
    }
    //////
    Serial.println("向下滚动");
    startTime = millis();
    while(millis()<startTime+2000) {
      bleMouse.move(0,0,-1);
      delay(100);
    }
    delay(500);
  }
  delay(2000);
}
