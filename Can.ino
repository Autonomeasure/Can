#include "Can.h"

#define interval 100

long previousMillis = 0;

Can can(7, 5, 9600, 4, 3, 9600);

void setup() {
  Serial.begin(9600);
  if (!can.bmp->begin()) {
    Serial.println("BMP sensor failed initializing!");
    while (1)
      ;
  }
  can.begin();
  // Serial.println("Before can.begin();");
  // uint8_t cBegin = can.begin();
  // Serial.println("After can.begin();");
  // if (cBegin > 0) {
  //   Serial.println("Initialization failed..");
  //   Serial.println(cBegin);
  // }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    can.tick();
  }
}
