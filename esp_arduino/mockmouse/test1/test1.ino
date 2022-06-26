#include "mymouse.h"
/**
 * This example turns the ESP32 into a Bluetooth LE mouse that scrolls down every 2 seconds.
 */
//BleMouse bleMouse;
Mymouse mymouse;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  mymouse.set_up();
}

void loop() {
  if(mymouse.isConnected()) {
    Serial.println("running");
    delay(100);

    mymouse.self_main();
  }
}
