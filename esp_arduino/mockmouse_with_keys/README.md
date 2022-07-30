https://github.com/ServAlex/ESP32-BLE-Combo
https://github.com/T-vK/ESP32-BLE-Keyboard


二合一
```
#include <BleCombo.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Starting work!");
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  if(Keyboard.isConnected()) {
    Serial.println("Sending 'Hello world'");
    Keyboard.println("Hello World");

    Serial.println("Sending Enter key...");
    Keyboard.write(KEY_RETURN);

    Serial.println("Sending Play/Pause media key...");
    Keyboard.write(KEY_MEDIA_PLAY_PAUSE);

    Serial.println("Sending Ctrl+Alt+Delete...");
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_DELETE);
    delay(100);
    Keyboard.releaseAll();

    Serial.println("Move mouse pointer up");
    Mouse.move(0,-10);
    
    Serial.println("Scroll Down");
    Mouse.move(0,0,-1);

    Serial.println("Left click");
    Mouse.click(MOUSE_LEFT);
  }
  
  Serial.println("Waiting 2 seconds...");
  delay(2000);
}

```

纯键盘
```
/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending 'Hello world'...");
    bleKeyboard.print("Hello world");

    delay(1000);

    Serial.println("Sending Enter key...");
    bleKeyboard.write(KEY_RETURN);

    delay(1000);

    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);

    delay(1000);
    
   //
   // Below is an example of pressing multiple keyboard modifiers 
   // which by default is commented out. 
   // 
   /* Serial.println("Sending Ctrl+Alt+Delete...");
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(KEY_LEFT_ALT);
    bleKeyboard.press(KEY_DELETE);
    delay(100);
    bleKeyboard.releaseAll();
    */

  }
  Serial.println("Waiting 5 seconds...");
  delay(5000);
}
```



