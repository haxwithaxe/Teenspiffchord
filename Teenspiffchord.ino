// teenspiffchord v2
// - reinvented spiffchorder
// - redesigned for teensy 3.2 original worked on teensy 2.0

#include <Bounce.h>
#include "nasa_us.h"

/* uncomment to enable debug output
#define DEBUG
*/

// Magic keyboard numbers
#define FUNCTION_MODE 1
#define NUMBER_MODE 2
#define DEFAULT_MODE 3
#define SHIFT 1

// Debounce interval
#define DEBOUNCE_TIMEOUT 10

// Time to wait before assuming the chord has been completely pressed.
#define TYPETIME 50

// Pins
#define BUTTON_F 2
#define BUTTON_C 3
#define BUTTON_N 4
#define BUTTON_I 15
#define BUTTON_M 16
#define BUTTON_R 17
#define BUTTON_P 18
#define LEDPIN 13


unsigned long presstime;
boolean pressed;
int chordvalue;
int last_chordvalue = 0;
int keymapmode;
int pressedkey;
int shiftmode;
int buttonsheld;
int last_buttonsheld = 0;
int currentmodifier;
bool repeat_mode = false;

Bounce button_p = Bounce(BUTTON_P, DEBOUNCE_TIMEOUT);
Bounce button_r = Bounce(BUTTON_R, DEBOUNCE_TIMEOUT);
Bounce button_m = Bounce(BUTTON_M, DEBOUNCE_TIMEOUT);
Bounce button_i = Bounce(BUTTON_I, DEBOUNCE_TIMEOUT);
Bounce button_n = Bounce(BUTTON_N, DEBOUNCE_TIMEOUT);
Bounce button_c = Bounce(BUTTON_C, DEBOUNCE_TIMEOUT);
Bounce button_f = Bounce(BUTTON_F, DEBOUNCE_TIMEOUT);


void debug(char*name) {
#ifdef DEBUG
	Serial.print(String(name));
	Serial.print(": "+String(pressed));
	Serial.print(", buttonsheld = "+String(buttonsheld, DEC));
	Serial.print(", last buttonsheld = "+String(last_buttonsheld, DEC));
	Serial.print(", chordvalue = "+String(chordvalue, DEC));
	Serial.print(", last_chordvalue = "+String(last_chordvalue, DEC));
	Serial.println();
#endif
}

/*
   Bypass checks if in repeat mode for rapid 
   keypresses while holing down keys

   repeat mode not implemented but the bypasses required are in place.
 */
bool in_repeat_mode(bool condition) {
	if (repeat_mode) return true;
	return condition;
}

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

	Serial.begin(9600);
	pinMode(LEDPIN, OUTPUT);

	pinMode(0, INPUT_PULLUP);

	pinMode(BUTTON_F, INPUT_PULLUP);
	pinMode(BUTTON_C, INPUT_PULLUP);
	pinMode(BUTTON_N, INPUT_PULLUP);
	pinMode(BUTTON_I, INPUT_PULLUP);
	pinMode(BUTTON_M, INPUT_PULLUP);
	pinMode(BUTTON_R, INPUT_PULLUP);
	pinMode(BUTTON_P, INPUT_PULLUP);
}


void loop() {
	button_p.update();
	button_r.update();
	button_m.update();
	button_i.update();
	button_n.update();
	button_c.update();
	button_f.update();

	if (!pressed) { 
		if (chordvalue) { 
			releasekeys(); 
		}
		digitalWrite(LEDPIN, LOW);
	}
	buttonsheld = 0; 
	chordvalue = 0; 

	// Track the buttons that are currently pressed.
	if (button_p.read() == LOW) {
		pressed=true;
		buttonsheld++;
		chordvalue += 1;
	}

	if (button_r.read() == LOW) {
		pressed=true;
		buttonsheld++;
		chordvalue += 2;
	}

	if (button_m.read() == LOW) {
		pressed=true;
		buttonsheld++;
		chordvalue += 4;
	}

	if (button_i.read() == LOW) {
		pressed=true;
		buttonsheld++;
		chordvalue += 8;
	}

	if (button_n.read() == LOW) {
		pressed=true;
		buttonsheld++;
		chordvalue += 16;
	}

	if (button_c.read() == LOW) {
		pressed=true;
		buttonsheld++;
		chordvalue += 32;
	}

	if (button_f.read() == LOW) {
		pressed=true;
		buttonsheld++;
		chordvalue += 64;
	}
	
	// Start the timer for any new keypress.
	if (button_p.fallingEdge()) {
			debug("+ pinky");
			presstime=millis();
	}

	if (button_r.fallingEdge()) {
			debug("+ ring");
			presstime=millis();
	}

	if (button_m.fallingEdge()) {
			debug("+ middle");
			presstime=millis();
	}

	if (button_i.fallingEdge()) {
			debug("+ index");
			presstime=millis();
	}

	if (button_n.fallingEdge()) {
			debug("+ near");
			presstime=millis();
	}

	if (button_c.fallingEdge()) {
			debug("+ center");
			presstime=millis();
	}

	if (button_f.fallingEdge()) {
			debug("+ far");
			presstime=millis();
	}

	if (buttonsheld == 0) {
		pressed = false;
		last_chordvalue = 0;
	}

	if (pressed) {
		digitalWrite(LEDPIN, HIGH);
	}

	if (chordvalue) {
		bool is_new_chordvalue;
		if (chordvalue > last_chordvalue) {
			is_new_chordvalue = true;
		} else {
			is_new_chordvalue = false;
			last_buttonsheld = buttonsheld;
			last_chordvalue = chordvalue;
		}

		if ((presstime + TYPETIME) < millis() &&  in_repeat_mode(is_new_chordvalue)) {
			processchord(chordvalue);
			last_chordvalue = chordvalue;
			last_buttonsheld = buttonsheld;
		} else if (buttonsheld == 0) {
			processchord(chordvalue);
			pressed = false;
			last_chordvalue = 0;
		}
	}
}

