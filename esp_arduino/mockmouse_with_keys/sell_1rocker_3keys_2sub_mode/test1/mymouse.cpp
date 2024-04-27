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
    pinMode(s_key_2, INPUT_PULLUP); // INPUT
    pinMode(s_key_3, INPUT_PULLUP); // INPUT
    pinMode(rocker_key,INPUT_PULLDOWN); // 34-39 can only be input and can not PULL UP/DOWN.

    pinMode(stat_led, OUTPUT);
    digitalWrite(stat_led, LOW);
    pinMode(stat_sub_led, OUTPUT);
    digitalWrite(stat_sub_led, LOW);

    // Serial.printf("mymouse init\n");
    // init ble
    bleKeyboard.begin();
    bleMouse.begin();
}

void Mymouse::self_main(){
    // ----------------按键区-------------------
    // 左键
    if(digitalRead(s_key_1) == LOW){
        delay(50); // 消抖
        bleMouse.press(MOUSE_LEFT);
        //消抖
        while(digitalRead(s_key_1) == LOW);
        bleMouse.release(MOUSE_LEFT);
        delay(50);
    }
    // 右键
    if(digitalRead(s_key_2) == LOW){
        delay(50); // 消抖
        bleMouse.press(MOUSE_RIGHT);
        //消抖
        while(digitalRead(s_key_2) == LOW);
        bleMouse.release(MOUSE_RIGHT);
        delay(50);
    }
    // 模式切换
    if(digitalRead(s_key_3) == LOW){
        delay(50); // 消抖
        while(digitalRead(s_key_1) == LOW);
        mode = !mode;
        // 亮灯
        if(mode){
            digitalWrite(stat_led, HIGH);
        }
        else{
            digitalWrite(stat_led, LOW);
        }
        delay(50);
    }
    // 摇杆按键
    if(digitalRead(rocker_key) == HIGH){
        if(!mode){
            // 指针模式对应左键
            delay(50); // 消抖
            bleMouse.press(MOUSE_LEFT);
            while(digitalRead(rocker_key) == HIGH){
                // 按下时移动光标
                temp_val_1 = get_ADC(rocker_x, adc_bias_rocker);
                temp_val_2 = get_ADC(rocker_y, adc_bias_rocker);
                if(temp_val_1 != 0 || temp_val_2 != 0){
                    move_point_right_down(temp_val_1, temp_val_2);
                }
            }
            bleMouse.release(MOUSE_LEFT);
            delay(50);
        }
        else{
            // 移动模式对应子模式切换
            delay(50); // 消抖
            while(digitalRead(rocker_key) == HIGH);
            sub_mode = !sub_mode;
            if(sub_mode){
                digitalWrite(stat_sub_led, HIGH);
            }
            else{
                digitalWrite(stat_sub_led, LOW);
            }
            delay(50);
        }
    }

    // ----------------移动区-------------------
    // 摇杆 or 滚轮
    temp_val_1 = get_ADC(rocker_x, adc_bias_rocker);
    temp_val_2 = get_ADC(rocker_y, adc_bias_rocker);
    if(mode){
        temp_val_1 = change_speed(temp_val_1, true);
        temp_val_2 = change_speed(temp_val_2);
        
        if(!sub_mode){
            // 滚轮
            if(temp_val_1 != 0 || temp_val_2 != 0){
                move_scroll_up_right(temp_val_1, temp_val_2);
            }
        }
        else{
            // 方向键
            if(temp_val_2 >= rank_num-1){
                bleKeyboard.press(KEY_UP_ARROW);
                while(temp_val_2 >= rank_num-1){};
                bleKeyboard.releaseAll();
            }
            else if(temp_val_2 <= -rank_num+1){
                bleKeyboard.press(KEY_DOWN_ARROW);
                while(temp_val_2 <= -rank_num+1);
                bleKeyboard.releaseAll();
            }

            else if(temp_val_1 >= rank_num-1){
                bleKeyboard.press(KEY_LEFT_ARROW);
                while(temp_val_1 >= rank_num-1);
                bleKeyboard.releaseAll();
            }
            else if(temp_val_1 <= -rank_num+1){
                bleKeyboard.press(KEY_RIGHT_ARROW);
                while(temp_val_1 <= -rank_num+1);
                // bleKeyboard.release(KEY_RIGHT_ARROW);
                bleKeyboard.releaseAll();
            }
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
