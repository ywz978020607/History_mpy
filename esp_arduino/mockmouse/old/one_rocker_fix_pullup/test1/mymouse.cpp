#include "mymouse.h"

void Mymouse::set_up(){
    // init adc
    //set the resolution to 12 bits (0-4096)
    analogReadResolution(adc_bit);
    // set bias
    Serial.println((get_bias(rocker_x) + get_bias(rocker_y))/2, DEC);
    adc_bias_rocker = (get_bias(rocker_x) + get_bias(rocker_y))/2;
    // init gpio when not interrupt
    pinMode(left_key, INPUT_PULLUP); // INPUT
    pinMode(right_key,INPUT_PULLUP); // 34-39 can only be input and can not PULL UP/DOWN.

    // Serial.printf("mymouse init\n");
    // init ble
    bleMouse.begin();
}

void Mymouse::self_main(){
    // KEY 中断类
    // if(left_key.checkPressed()){
    // press_key(MOUSE_LEFT);
    // }
    // KEY非中断
    if(digitalRead(left_key) == LOW){
        delay(150); // 消抖
//        Serial.println("push left button.\n");
        press_key(MOUSE_LEFT);
        //消抖
        while(digitalRead(left_key) == LOW);
        delay(150);
    }
    if(digitalRead(right_key) == LOW){ // 注意要硬件/软件上拉
        // 摇杆键多功能： 模式切换&普通右键
        delay(150); // 消抖
        
        delay(400);
        if(digitalRead(right_key) == HIGH){
            mode = !mode;    
//            Serial.println("change mode");
//            Serial.println(mode?1:0, DEC);
        }
        else{
            press_key(MOUSE_RIGHT);
//            Serial.println("right");
        }
        while(digitalRead(right_key) == LOW);
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
