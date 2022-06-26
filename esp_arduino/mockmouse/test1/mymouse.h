

#ifndef _MYMOUSE_H_
#define _MYMOUSE_H_
 
#include <Arduino.h>
#include <BleConnectionStatus.h>
#include <BleMouse.h>
// #include "Button.h"

class Mymouse {
  public:
    // Mymouse();
    // ~Mymouse();
    bool isConnected(){
        return bleMouse.isConnected();
    }
    void set_up();
    void self_main(); // 主函数

    // 
    int get_ADC(int pin_val){
      int val = analogRead(pin_val);
        return val > shift_threshold?val:0; //analogReadMilliVolts()
    }
    int change_speed(int val, bool oppo = false){
      // +/-
      val -= (1 << (adc_bit - 1)); // [-2048, 2047]
      val = (val>0)?val++:val; //凑齐可以除到相同数字
      // adc val->scroll/move speed
      return oppo?(val>>(adc_bit - 3)):-(val>>(adc_bit - 3)); //12bit-> max=4
    }

    void move_point_down(int val){
      if(val == 0){
        return;
      }
      bleMouse.move(0,val);  //数值可改变速度
      delay(100); // 修改delay同样可以改变速度 - 作为效果调整使用
    }
    void move_point_right(int val){
      if(val == 0){
        return;
      }
      bleMouse.move(val, 0); //数值可改变速度
      delay(100); // 修改delay同样可以改变速度 - 作为效果调整使用
    }
    void move_scroll_up(int val){
      if(val == 0){
        return;
      }
      bleMouse.move(0,0, val); //数值可改变速度
      delay(100); // 修改delay同样可以改变速度 - 作为效果调整使用
    }
    void move_scroll_right(int val){
      if(val == 0){
        return;
      }
      bleMouse.move(0,0,0, val); //数值可改变速度
      delay(100); // 修改delay同样可以改变速度 - 作为效果调整使用
    }

    void press_key(uint8_t mode){
      // 支持 MOUSE_LEFT MOUSE_RIGHT MOUSE_MIDDLE MOUSE_BACK MOUSE_FORWARD 以及 MOUSE_LEFT | MOUSE_RIGHT 可多元复合操作
      bleMouse.click(mode);
      delay(500);
    }

  private:
    // TODO 中断
    // Button left_key = Button(22);
    // Button right_key = Button(23);
    // 轮询代替中断
    int left_key = 22;
    int right_key = 23;

    int point_x = 32;
    int point_y = 33;
    
    int scroll_x = 34;
    int scroll_y = 35;
    BleMouse bleMouse;
    //BleMouse bleMouse;

    int temp_val = 0;
    int shift_threshold = 100; //防漂移阈值
    int adc_bit = 12;
};

#endif
