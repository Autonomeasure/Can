#include "Can.h"

Can can = Can::Can(&Serial1, &Serial2, 38, 3);

// Set the variables for the "delay" without delay()
unsigned long previousMillis;
const int interval = 500; // Run the code two times a second

void setup() {
	Serial.begin(9600); // Debug serial connection
	Serial.print("Begin: ");
	Error *beginErrors; 
	uint8_t amountOfErrors = can.begin(beginErrors);
	if (amountOfErrors > 0) {
		// This means we have at least one error
	}
	can.configureRadio(); // Set the correct configuration in the APC220 module
}

void loop() {
	unsigned long currentMillis = millis();

	// Check here if the current time - the last time the code has ran >= interval, if that is the case run the code that needs to run
	if (currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis; // Set the previousMillis to the current millis for the next run of this loop

		// Tick logic here
		// Error handling
		Error *errors;
		uint8_t amountOfErrors = can.tick(errors); // Call Can::tick and save any errors that occured
		if (amountOfErrors > 0) {
			// Error(s) occured
		}
	}
}
