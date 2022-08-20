# 增加轨迹球模块



```cpp
// http://www.yfrobot.com.cn/wiki/index.php?title=%E8%BD%A8%E8%BF%B9%E7%90%83%E6%A8%A1%E5%9D%97

/**************************************************************************
BlackBerry Trackballer Breakout Demo
***************************************************************************/
//Define Trackballer Breakout pin connections to Arduino
#define Btn 2
#define Lft 3
#define Rht 4
#define Up 5
#define Dwn 6
 
//Define variables used in sketch
int buttonClick;
unsigned long mouse_Lft;
unsigned long old_mouse_Lft;
unsigned long mouse_Rht;
unsigned long old_mouse_Rht;
unsigned long mouse_Up;
unsigned long old_mouse_Up;
unsigned long mouse_Dwn;
unsigned long old_mouse_Dwn;
int x_position;
int y_position;
 
/*********************Setup Loop*************************/
void setup() {
 
  //Define pin functionality on the Arduino
  pinMode(Btn, INPUT);
  pinMode(Lft, INPUT);
  pinMode(Rht, INPUT);
  pinMode(Up, INPUT);
  pinMode(Dwn, INPUT);
 
  //Start Serial port for debugging.
  Serial.begin(9600);
  Serial.println("Begin Trackballer Demo");
  delay(1000);
  Serial.println("Begin Trackball tracking");
}
 
 
/*********************Main Loop*************************/
void loop() {
  //read the pin state
  mouse_Lft = digitalRead(Lft);
  mouse_Rht = digitalRead(Rht);
  if (mouse_Lft != old_mouse_Lft)
  {
    x_position = --x_position;
    old_mouse_Lft = mouse_Lft;
    Serial.print("Trackball Position: \t X-Position= ");
    Serial.println(x_position);
  }
  if (mouse_Rht != old_mouse_Rht)
  {
    x_position = ++x_position;
    old_mouse_Rht = mouse_Rht;
    Serial.print("Trackball Position: \t X-Position= ");
    Serial.println(x_position);
  }
  delay(50);
  //read the pin state
  mouse_Up = digitalRead(Up);
  mouse_Dwn = digitalRead(Dwn);
  if (mouse_Up != old_mouse_Up)
  {
    y_position = ++y_position;
    old_mouse_Up = mouse_Up;
    Serial.print("Trackball Position: \t  \t Y-position= ");
    Serial.println(y_position);
  }
  if (mouse_Dwn != old_mouse_Dwn)
  {
    y_position = --y_position;
    old_mouse_Dwn = mouse_Dwn;
    Serial.print("Trackball Position: \t  \t Y-position= ");
    Serial.println(y_position);
  }
  delay(50);
 
  //Check for button click. If present, print to Serial monitor.
  buttonClick = digitalRead(Btn);
  if (buttonClick == LOW)
  {
    Serial.println("Click");
  }
}
```



