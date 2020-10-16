#include "Can.h"

//#define D_MPU_SCL 1
//#define D_MPU_SDA 2
//#define D_BMP_SCL 3
//#define D_BMP_SDA 4

const int D_MPU_SCL = 1;
const int D_MPU_SDA = 2;
const int D_BMP_SCL = 3;
const int D_BMP_SDA = 4;

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

//Can can(2, 3, 9600, 1, 2, 9600, 4, 3 3, 4);
Can can (4, 3, 9600);



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
//  Vector3 a;
//  Vector3 gy;
//  can.getGy(&a, &gy);
//  Serial.print("Accelerometer: ");
//  Serial.print("x = ");
//  Serial.print(a.x);
//  Serial.print(" | y = ");
//  Serial.print(a.y);
//  Serial.print(" | z = ");
//  Serial.println(a.z);
//
//  Serial.print("Gyroscope: ");
//  Serial.print("x = ");
//  Serial.print(gy.x);
//  Serial.print(" | y = ");
//  Serial.print(gy.y);
//  Serial.print(" | z = ");
//  Serial.println(gy.z);
  Location loc = can.getLocation();
  Serial.print("Lat: ");
  Serial.println(loc.lat);
  Serial.print("Lon: ");
  Serial.println(loc.lon);
  delay(200);
}
