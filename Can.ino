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
 */
 
#include "Can.h"

Can can(1023.15, 28, 30, NULL, NULL);

void setup() {
  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);
    Serial.begin(9600);
  if (!can.begin()) {
    Serial.println("Something went wrong"); 
  }
  can.configureRadio();
}

void loop() {
  can.tick();
  delay(333);
}
