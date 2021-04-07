/*
 * CanSat 2020-2021 Team Autonomeasure - Can
 * Code written by: Joep van Dijk
 * First mission:
 *    Our first mission is to be able to land properly, collect data from the BMP280 sensor and send that data to the groundstation at least one time per second. 
 * 
 * Second mission:
 *    Our second mission is to make a sustainable data collection station. 
 * 
 * Links:
 *    GitHub Autonomeasure:           https://github.com/Autonomeasure
 *    GitHub Can repo:                https://github.com/Autonomeasure/Can
 *    GitHub GroundStation repo:      https://github.com/Autonomeasure/GroundStation
 *    Instagram:                      https://instagram.com/Autonomeasure/
 *    
 * This project falls under the GNU GPL-3.0 license, see LICENSE or https://www.gnu.org/licenses/gpl-3.0.txt. 
 */
#include "Can.h"

Can can = Can(&Serial2, 38, 3, 1016);

// Set the variables for the "delay" without delay()
unsigned long previousMillis;
const int interval = 500; // Run the code two times a second

void setup() {
	Serial.begin(9600); // Debug serial connection
	uint8_t error = can.begin();
	if (error > 0) {
		// This means we have an error
	}
//	can.configureRadio(); // Set the correct configuration in the APC220 module
}

void loop() {
	unsigned long currentMillis = millis();

	// Check here if the current time - the last time the code has ran >= interval, if that is the case run the code that needs to run
	if (currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis; // Set the previousMillis to the current millis for the next run of this loop

		// Tick logic here
		// Error handling
		uint8_t error = can.tick(); // Call Can::tick and save any errors that occured
//    Serial1.println("lol");
		if (error > 0) {
			// Error(s) occured
//      Serial.print("ERROR: ");
//      Serial.println(error);
		}
	}
}
