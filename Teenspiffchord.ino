//#include <Versalino.h>

// teenspiffchord v1 
// - reinvented spiffchorder
// - ment for teensy 2.0 (atmega32u4) but might work on others

#include <Bounce.h>
#include "nasa_us.h"

#define DEBOUNCE_TIMEOUT 10
#define TYPETIME 500          // 500 for noooooobs
#define BUTTON_F 4
#define BUTTON_C 3
#define BUTTON_N 2
#define BUTTON_I 14
#define BUTTON_M 15
#define BUTTON_R 16
#define BUTTON_P 17
#define LEDPIN 13

Bounce button0 = Bounce(BUTTON_P, DEBOUNCE_TIMEOUT);
Bounce button1 = Bounce(BUTTON_R, DEBOUNCE_TIMEOUT);
Bounce button2 = Bounce(BUTTON_M, DEBOUNCE_TIMEOUT);
Bounce button3 = Bounce(BUTTON_I, DEBOUNCE_TIMEOUT);
Bounce button4 = Bounce(BUTTON_N, DEBOUNCE_TIMEOUT);
Bounce button5 = Bounce(BUTTON_C, DEBOUNCE_TIMEOUT);
Bounce button6 = Bounce(BUTTON_F, DEBOUNCE_TIMEOUT);

unsigned long presstime;
boolean pressed;
int chordvalue;
int keymapmode;
int pressedkey;
int shiftmode;
int buttonsheld;
int currentmodifier;

#define FUNCTION_MODE 1
#define NUMBER_MODE 2
#define DEFAULT_MODE 3

#define SHIFT 1

int getkey(int value, int keymap) {
	int key;
	if (keymap == FUNCTION_MODE) {
		key = keymap_function[value];
	} else if (keymap == NUMBER_MODE) {
		key = keymap_numsym[value];
	} else {
		key = keymap_default[value];
	}
	return key;
}

void handle_shiftmode() {
    if (shiftmode) {
      Keyboard.set_modifier(currentmodifier);
      Keyboard.send_now();
      shiftmode = 0;
      currentmodifier = 0;
    }
}

void macro(int key1, int key2, int key3, int key4, int key5, int key6) {
	
	handle_shiftmode();

	if (key1 > 0) {
		Keyboard.set_key1(key1);
	}
	if (key2 > 0) {
		Keyboard.set_key2(key2);
	}
	if (key3 > 0) {
		Keyboard.set_key3(key3);
	}
	if (key4 > 0) {
		Keyboard.set_key4(key4);
	}
	if (key5 > 0) {
		Keyboard.set_key5(key3);
	}
	if (key6 > 0) {
		Keyboard.set_key6(key3);
	}
    Keyboard.send_now();

    delay(10);
    releasekeys();
}

void send_key(int key) {
	macro(key, KEY__, KEY__, KEY__, KEY__, KEY__);
}

void macro(int key1, int key2) {
	macro(key1, key2, KEY__, KEY__, KEY__, KEY__);
}

void macro(int key1, int key2, int key3) {
	macro(key1, key2, key3, KEY__, KEY__, KEY__);
}

void macro(int key1, int key2, int key3, int key4) {
	macro(key1, key2, key3, key4, KEY__, KEY__);
}

void macro(int key1, int key2, int key3, int key4, int key5) {
	macro(key1, key2, key3, key4, key5, KEY__);
}


void processchord(int value) {

  pressedkey = getkey(value,keymapmode);
      
	switch (pressedkey) {
		case MODE_FUNC:
			// set function mode
			keymapmode = FUNCTION_MODE;
			break;
		case  MODE_NUM:
			// set number mode
			keymapmode = NUMBER_MODE;
			break;
		case  MODE_RESET:
			// set mode back to default mode
			keymapmode = DEFAULT_MODE;
			break;
		case  SHIFTMODE:
			// pressed shift
			shiftmode = SHIFT;
			currentmodifier = (currentmodifier | MODIFIERKEY_SHIFT);
			break;
		case  ALTSHIFTMODE:
			// pressed alt
			shiftmode = SHIFT;
			currentmodifier = (currentmodifier | MODIFIERKEY_ALT);
			break;
		case  CTRLSHIFTMODE:
			// pressed ctrl
			shiftmode = SHIFT;
			currentmodifier = (currentmodifier | MODIFIERKEY_CTRL);
			break;
		case  NUMSHIFTMODE:
			// pressed numshiftmode ( N+C )
			shiftmode = SHIFT;
			currentmodifier = (currentmodifier | MODIFIERKEY_SHIFT);
			keymapmode = NUMBER_MODE;
			break;
		case  MACRO_quotes:
			macro(KEY_QUOTE, KEY_QUOTE, KEY_LEFT);
			break;
		case  MACRO_00:
			macro(KEY_0, KEY_0);
			break;
		case MACRO_000:
			macro(KEY_0, KEY_0, KEY_0);
			break;
		case  MACRO_parens:
			shiftmode = SHIFT;
			macro(KEY_9, KEY_0);
			send_key(KEY_LEFT);
			break;
		default:
			// Normal chord, just print the output :D
			send_key(pressedkey);
			keymapmode = DEFAULT_MODE;
			break;
	}
}

