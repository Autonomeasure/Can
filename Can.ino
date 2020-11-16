/*
 * CanSat 2020-2021 Team Autonomeasure
 * Code written by: Joep van Dijk
 * First mission:
 *    Our first mission is to be able to land properly, collect data from the BMP280 sensor and send that data to the groundstation at least one time per second. 
 * 
 * Second mission:
 *    Our second mission is to make a sustainable data collection station. 
 * 
 * Links:
 *    GitHub Autonomeasure: https://github.com/Autonomeasure
 *    GitHub Can repo:      https://github.com/Autonomeasure/Can
 */
#include "Can.h"

#define interval 100

long previousMillis = 0;

Can *can;

void setup() {
  Serial.begin(9600);
  can = new Can(1023.25); // seaLevelhPa is not correct yet
}

void loop() {
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis > interval) {
		previousMillis = currentMillis;
		// TICK
		if (can->getState() == 0b00000000) { // Check if the STATE is still 0
			// Check if we're falling => state 0b00000001
			// Check if we are falling using the acceleration and the altitude history
			bool altitude_falling = false;
			bool acceleration_falling = false;
			for (uint8_t i = 0; i < 9; i++) {
				if (can->altitude_history[9] - can->altitude_history[i] > 1.5) {
					altitude_falling = true;
				}
			}
			Vector3 *a;
			can->getAcceleration(a);
		}
	}
}
