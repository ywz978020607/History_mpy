#include "mymouse.h"
/**
 * This example turns the ESP32 into a Bluetooth LE mouse that scrolls down every 2 seconds.
 */
//BleMouse bleMouse;
Mymouse mymouse;

void x_inc(){
  mymouse.x_move++;
}
void x_dec(){
  mymouse.x_move--;
}
void y_inc(){
  mymouse.y_move++;
}
void y_dec(){
  mymouse.y_move--;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  // init tracker interrupt
  attachInterrupt(digitalPinToInterrupt(mymouse.left_pin), x_inc, FALLING);
  attachInterrupt(digitalPinToInterrupt(mymouse.right_pin), x_dec, FALLING);
  attachInterrupt(digitalPinToInterrupt(mymouse.up_pin), y_inc, FALLING);
  attachInterrupt(digitalPinToInterrupt(mymouse.down_pin), y_dec, FALLING);

  mymouse.set_up();
}

void loop() {
  if(mymouse.isConnected()) {//
    mymouse.self_main();
//    Serial.println("running");
//    delay(100);
  }
}
