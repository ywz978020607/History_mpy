#ifndef _MYMOUSE_H_
#define _MYMOUSE_H_
 
#include <Arduino.h>
//#include <BleConnectionStatus.h>
//#include <BleCombo.h>
#include "BleComboKeyboard.h"
#include "BleComboMouse.h"
// #include <BleMouse.h>


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

  public:
    // ADC1: 32, 33, 34, ...
    // 若ADC1用完了，只能用ADC2-安全范围12~14,25~27
    // 轮询代替中断 34-39只能输入且不能上下拉
    int dir_key = 21;
    int dir_up = 25;
    int dir_down = 4;
    int dir_left = 22;
    int dir_right= 23;

    int key_left_up   = 17;
    int key_left_down = 26;
    
    int s_key_1 = 19;

    int pointer_key = 18; //摇杆自带
    int pointer_x = 34;
    int pointer_y = 35;
    
    int adc_battery = 33;
    int signal_out = 5;
    
    int pullup_input[8] = {key_left_up, key_left_down, s_key_1, dir_key, dir_up, dir_down, dir_left, dir_right};
    int pulldown_input[1] = {pointer_key};


    //
    BleComboKeyboard  bleKeyboard;
    BleComboMouse bleMouse = BleComboMouse(&bleKeyboard);

    int temp_val_1 = 0;
    int temp_val_2 = 0;
    int adc_bit = 12;
    int rank_num = 4; //4挡 [0,8] 9个值 -- 由于默认向下取整，故加上2^(adc_bit-rank_num-1); // 2^(adc_bit - rank_num)为1 故加上一半的bias用来取整
    int adc_bias_pointer= 144; // 补偿-调试时中间位置与中间值的采样差 -- 摇杆模块采用3.3V供电  自动校正
    bool mode = true; // true: scroll false: direction

    float speed_factor = 0.5; // from 0.5 to 1.0 渐变速
    long int do_nothing_cnt = 0; //do nothing cnt
    bool has_action = false;
};

#endif
