#include "mymouse.h"

void Mymouse::set_up(){
    // init adc
    //set the resolution to 12 bits (0-4096)
    analogReadResolution(adc_bit);
    // set bias
   Serial.println((get_bias(point_x) + get_bias(point_y))/2, DEC);
   Serial.println((get_bias(scroll_x) + get_bias(scroll_y))/2, DEC);
    adc_bias_point = (get_bias(point_x) + get_bias(point_y))/2;
    adc_bias_scroll = (get_bias(scroll_x) + get_bias(scroll_y))/2;
    // init gpio when not interrupt
    pinMode(left_key, INPUT_PULLUP); // INPUT
    pinMode(right_key,INPUT_PULLUP);

    // Serial.printf("mymouse init\n");
    // init ble
    bleMouse.begin();
}

void Mymouse::self_main(){
    // KEY 中断类
    // if(left_key.checkPressed()){
    // press_key(MOUSE_LEFT);
    // }
    // if(right_key.checkPressed()){
    // press_key(MOUSE_RIGHT);
    // }
    // KEY非中断
    if(digitalRead(left_key) == LOW){
        press_key(MOUSE_LEFT);
    }
    if(digitalRead(right_key) == LOW){
        press_key(MOUSE_RIGHT);
    }

    // 摇杆
    temp_val_1 = get_ADC(point_x, adc_bias_point);
    temp_val_1 = change_speed(temp_val_1, true);
    temp_val_2 = get_ADC(point_y, adc_bias_point);
    temp_val_2 = change_speed(temp_val_2, true);
    
    if(temp_val_1 != 0 || temp_val_2 != 0){
        move_point_right_down(temp_val_1, temp_val_2);
      }

    // 滚轮
    temp_val_1 = get_ADC(scroll_x, adc_bias_scroll);
    temp_val_1 = change_speed(temp_val_1, true);
    temp_val_2 = get_ADC(scroll_y, adc_bias_scroll);
    temp_val_2 = change_speed(temp_val_2);
    if(temp_val_1 != 0 || temp_val_2 != 0){
//      move_scroll_right(temp_val_1);
//      move_scroll_up(temp_val_2);
        move_scroll_up_right(temp_val_2, temp_val_1);
    }
}
