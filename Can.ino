#include "Can.h"

#define interval 100

long previousMillis = 0;

//Can can(7, 5, 9600, 4, 3, 9600);/
Can *can;

void setup() {
  Serial.begin(9600);
  // Serial.println("Setup Can object");
  // can = new Can(7, 5, 9600, 4, 3, 9600);
  // uint8_t cBegin = can->begin();
  // if (cBegin > 0) {
  //   Serial.println("Initialization failed..");
  //   Serial.println(cBegin);
  // }
}

void loop() {
  // unsigned long currentMillis = millis();

  // if (currentMillis - previousMillis > interval) {
  //   previousMillis = currentMillis;
  //   can->tick();
  // }
}
