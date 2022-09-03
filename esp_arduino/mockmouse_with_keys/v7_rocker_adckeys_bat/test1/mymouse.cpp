#include "mymouse.h"

// // 左指针，右方向
// #define pointer_get_val_1 change_speed(get_ADC(pointer_x, adc_bias_pointer), false)
// #define pointer_get_val_2 change_speed(get_ADC(pointer_y, adc_bias_pointer), false)

// 左方向，右指针
#define pointer_get_val_1 change_speed(get_ADC(pointer_x, adc_bias_pointer), true)
#define pointer_get_val_2 change_speed(get_ADC(pointer_y, adc_bias_pointer), true)

#define adc_keys_get_val get_ADC_keys(adc_key)

void Mymouse::set_up(){
    // init adc
    //set the resolution to 12 bits (0-4096)
    analogReadResolution(adc_bit);
    // battery ADC(ADC2)
    Serial.println(analogRead(adc_battery), DEC);
    // set bias
    Serial.println((get_bias(pointer_x) + get_bias(pointer_y))/2, DEC);
    adc_bias_pointer = (get_bias(pointer_x) + get_bias(pointer_y))/2;
    // init gpio when not interrupt
    for(int idx = 0; idx < 3; idx++) {
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
    has_action = false;
    // --------独立Key-----------
    if(digitalRead(s_key_1) == LOW){
        has_action = true;
        delay(50); // 消抖
        // bleMouse.click(MOUSE_LEFT);
        bleMouse.press(MOUSE_LEFT);
        while(digitalRead(s_key_1) == LOW){
            // copy from below
            temp_val_1 = pointer_get_val_1;
            temp_val_2 = pointer_get_val_2;
            if(temp_val_1 != 0 || temp_val_2 != 0){
                move_point_right_down(temp_val_1, temp_val_2);
            }
            // copy done

            // // add for media control
            // temp_val_1 = scroll_get_val_1;
            // temp_val_2 = scroll_get_val_2;
            // if(temp_val_2 >= rank_num-1){
            //     bleMouse.release(MOUSE_LEFT);
            //     bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
            //     while(scroll_get_val_2 >= rank_num-1){};
            // }
            // else if(temp_val_2 <= -rank_num+1){
            //     bleMouse.release(MOUSE_LEFT);
            //     bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
            //     while(scroll_get_val_2 <= -rank_num+1);
            // }

            // else if(temp_val_1 >= rank_num-1){ //LEFT
            //     bleMouse.release(MOUSE_LEFT);
            //     bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
            //     while(scroll_get_val_1 >= rank_num-1);
            // }
            // else if(temp_val_1 <= -rank_num+1){ //right
            //     bleMouse.release(MOUSE_LEFT);
            //     bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
            //     while(scroll_get_val_1 <= -rank_num+1);
            // }
            // // pause  KEY_MEDIA_PLAY_PAUSE
            // if(digitalRead(rocker_key) == HIGH){
            //     bleMouse.release(MOUSE_LEFT);
            //     delay(50); // 消抖
            //     bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
            //     while(digitalRead(rocker_key) == HIGH);
            //     delay(50);
            // }
        };
        bleMouse.release(MOUSE_LEFT);
        delay(50);
    }

    // ------pointer---------
    // pointer key
    if(digitalRead(pointer_key) == HIGH){
        has_action = true;
        delay(50); // 消抖
        bleMouse.press(MOUSE_LEFT);
        while(digitalRead(pointer_key) == HIGH);
        bleMouse.release(MOUSE_LEFT);
        delay(50);
    }
    // pointer ADC
    temp_val_1 = pointer_get_val_1;
    temp_val_2 = pointer_get_val_2;
    if(temp_val_1 != 0 || temp_val_2 != 0){
        has_action = true;
        move_point_right_down(temp_val_1, temp_val_2);
    }

    // // ------direction_keys--------
    temp_val_1 = adc_keys_get_val;
    if(temp_val_1 != 0){
        has_action = true;
        if(temp_val_1 == AdcKey::Dir1_Ok){
            // 摇杆键多功能： 模式切换&普通右键
            delay(150); // 消抖
            delay(400);
            if(adc_keys_get_val != AdcKey::Dir1_Ok){
                mode = !mode;
            }
            else{
                bleMouse.click(MOUSE_RIGHT);
            }
            while(adc_keys_get_val == AdcKey::Dir1_Ok);
            delay(150);
        }
        // 滚轮mode=ture
        else if(mode) {
            if(temp_val_1 == AdcKey::Dir1_Up){
                delay(50); // 消抖
                bleMouse.move(0,0, 5, 0); //数值可改变速度
            }
            if(temp_val_1 == AdcKey::Dir1_Down){
                delay(50); // 消抖
                bleMouse.move(0,0, -5, 0); //数值可改变速度
            }
            if(temp_val_1 == AdcKey::Dir1_Left){
                delay(50); // 消抖
                bleMouse.move(0,0, 0, -5); //数值可改变速度
            }
            if(temp_val_1 == AdcKey::Dir1_Right){
                delay(50); // 消抖
                bleMouse.move(0,0, 0, 5); //数值可改变速度
            }
        }
        // 方向键mode=false
        else if(!mode) {
            if(temp_val_1 == AdcKey::Dir1_Up){
                delay(50); // 消抖
                bleKeyboard.press(KEY_UP_ARROW);
                while(temp_val_1 == AdcKey::Dir1_Up);
                bleKeyboard.releaseAll();
            }
            if(temp_val_1 == AdcKey::Dir1_Down){
                delay(50); // 消抖
                bleKeyboard.press(KEY_DOWN_ARROW);
                while(temp_val_1 == AdcKey::Dir1_Down);
                bleKeyboard.releaseAll();
            }
            if(temp_val_1 == AdcKey::Dir1_Left){
                delay(50); // 消抖
                bleKeyboard.press(KEY_LEFT_ARROW);
                while(temp_val_1 == AdcKey::Dir1_Left);
                bleKeyboard.releaseAll();
            }
            if(temp_val_1 == AdcKey::Dir1_Right){
                delay(50); // 消抖
                bleKeyboard.press(KEY_RIGHT_ARROW);
                while(temp_val_1 == AdcKey::Dir1_Right);
                bleKeyboard.releaseAll();
            }
        }
    }

    // --------seperate keys-------
    // else if()
    
    // -----do nothing and manage speed_factor-----
    // if(has_action){
    //     do_nothing_cnt = 0;
    //     speed_factor = (speed_factor < 1.0)?(speed_factor + 0.005):speed_factor;
    // }
    // else{
    //     do_nothing_cnt += 1;
    // }
    // if(do_nothing_cnt >= 1000){
    //     speed_factor = 0.5; //reset
    // }

}
