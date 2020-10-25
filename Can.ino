#include "Can.h"

Can can(4, 3, 9600);



void setup() {
  Serial.begin(9600);
//  if (!can.mpu->begin()) {
//    Serial.println("Failed to find MPU6050 chip");
//    while (1) {
//      delay(10);
//    }
//  }
}

void loop() {
  Vector3 a;
  Vector3 gy;
  can.getGy(&a, &gy);
  Serial.print("Accelerometer: ");
  Serial.print("x = ");
  Serial.print(a.x);
  Serial.print(" | y = ");
  Serial.print(a.y);
  Serial.print(" | z = ");
  Serial.println(a.z);

  Serial.print("Gyroscope: ");
  Serial.print("x = ");
  Serial.print(gy.x);
  Serial.print(" | y = ");
  Serial.print(gy.y);
  Serial.print(" | z = ");
  Serial.println(gy.z);

  Location loc = can.getLocation();
  Serial.print("Lat: ");
  Serial.println(loc.lat);
  Serial.print("Lon: ");
  Serial.println(loc.lon);
  delay(200);
}
