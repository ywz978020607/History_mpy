#include "mymouse.h"

void Mymouse::set_up(){
    // init adc
    //set the resolution to 12 bits (0-4096)
    analogReadResolution(adc_bit);
    // set bias
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
    temp_val = get_ADC(point_x, adc_bias_point);
//    Serial.println("point_x");
//    Serial.println(temp_val, DEC);
//    Serial.println(change_speed(temp_val), DEC);
    if(temp_val != 0)
    move_point_right(change_speed(temp_val, true));

    temp_val = get_ADC(point_y, adc_bias_point);
    if(temp_val != 0)
    move_point_down(change_speed(temp_val, true));

    temp_val = get_ADC(scroll_x, adc_bias_scroll);
    if(temp_val != 0)
    move_scroll_right(change_speed(temp_val, true));

    temp_val = get_ADC(scroll_y, adc_bias_scroll);
//    Serial.println("scroll_y");
//    Serial.println(temp_val, DEC);
    if(temp_val != 0)
    move_scroll_up(change_speed(temp_val));
    // 
}