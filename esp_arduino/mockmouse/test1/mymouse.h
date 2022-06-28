

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
    int get_ADC(int pin_val, int temp_add_bias = 0){
      return min(analogRead(pin_val) + temp_add_bias, 1<< adc_bit);
    }
    int change_speed(unsigned int val, bool oppo = false){
      val += ( 1 << (adc_bit - rank_num - 1) ); //输出挡位区间的一半 [0, 2*rank_num] -> [-xx,xx]
      val = val >> (adc_bit - rank_num + 1);  // 缩放到只关注2*rank_num挡位
      return oppo?(rank_num - val):(val - rank_num);
    }

    void move_point_down(int val){
      if(val == 0){
        return;
      }
      for(unsigned char ii=0;ii<15;ii++){
        bleMouse.move(0,val*2);  //数值可改变速度
        delay(1); // 修改delay同样可以改变速度 - 作为效果调整使用
      }
    }
    void move_point_right(int val){
      if(val == 0){
        return;
      }
      for(unsigned char ii=0;ii<15;ii++){
        bleMouse.move(val*2, 0); //数值可改变速度
        delay(1); // 修改delay同样可以改变速度 - 作为效果调整使用
      }
    }
    void move_scroll_up(int val){
      if(val == 0){
        return;
      }
      for(unsigned char ii=0;ii<2;ii++){
        bleMouse.move(0,0, val); //数值可改变速度
        delay(50); // 修改delay同样可以改变速度 - 作为效果调整使用
      }
    }
    void move_scroll_right(int val){
      if(val == 0){
        return;
      }
      for(unsigned char ii=0;ii<2;ii++){
        bleMouse.move(0,0,0, val); //数值可改变速度
        delay(50); // 修改delay同样可以改变速度 - 作为效果调整使用
      }
    }

    void press_key(uint8_t mode){
      // 支持 MOUSE_LEFT MOUSE_RIGHT MOUSE_MIDDLE MOUSE_BACK MOUSE_FORWARD 以及 MOUSE_LEFT | MOUSE_RIGHT 可多元复合操作
      bleMouse.click(mode);
      delay(500);
    }

  public:
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
//    int shift_threshold = 100; //防漂移阈值
    int adc_bit = 12;
    int rank_num = 4; //4挡 [0,8] 9个值 -- 由于默认向下取整，故加上2^(adc_bit-rank_num-1); // 2^(adc_bit - rank_num)为1 故加上一半的bias用来取整
    int adc_bias_point = 144; // 补偿-调试时中间位置与中间值的采样差 -- 摇杆模块采用3.3V供电
    int adc_bias_scroll = 200; // 补偿-调试时中间位置与中间值的采样差 -- 摇杆模块采用3.3V供电
};

#endif
