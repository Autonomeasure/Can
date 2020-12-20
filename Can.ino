#include "Can.h"

Can can(&Serial1, &Serial2, 38);

void setup() {
	Serial.begin(9600); // Debug serial connection
	Serial.print("Begin: ");
	Serial.println(can.begin());
	can.configureRadio();
}

void loop() {
	
}