void releasekeys() {
	Keyboard.set_modifier(0);
	Keyboard.set_key1(0);
	Keyboard.set_key2(0);
	Keyboard.set_key3(0);
	Keyboard.set_key4(0);
	Keyboard.set_key5(0);
	Keyboard.set_key6(0);
	Keyboard.send_now();
}

void setup() {
	pinMode(0, INPUT_PULLUP);
	pinMode(BUTTON_F, INPUT_PULLUP);
	pinMode(BUTTON_C, INPUT_PULLUP);
	pinMode(BUTTON_N, INPUT_PULLUP);
	pinMode(BUTTON_I, INPUT_PULLUP);
	pinMode(BUTTON_M, INPUT_PULLUP);
	pinMode(BUTTON_R, INPUT_PULLUP);
	pinMode(BUTTON_P, INPUT_PULLUP);
	pinMode(LEDPIN, OUTPUT);
}

void loop() {
	button0.update();
	button1.update();
	button2.update();
	button3.update();
	button4.update();
	button5.update();
	button6.update();

	if (!pressed) { 
		if (chordvalue) { 
			releasekeys(); 
		}
		chordvalue = 0; 
		buttonsheld = 0; 
		digitalWrite(LEDPIN, LOW);
	}

	if (button0.fallingEdge()) {
		if (!pressed) {
			pressed=true;
			presstime=millis();
		}
		buttonsheld++;
		chordvalue += 1;
	}
	if (button1.fallingEdge()) {
		if (!pressed) {
			pressed=true;
			presstime=millis();
		}
		buttonsheld++;
		chordvalue += 2;
	}
	if (button2.fallingEdge()) {
		if (!pressed) {
			pressed=true;
			presstime=millis();
		}
		buttonsheld++;
		chordvalue += 4;
	}
	if (button3.fallingEdge()) {
		if (!pressed) {
			pressed=true;
			presstime=millis();
		}
		buttonsheld++;
		chordvalue += 8;
	}
	if (button4.fallingEdge()) {
		if (!pressed) {
			pressed=true;
			presstime=millis();
		}
		buttonsheld++;
		chordvalue += 16;
	}
	if (button5.fallingEdge()) {
		if (!pressed) {
			pressed=true;
			presstime=millis();
		}
		buttonsheld++;
		chordvalue += 32;
	}
	if (button6.fallingEdge()) {
		if (!pressed) {
			pressed=true;
			presstime=millis();
		}
		buttonsheld++;
		chordvalue += 64;
	}

	if (button0.risingEdge()) {
		buttonsheld--;
	}
	if (button1.risingEdge()) {
		buttonsheld--;
	}
	if (button2.risingEdge()) {
		buttonsheld--;
	}
	if (button3.risingEdge()) {
		buttonsheld--;
	}
	if (button4.risingEdge()) {
		buttonsheld--;
		}
	if (button5.risingEdge()) {
		buttonsheld--;
	}
	if (button6.risingEdge()) {
		buttonsheld--;
	}

	if (buttonsheld == 0) {
		pressed = false;
	}

	if (pressed) {
		digitalWrite(LEDPIN, HIGH);
	}

	if (chordvalue) {
		if ((presstime + TYPETIME) < millis()) {
			processchord(chordvalue); 
		} else if (buttonsheld == 0) {
			processchord(chordvalue);
			pressed = false;      
		}
	}

}

