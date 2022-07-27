
#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <Arduino.h>
#include <FunctionalInterrupt.h>

// BUTTON 中断
class Button {
public:
	Button(uint8_t reqPin) : PIN(reqPin){
		pinMode(PIN, INPUT_PULLUP);
		attachInterrupt(PIN, std::bind(&Button::isr,this), FALLING);
	};
	~Button() {
		detachInterrupt(PIN);
	}

	void ARDUINO_ISR_ATTR isr() {
		numberKeyPresses += 1;
		pressed = true;
	}

	bool checkPressed() {
		if (pressed) {
			// Serial.printf("Button on pin %u has been pressed %u times\n", PIN, numberKeyPresses);
			pressed = false;
      return true;
		}
    return false;
	}

private:
	const uint8_t PIN;
    volatile int numberKeyPresses;
    volatile bool pressed;
};

#endif
