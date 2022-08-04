#include "mymouse.h"

#define direct_get_val_1 change_speed(get_ADC(rocker_x, adc_bias_rocker), false)
#define direct_get_val_2 change_speed(get_ADC(rocker_y, adc_bias_rocker), false)


void Mymouse::set_up(){
    // init adc
    //set the resolution to 12 bits (0-4096)
    analogReadResolution(adc_bit);
    // set bias
    Serial.println((get_bias(rocker_x) + get_bias(rocker_y))/2, DEC);
    adc_bias_rocker = (get_bias(rocker_x) + get_bias(rocker_y))/2;
    adc_bias_pointer = (get_bias(pointer_x) + get_bias(pointer_y))/2;
    // init gpio when not interrupt
    pinMode(s_key_1, INPUT_PULLUP); // INPUT
    pinMode(rocker_key,INPUT_PULLDOWN); // 34-39 can only be input and can not PULL UP/DOWN.
    pinMode(pointer_key,INPUT_PULLDOWN); // 34-39 can only be input and can not PULL UP/DOWN.

    // init ble
    bleKeyboard.begin();
    bleMouse.begin();
}

void Mymouse::self_main(){
    // 独立Key
    if(digitalRead(s_key_1) == LOW){
        delay(50); // 消抖
        bleMouse.press(MOUSE_LEFT);
        while(digitalRead(s_key_1) == LOW);
        bleMouse.release(MOUSE_LEFT);
        delay(50);
    }

    // pointer
    if(digitalRead(pointer_key) == HIGH){
        delay(50); // 消抖
        bleMouse.press(MOUSE_LEFT);
        while(digitalRead(pointer_key) == HIGH);
        bleMouse.release(MOUSE_LEFT);
        delay(50);
    }
    temp_val_1 = get_ADC(pointer_x, adc_bias_pointer);
    temp_val_2 = get_ADC(pointer_y, adc_bias_pointer);
    temp_val_1 = change_speed(temp_val_1);
    temp_val_2 = change_speed(temp_val_2);
    if(temp_val_1 != 0 || temp_val_2 != 0){
        move_point_right_down(temp_val_1, temp_val_2);
    }

    // rocker -- direction or scroll
    if(digitalRead(rocker_key) == HIGH){ // 注意要硬件/软件上拉
        // 摇杆键多功能： 模式切换&普通右键
        delay(150); // 消抖
        delay(400);
        if(digitalRead(rocker_key) == LOW){
            mode = !mode;
        }
        else{
            bleMouse.click(MOUSE_RIGHT);
        }
        while(digitalRead(rocker_key) == HIGH);
        delay(150);
    }

    // 方向键 or 滚轮 -- scroll rocker
    temp_val_1 = get_ADC(rocker_x, adc_bias_rocker);
    temp_val_2 = get_ADC(rocker_y, adc_bias_rocker);
    temp_val_1 = change_speed(temp_val_1);
    temp_val_2 = change_speed(temp_val_2);
    if(mode){
        // 滚轮
        if(temp_val_1 != 0 || temp_val_2 != 0){
            move_scroll_up_right(temp_val_2, temp_val_1);
        }
    }
    else{
        // 方向键
//        Serial.println(temp_val_1, DEC);
        if(temp_val_2 >= rank_num-1){
            bleKeyboard.press(KEY_UP_ARROW);
            while(direct_get_val_2 >= rank_num-1){};
            bleKeyboard.releaseAll();
        }
        else if(temp_val_2 <= -rank_num+1){
            bleKeyboard.press(KEY_DOWN_ARROW);
            while(direct_get_val_2 <= -rank_num+1);
            bleKeyboard.releaseAll();
        }

        else if(temp_val_1 >= rank_num-1){
            bleKeyboard.press(KEY_LEFT_ARROW);
            while(direct_get_val_1 >= rank_num-1);
            bleKeyboard.releaseAll();
        }
        else if(temp_val_1 <= -rank_num+1){
            bleKeyboard.press(KEY_RIGHT_ARROW);
            while(direct_get_val_1 <= -rank_num+1);
            // bleKeyboard.release(KEY_RIGHT_ARROW);
            bleKeyboard.releaseAll();
        }
    }
}
