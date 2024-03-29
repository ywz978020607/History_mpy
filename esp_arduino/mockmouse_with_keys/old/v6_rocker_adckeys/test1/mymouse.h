#ifndef _MYMOUSE_H_
#define _MYMOUSE_H_
 
#include <Arduino.h>
//#include <BleConnectionStatus.h>
//#include <BleCombo.h>
#include "BleComboKeyboard.h"
#include "BleComboMouse.h"
// #include <BleMouse.h>

enum AdcKey : int {
  Dir1_Right= 1,
  Dir1_Ok   = 2,
  Dir1_Up   = 3,
  Dir1_Left = 4,
  Dir1_Down = 5,

  Dir2_Right= 6,
  Dir2_Ok   = 7,
  Dir2_Up   = 8,
  Dir2_Left = 9,
  Dir2_Down = 10,
};

class Mymouse {
  public:
    // Mymouse();
    // ~Mymouse();
    bool isConnected(){
        return bleKeyboard.isConnected();
    }
    void set_up();
    void self_main(); // 主函数

    int get_bias(int pin_val){
      int avg_val = 0;
      int avg_num = 5;
      for(int ii=0;ii<avg_num;ii++){
        avg_val += analogRead(pin_val);
        delay(50);
      }
      avg_val = avg_val/avg_num;
      return ((1<<(adc_bit - 1)) - avg_val);
    }
    int get_ADC(int pin_val, int temp_add_bias = 0){
      return min(analogRead(pin_val) + temp_add_bias, 1<< adc_bit);
    }
    int change_speed(unsigned int val, bool oppo = false){
      val += ( 1 << (adc_bit - rank_num - 1) ); //输出挡位区间的一半 [0, 2*rank_num] -> [-xx,xx]
      val = val >> (adc_bit - rank_num + 1);  // 缩放到只关注2*rank_num挡位
      return oppo?(rank_num - val):(val - rank_num);
    }

    void move_point_right_down(int val1, int val2){
        if(val1 == 0 && val2 == 0){
          return;
        }
        for(unsigned char ii=0;ii<10;ii++){
          bleMouse.move(val1*2, val2*2); //数值可改变速度
          delay(10); // 修改delay同样可以改变速度 - 作为效果调整使用
        }
      }

    // void move_scroll_up_right(int val1, int val2){
    //     if(val1 == 0 && val2 == 0){
    //         return;
    //       }
    //     for(unsigned char ii=0;ii<2;ii++){
    //       bleMouse.move(0,0, val1, val2); //数值可改变速度
    //       delay(50); // 修改delay同样可以改变速度 - 作为效果调整使用
    //     }
    //   }
    
    int get_ADC_keys(int pin_val){
      return (analogRead(pin_val) + (adc_key_internal>>1) ) / adc_key_internal;
    }

  public:
    // ADC1: 32, 33, 34, ...
    // 若ADC1用完了，只能用ADC2-安全范围12~14,25~27
    // 轮询代替中断 34-39只能输入且不能上下拉
    int adc_key = 32; //两组5向开关

    int key_left_up   = 17;
    int key_left_down = 26;
    int key_right_up  = 16;
    int key_right_down= 27;
    
    int s_key_1 = 19;

    int pointer_key = 18; //摇杆自带
    int pointer_x = 34;
    int pointer_y = 35;
    
    int adc_battery = 33;
    int signal_out = 5;
    
    int pullup_input[5] = {key_left_up, key_left_down, key_right_up, key_right_down, s_key_1};
    int pulldown_input[2] = {pointer_key, adc_key};


    //
    BleComboKeyboard  bleKeyboard;
    BleComboMouse bleMouse = BleComboMouse(&bleKeyboard);

    int temp_val_1 = 0;
    int temp_val_2 = 0;
    int adc_bit = 12;
    int rank_num = 4; //4挡 [0,8] 9个值 -- 由于默认向下取整，故加上2^(adc_bit-rank_num-1); // 2^(adc_bit - rank_num)为1 故加上一半的bias用来取整
    int adc_bias_pointer= 144; // 补偿-调试时中间位置与中间值的采样差 -- 摇杆模块采用3.3V供电  自动校正
    bool mode = true; // true: scroll false: direction

    //adc_key - 采用数值运算
    int adc_key_num = 10; // 4096 / adc_key_num 为每个挡位的中心值，output = (adc+Δ/2)//Δ, Δ=(1<<adc_bit) // adc_key_num
    int adc_key_internal = (1 << adc_bit) / adc_key_num; //int adc_key_internal_half = adc_key_internal >> 1;

    float speed_factor = 0.5; // from 0.5 to 1.0 渐变速
    long int do_nothing_cnt = 0; //do nothing cnt
    bool has_action = false;
};

#endif
