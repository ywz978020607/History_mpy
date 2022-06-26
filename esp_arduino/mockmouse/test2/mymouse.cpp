#include "mymouse.h"

void Mymouse::set_up(){
    // Serial.printf("mymouse init\n");
    // init ble
    bleMouse.begin();

    // init adc
    //set the resolution to 12 bits (0-4096)
    analogReadResolution(adc_bit);

    // init gpio when not interrupt
//    pinMode(left_key, INPUT_PULLDOWN); // INPUT
//    pinMode(right_key,INPUT_PULLDOWN);
}

void Mymouse::self_main(){
    // KEY 中断类
     if(left_key.checkPressed()){
     press_key(MOUSE_LEFT);
     }
     if(right_key.checkPressed()){
     press_key(MOUSE_RIGHT);
     }
//    // KEY非中断
//    if(digitalRead(left_key) == HIGH){
//        press_key(MOUSE_LEFT);
//    }
//    if(digitalRead(right_key) == HIGH){
//        press_key(MOUSE_RIGHT);
//    }

    // 摇杆
    temp_val = get_ADC(point_x);
    if(temp_val != 0)
    move_point_down(change_speed(temp_val));

    temp_val = get_ADC(point_y);
    if(temp_val != 0)
    move_point_right(change_speed(temp_val));

    temp_val = get_ADC(scroll_x);
    if(temp_val != 0)
    move_scroll_up(change_speed(temp_val, true));

    temp_val = get_ADC(scroll_y);
    if(temp_val != 0)
    move_scroll_right(change_speed(temp_val));
    // 
}
