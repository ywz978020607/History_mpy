#ifndef _MYMOUSE_H_
#define _MYMOUSE_H_

#include <Arduino.h>
//#include <BleConnectionStatus.h>
//#include <BleCombo.h>
#include "BleComboKeyboard.h"
#include "BleComboMouse.h"
// #include <BleMouse.h>

// tracker
#define bounce_interval       30
#define base_move_pixels      2
#define exponential_bound     20
#define exponential_base      1.2


class Direction {
public:
  Direction(int pin1, int pin2) {
    this->pins[0] = pin1;
    this->pins[1] = pin2;
    pinMode(this->pins[0], INPUT);
    pinMode(this->pins[1], INPUT);
  };
  int read_action() {
    for(int i = 0; i < 2; ++i) {
      this->current_actions[i] = digitalRead(this->pins[i]);
      this->current_action_times[i] = millis();
      if(this->current_actions[i] != this->last_actions[i]) {
        this->last_actions[i] = this->current_actions[i];
        exponential = (exponential_bound - (this->current_action_times[i] - this->last_action_times[i]));
        exponential = (exponential > 0) ? exponential : 1;
        move_multiply = exponential_base;
        for(int i = 0; i < exponential; ++i) {
          move_multiply *= exponential_base;
        }
        this->last_action_times[i] = this->current_action_times[i];
        if(i == 0) {
          return (-1) * base_move_pixels * move_multiply;
        } else {
          return base_move_pixels * move_multiply;
        }
      }
    }
    return 0;
  };
private:
  int pins[2];
  int current_actions[2];
  int last_actions[2];
  int  exponential;
  double move_multiply;
  unsigned long current_action_times[2];
  unsigned long last_action_times[2];
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

    void move_scroll_up_right(int val1, int val2){
        if(val1 == 0 && val2 == 0){
            return;
          }
        for(unsigned char ii=0;ii<2;ii++){
          bleMouse.move(0,0, val1, val2); //数值可改变速度
          delay(50); // 修改delay同样可以改变速度 - 作为效果调整使用
        }
      }

  public:
    // 轮询代替中断 34-39只能输入且不能上下拉
    int rocker_key = 26; //摇杆自带
    int rocker_x = 32;
    int rocker_y = 33;

    int pointer_key = 18; //摇杆自带
    int pointer_x = 34;
    int pointer_y = 35;

    int s_key_1 = 23;
    
    int adc_battery = 27; // 采集ADC电压，由于ADC1用完了，只能用ADC2-安全范围12~14,25~27

    BleComboKeyboard  bleKeyboard;
    BleComboMouse bleMouse = BleComboMouse(&bleKeyboard);

    int temp_val_1 = 0;
    int temp_val_2 = 0;
    int adc_bit = 12;
    int rank_num = 4; //4挡 [0,8] 9个值 -- 由于默认向下取整，故加上2^(adc_bit-rank_num-1); // 2^(adc_bit - rank_num)为1 故加上一半的bias用来取整
    int adc_bias_rocker = 144; // 补偿-调试时中间位置与中间值的采样差 -- 摇杆模块采用3.3V供电  自动校正
    int adc_bias_pointer= 144; // 补偿-调试时中间位置与中间值的采样差 -- 摇杆模块采用3.3V供电  自动校正
    bool mode = true; // true: scroll false: direction


    // tracker
    int white_led_pin = 4;
    int btn_pin               =5;
    int right_pin             =16;
    int left_pin              =17;
    int down_pin              =18;
    int up_pin                =19;
    // button and debounce
    int btn_state;
    int btn_read_state;
    unsigned long btn_current_action_time;
    unsigned long btn_last_action_time;

    // mouse move
    volatile int x_move, y_move;
    Direction x_direction = Direction(left_pin, right_pin);
    Direction y_direction = Direction(up_pin, down_pin);
    
    // unsigned long global_timer = millis();
    // unsigned long global_last_timer = millis();
};

#endif
