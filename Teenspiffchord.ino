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

unsigned long press_time;
boolean pressed;
int chord_value;
int keymap_mode;
int pressed_key;
int shift_mode;
int buttons_held;
int current_modifier;

#define FUNCTION_MODE 1
#define NUMBER_MODE 2
#define DEFAULT_MODE 3

#define SHIFT 1

int get_key(int value, int keymap) {
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

void process_chord(int value) {

  pressed_key = get_key(value, keymap_mode);
      
  if (pressed_key == MODE_FUNC) {
    // set function mode
    keymap_mode = FUNCTION_MODE;
  } else if (pressed_key == MODE_NUM) {
    // set number mode
    keymap_mode = NUMBER_MODE;
  } else if (pressed_key == MODE_RESET) {
    // set mode back to default mode
    keymap_mode = DEFAULT_MODE;
  } else if (pressed_key == shift_mode) {
    // pressed shift
    shift_mode = SHIFT;
    current_modifier = (current_modifier | MODIFIERKEY_SHIFT);
  } else if (pressed_key == ALTSHIFTMODE) {
    // pressed alt
    shift_mode = SHIFT;
    current_modifier = (current_modifier | MODIFIERKEY_ALT);
  } else if (pressed_key == CTRLSHIFTMODE) {
    // pressed ctrl
    shift_mode = SHIFT;
    current_modifier = (current_modifier | MODIFIERKEY_CTRL);
  } else if (pressed_key == NUMSHIFTMODE) {
    // pressed numshift_mode ( N+C )
    shift_mode = SHIFT;
    current_modifier = (current_modifier | MODIFIERKEY_SHIFT);
    keymap_mode = NUMBER_MODE;
  } else if (pressed_key == MACRO_quotes) {
    if (shift_mode) {
      Keyboard.set_modifier(current_modifier);
      Keyboard.send_now();
      shift_mode = 0;
      current_modifier = 0;
    }
    Keyboard.set_key1(KEY_QUOTE);
    Keyboard.set_key2(KEY_QUOTE);
    Keyboard.set_key3(KEY_LEFT);
    Keyboard.send_now();

    delay(10);
    released_keys();
        
// #define MACRO_00 144
// #define MACRO_000 145
// #define MACRO_parens 146

  } else {
    // normal chord, just print the output :D

    if (shift_mode) {
      Keyboard.set_modifier(current_modifier);
      Keyboard.send_now();
      shift_mode = 0;
      current_modifier = 0;
    }
    Keyboard.set_key1(pressed_key);
    Keyboard.send_now();
    delay(10);
    released_keys();
    keymap_mode = DEFAULT_MODE;
  }
}

void released_keys() {
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
    if (chord_value) { 
      released_keys(); 
    }
    chord_value = 0; 
    buttons_held = 0; 
    digitalWrite(LEDPIN, LOW);
  }

  if (button0.fallingEdge()) {
    if (!pressed) {
      pressed=true;
      press_time=millis();
    }
    buttons_held++;
    chord_value += 1;
  }
  if (button1.fallingEdge()) {
    if (!pressed) {
      pressed=true;
      press_time=millis();
    }
    buttons_held++;
    chord_value += 2;
  }
  if (button2.fallingEdge()) {
    if (!pressed) {
      pressed=true;
      press_time=millis();
    }
    buttons_held++;
    chord_value += 4;
  }
  if (button3.fallingEdge()) {
    if (!pressed) {
      pressed=true;
      press_time=millis();
    }
    buttons_held++;
    chord_value += 8;
  }
  if (button4.fallingEdge()) {
    if (!pressed) {
      pressed=true;
      press_time=millis();
    }
    buttons_held++;
    chord_value += 16;
  }
  if (button5.fallingEdge()) {
    if (!pressed) {
      pressed=true;
      press_time=millis();
    }
    buttons_held++;
    chord_value += 32;
  }
  if (button6.fallingEdge()) {
    if (!pressed) {
      pressed=true;
      press_time=millis();
    }
    buttons_held++;
    chord_value += 64;
  }

  if (button0.risingEdge()) {
    buttons_held--;
  }
  if (button1.risingEdge()) {
    buttons_held--;
  }
  if (button2.risingEdge()) {
    buttons_held--;
  }
  if (button3.risingEdge()) {
    buttons_held--;
  }
  if (button4.risingEdge()) {
    buttons_held--;
  }
  if (button5.risingEdge()) {
    buttons_held--;
  }
  if (button6.risingEdge()) {
    buttons_held--;
  }
  
  if (buttons_held == 0) {
    pressed = false;
  }

  if (pressed) {
    digitalWrite(LEDPIN, HIGH);
  }

  if (chord_value) {
    if ((press_time + TYPETIME) < millis()) {
      process_chord(chord_value); 
    } else if (buttons_held == 0) {
      process_chord(chord_value);
      pressed = false;      
    }
  }
}

