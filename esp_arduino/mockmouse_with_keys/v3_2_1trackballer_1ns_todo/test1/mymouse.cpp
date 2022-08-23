#include "mymouse.h"

// // 左指针，右方向
// #define pointer_get_val_1 change_speed(get_ADC(pointer_x, adc_bias_pointer), false)
// #define pointer_get_val_2 change_speed(get_ADC(pointer_y, adc_bias_pointer), false)
// #define scroll_get_val_1 change_speed(get_ADC(rocker_x, adc_bias_rocker), false)
// #define scroll_get_val_2 change_speed(get_ADC(rocker_y, adc_bias_rocker), false)

// 左方向，右指针
#define pointer_get_val_1 change_speed(get_ADC(pointer_x, adc_bias_pointer), true)
#define pointer_get_val_2 change_speed(get_ADC(pointer_y, adc_bias_pointer), true)
#define scroll_get_val_1 change_speed(get_ADC(rocker_x, adc_bias_rocker), true)
#define scroll_get_val_2 change_speed(get_ADC(rocker_y, adc_bias_rocker), true)

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
    pinMode(s_key_1, INPUT_PULLUP); // INPUT
    pinMode(rocker_key,INPUT_PULLDOWN); // 34-39 can only be input and can not PULL UP/DOWN.
    pinMode(pointer_key,INPUT_PULLDOWN); // 34-39 can only be input and can not PULL UP/DOWN.

    // init tracker
    pinMode(white_led_pin, OUTPUT);
    pinMode(btn_pin, INPUT);
    digitalWrite(white_led_pin, HIGH);

    // init ble
    bleKeyboard.begin();
    bleMouse.begin();
}

void Mymouse::self_main(){

//     // 独立Key
//     if(digitalRead(s_key_1) == LOW){
//         delay(50); // 消抖
//         // bleMouse.click(MOUSE_LEFT);
//         bleMouse.press(MOUSE_LEFT);
//         while(digitalRead(s_key_1) == LOW){
//             // copy from below
//             temp_val_1 = pointer_get_val_1;
//             temp_val_2 = pointer_get_val_2;
//             if(temp_val_1 != 0 || temp_val_2 != 0){
//                 move_point_right_down(temp_val_1, temp_val_2);
//             }
//             // copy done

//             // add for media control
//             temp_val_1 = scroll_get_val_1;
//             temp_val_2 = scroll_get_val_2;
//             if(temp_val_2 >= rank_num-1){
//                 bleMouse.release(MOUSE_LEFT);
//                 bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
//                 while(scroll_get_val_2 >= rank_num-1){};
//             }
//             else if(temp_val_2 <= -rank_num+1){
//                 bleMouse.release(MOUSE_LEFT);
//                 bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
//                 while(scroll_get_val_2 <= -rank_num+1);
//             }

//             else if(temp_val_1 >= rank_num-1){ //LEFT
//                 bleMouse.release(MOUSE_LEFT);
//                 bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
//                 while(scroll_get_val_1 >= rank_num-1);
//             }
//             else if(temp_val_1 <= -rank_num+1){ //right
//                 bleMouse.release(MOUSE_LEFT);
//                 bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
//                 while(scroll_get_val_1 <= -rank_num+1);
//             }
//             // pause  KEY_MEDIA_PLAY_PAUSE
//             if(digitalRead(rocker_key) == HIGH){
//                 bleMouse.release(MOUSE_LEFT);
//                 delay(50); // 消抖
//                 bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
//                 while(digitalRead(rocker_key) == HIGH);
//                 delay(50);
//             }
//         };
//         bleMouse.release(MOUSE_LEFT);
//         delay(50);
//     }
//     // pointer key
//     if(digitalRead(pointer_key) == HIGH){
//         delay(50); // 消抖
//         bleMouse.press(MOUSE_LEFT);
//         while(digitalRead(pointer_key) == HIGH);
//         bleMouse.release(MOUSE_LEFT);
//         delay(50);
//     }

//     // rocker key
//     if(digitalRead(rocker_key) == HIGH){ // 注意要硬件/软件上拉
//         // 摇杆键多功能： 模式切换&普通右键
//         delay(150); // 消抖
//         delay(400);
//         if(digitalRead(rocker_key) == LOW){
//             mode = !mode;
//         }
//         else{
//             bleMouse.click(MOUSE_RIGHT);
//         }
//         while(digitalRead(rocker_key) == HIGH);
//         delay(150);
//     }

//     // pointer ADC
//     temp_val_1 = pointer_get_val_1;
//     temp_val_2 = pointer_get_val_2;
//     if(temp_val_1 != 0 || temp_val_2 != 0){
//         move_point_right_down(temp_val_1, temp_val_2);
//     }
    
//     // rocker ADC
//     // 方向键 or 滚轮 -- scroll rocker
//     temp_val_1 = scroll_get_val_1;
//     temp_val_2 = scroll_get_val_2;
//     if(mode){
//         // 滚轮
//         if(temp_val_1 != 0 || temp_val_2 != 0){
//             move_scroll_up_right(temp_val_2, temp_val_1);
//         }
//     }
//     else{
//         // 方向键
// //        Serial.println(temp_val_1, DEC);
//         if(temp_val_2 >= rank_num-1){
//             bleKeyboard.press(KEY_UP_ARROW);
//             while(scroll_get_val_2 >= rank_num-1){};
//             bleKeyboard.releaseAll();
//         }
//         else if(temp_val_2 <= -rank_num+1){
//             bleKeyboard.press(KEY_DOWN_ARROW);
//             while(scroll_get_val_2 <= -rank_num+1);
//             bleKeyboard.releaseAll();
//         }

//         else if(temp_val_1 >= rank_num-1){
//             bleKeyboard.press(KEY_LEFT_ARROW);
//             while(scroll_get_val_1 >= rank_num-1);
//             bleKeyboard.releaseAll();
//         }
//         else if(temp_val_1 <= -rank_num+1){
//             bleKeyboard.press(KEY_RIGHT_ARROW);
//             while(scroll_get_val_1 <= -rank_num+1);
//             // bleKeyboard.release(KEY_RIGHT_ARROW);
//             bleKeyboard.releaseAll();
//         }
//     }

    // -----------------

    // tracker Key
    btn_read_state = digitalRead(btn_pin);
    if(btn_read_state != btn_state) {
        btn_current_action_time = millis();
        if(btn_current_action_time - btn_last_action_time > bounce_interval) {
        btn_state = btn_read_state;
        btn_last_action_time = btn_current_action_time;
        Serial.println("click");
        if(btn_state == HIGH) {
            digitalWrite(white_led_pin, HIGH);
            bleMouse.release(MOUSE_LEFT);
        } else {
            digitalWrite(white_led_pin, LOW);
            bleMouse.press(MOUSE_LEFT);
        }
        }
    }

    // tracker Move
    x_move = x_direction.read_action();
    y_move = y_direction.read_action();
    if(x_move != 0 || y_move != 0) {
        Serial.println("move");
        Serial.println(x_move, DEC);
        Serial.println(y_move, DEC);
        move_point_right_down(x_move, y_move);
    }

}
