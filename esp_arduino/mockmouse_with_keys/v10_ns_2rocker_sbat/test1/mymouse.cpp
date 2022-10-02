#include "mymouse.h"

#define pointer_get_val_1 change_speed(get_ADC(pointer_x, adc_bias_pointer), true)
#define pointer_get_val_2 change_speed(get_ADC(pointer_y, adc_bias_pointer), true)
#define scroll_get_val_1 change_speed(get_ADC(rocker_x, adc_bias_rocker), false)
#define scroll_get_val_2 change_speed(get_ADC(rocker_y, adc_bias_rocker), false)

void Mymouse::set_up(){
    // init adc
    //set the resolution to 12 bits (0-4096)
    analogReadResolution(adc_bit);
    // battery ADC(ADC2)
    Serial.println(analogRead(adc_battery), DEC);
    // set bias
    Serial.println((get_bias(rocker_x) + get_bias(rocker_y))/2, DEC);
    adc_bias_rocker = (get_bias(rocker_x) + get_bias(rocker_y))/2;
    adc_bias_pointer = (get_bias(pointer_x) + get_bias(pointer_y))/2;
    // init gpio when not interrupt
    for(int idx = 0; idx < 4; idx++) {
        pinMode(pullup_input[idx], INPUT_PULLUP);
    }
    for(int idx = 0; idx < 2; idx++) {
        pinMode(pulldown_input[idx], INPUT_PULLDOWN);
    }
    digitalWrite(signal_out, HIGH);

    // init ble
    bleKeyboard.begin();
    bleMouse.begin();
}

void Mymouse::self_main(){
    // 独立Key
    if(digitalRead(s_key_left) == LOW){
        has_action = true;
        delay(50); // 消抖
        // bleMouse.click(MOUSE_LEFT);
        bleMouse.press(MOUSE_LEFT);
        while(digitalRead(s_key_left) == LOW){
            // copy from below
            temp_val_1 = pointer_get_val_1;
            temp_val_2 = pointer_get_val_2;
            if(temp_val_1 != 0 || temp_val_2 != 0){
                move_point_right_down(temp_val_1, temp_val_2);
            }
            // copy done


            // add for media control
            if(digitalRead(key_left_up) == LOW){
                bleMouse.release(MOUSE_LEFT);
                bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
                while(digitalRead(key_left_up) == LOW);
            }
            if(digitalRead(key_left_down) == LOW){
                bleMouse.release(MOUSE_LEFT);
                bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
                while(digitalRead(key_left_down) == LOW);
            }
        };
        bleMouse.release(MOUSE_LEFT);
        delay(50);
    }
    if(digitalRead(key_left_up) == LOW){
        has_action = true;
        delay(50); // 消抖
        bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
        while(digitalRead(key_left_up) == LOW);
    }
    if(digitalRead(key_left_down) == LOW){
        has_action = true;
        delay(50); // 消抖
        bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
        while(digitalRead(key_left_down) == LOW);
    }

    // pointer key
    if(digitalRead(pointer_key) == HIGH){
        delay(50); // 消抖
        bleMouse.press(MOUSE_LEFT);
        while(digitalRead(pointer_key) == HIGH);
        bleMouse.release(MOUSE_LEFT);
        delay(50);
    }

    // rocker key
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

    // pointer ADC
    temp_val_1 = pointer_get_val_1;
    temp_val_2 = pointer_get_val_2;
    if(temp_val_1 != 0 || temp_val_2 != 0){
        move_point_right_down(temp_val_1, temp_val_2);
    }
    
    // rocker ADC
    // 方向键 or 滚轮 -- scroll rocker
    temp_val_1 = scroll_get_val_1;
    temp_val_2 = scroll_get_val_2;
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
            while(scroll_get_val_2 >= rank_num-1){};
            bleKeyboard.releaseAll();
        }
        else if(temp_val_2 <= -rank_num+1){
            bleKeyboard.press(KEY_DOWN_ARROW);
            while(scroll_get_val_2 <= -rank_num+1);
            bleKeyboard.releaseAll();
        }

        else if(temp_val_1 >= rank_num-1){
            bleKeyboard.press(KEY_LEFT_ARROW);
            while(scroll_get_val_1 >= rank_num-1);
            bleKeyboard.releaseAll();
        }
        else if(temp_val_1 <= -rank_num+1){
            bleKeyboard.press(KEY_RIGHT_ARROW);
            while(scroll_get_val_1 <= -rank_num+1);
            // bleKeyboard.release(KEY_RIGHT_ARROW);
            bleKeyboard.releaseAll();
        }
    }
}
