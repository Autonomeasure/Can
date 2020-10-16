#if !defined(CAN_H)
#define CAN_H
#endif
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include "Adafruit_BMP280.h"
#include "Adafruit_MPU6050.h"

struct Location {
  float lat;
  float lon;
};

struct Vector3 {
  int16_t x;
  int16_t y;
  int16_t z;
};


class Can {
private:
  

public:
  Can(int r_rx, int r_tx, int r_baudrate, int D_BMP_SDA, int D_BMP_SCL, int g_baudrate, int g_rx, int g_tx, int D_MPU_SDA, int D_MPU_SCL);
  Can(uint8_t g_rx, uint8_t g_tx, uint8_t g_baudrate);
  Location          getLocation();
  void              getGy(Vector3* a, Vector3* gy);

  SoftwareSerial*   radio;
  SoftwareSerial*   gpsSerial;
  TinyGPS*          gps;
  Adafruit_BMP280*  bmp;
  Adafruit_MPU6050* mpu;
};
