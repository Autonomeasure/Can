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
