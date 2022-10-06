https://github.com/ServAlex/ESP32-BLE-Combo
https://github.com/T-vK/ESP32-BLE-Keyboard


淘宝材料清单
|材料|价格|备注|
| ----------- | ----------- | ----------- |
| [电池管理模块](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1e7a2e8d2TwcO6&id=649008596358&_u=628qvnn8f5f3) |2.8 + 2.5|黑色3.7v锂电池带5v输出款|
|[电池500mA](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1e7a2e8d2TwcO6&id=613909574730&_u=628qvnn87a78)|10.5|西瓜红[602040-500毫安]|
|[拨动开关三档20只](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1e7a2e8d3jlPVx&id=13302149297&_u=628qvnn8fc2a)|2.3|拨动开关 SS-12D00 1P2T 立式&nbsp：柄高5MM (20个)|
|[单片机](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1e7a2e8d2TwcO6&id=580133936548&_u=628qvnn84c9e)| 18+3|芯片CH9102 新款|
|[贴片按键40只](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1e7a2e8dudo0K0&id=608931747899&_u=628qvnn80b52)|2.62|散装/3*6*5/贴片2脚 (40只)|
|[侧按键直插20只](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1e7a2e8dudo0K0&id=35609447792&_u=628qvnn8e77a)|2.71|3*6*5侧插轻触按键开关 边二脚侧按(20个）|
|[NS摇杆](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1e7a2e8d3jlPVx&id=640317236517&_u=628qvnn8eef4)|3.5+10||
|[五向拨杆-可选](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.1e7a2e8d3jlPVx&id=595134302480&_u=628qvnn88140)|0.78|10*10*9MM|



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



