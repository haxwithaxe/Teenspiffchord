#!/bin/bash

make_buttons() {
	local pins=$@
	for pin in $pins; do
		echo "Bounce button_${pin} = Bounce(${pin}, DEBOUNCE_TIMEOUT);"
	done
}

make_setup() {
	local pins=$@
	for pin in $pins; do
		echo "pinMode(${pin}, INPUT_PULLUP);"
	done
}

make_loop() {
	local pins=$@
	for pin in $pins; do
		echo "	button_${pin}.update();"
	done

	for pin in $pins; do
		cat - <<EOButtonSense
	if (button_${pin}.fallingEdge()) {
		if (!pressed) {
			Serial.println("${pin}");
			pressed=true;
			presstime=millis();
		}
		buttonsheld++;
	}

	if (button_${pin}.risingEdge()) {
		buttonsheld--;
	}
EOButtonSense
	done
}


make_ino() {
	local pins=$@
	cat - <<EOF
// teenspiffchord v1 
// - reinvented spiffchorder
// - ment for teensy 2.0 (atmega32u4) but might work on others

#include <Bounce.h>
#include "nasa_us.h"

#define DEBOUNCE_TIMEOUT 10
#define TYPETIME 500          // 500 for noooooobs
#define LEDPIN 13

$(make_buttons $pins)

unsigned long presstime;
boolean pressed;
int pressedkey;
int buttonsheld;

void setup() {
	Serial.begin(9600);
	pinMode(0, INPUT_PULLUP);
	$(make_setup $pins)
	pinMode(LEDPIN, OUTPUT);
	buttonsheld = 0;
}

void loop() {

	if (!pressed) { 
		buttonsheld = 0; 
	}

$(make_loop $pins)

	// -------------------------
	if (buttonsheld == 0) {
		pressed = false;
	}
}
EOF
}

make_ino 1 2 3 4 5 6 7 14 15 16 17 18 19
