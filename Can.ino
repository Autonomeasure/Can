#include "Can.h"

// BMP280 (temperature, pressure and altitude)
  // SCL (clock)
  // SDA (data)
  // I2C address = 0x76 (can be found in Adafruit_BMP280.h)
// GPS
  // TX
  // RX
// Accelerometer
  // SCL (clock)
  // SDA (data)
// APC 220
  // RX
  // TX

Can can(2, 3, 9600, 4, 5, 9600);


void setup() {
  Serial.begin(9600);

//  if (!can.bmp->begin()) {
//    Serial.println("Can't find BMP sensor..");
//    while (true);
//  }
}

void loop() {
//  Serial.println(can.bmp->readTemperature());
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
  delay(200);
}
