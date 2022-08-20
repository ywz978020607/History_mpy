#include "mymouse.h"

void Mymouse::set_up(){
    // init adc
    //set the resolution to 12 bits (0-4096)
    analogReadResolution(adc_bit);
    // set bias
    Serial.println((get_bias(rocker_x) + get_bias(rocker_y))/2, DEC);
    adc_bias_rocker = (get_bias(rocker_x) + get_bias(rocker_y))/2;
    // init gpio when not interrupt
    pinMode(s_key_1, INPUT_PULLUP); // INPUT
    pinMode(rocker_key,INPUT_PULLDOWN); // 34-39 can only be input and can not PULL UP/DOWN.

    // Serial.printf("mymouse init\n");
    // init ble
    bleMouse.begin();
}

void Mymouse::self_main(){
    // KEY非中断
    if(digitalRead(s_key_1) == LOW){
        delay(50); // 消抖
        bleMouse.press(MOUSE_LEFT);
        //消抖
        while(digitalRead(s_key_1) == LOW);
        bleMouse.release(MOUSE_LEFT);
        delay(50);
    }
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

    // 摇杆 or 滚轮
    temp_val_1 = get_ADC(rocker_x, adc_bias_rocker);
    temp_val_2 = get_ADC(rocker_y, adc_bias_rocker);
//    Serial.println("temp_val:");
//    Serial.println(temp_val_1, DEC);
//    Serial.println(temp_val_2, DEC);
    if(mode){
        // 滚轮
        temp_val_1 = change_speed(temp_val_1, true);
        temp_val_2 = change_speed(temp_val_2);
        if(temp_val_1 != 0 || temp_val_2 != 0){
        //  move_scroll_right(temp_val_1);
        //  move_scroll_up(temp_val_2);
            move_scroll_up_right(temp_val_1, temp_val_2);
        }
    }
    else{
        // 指针
        temp_val_1 = change_speed(temp_val_1);
        temp_val_2 = change_speed(temp_val_2, true);
        if(temp_val_1 != 0 || temp_val_2 != 0){
            move_point_right_down(temp_val_2, temp_val_1);
        }
    }
}
