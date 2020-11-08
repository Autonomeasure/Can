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

// float is 32 bits
// 3 * 32 = 96 bits
// Let's say we read the acceleration every 100 ms
// And we want to remember the last 3 seconds
// 10 * 3 = 30 measurements
// 30 * 96 = 2.880 bits = 360 bytes
struct Vector3 {
  float x;
  float y;
  float z;
};

Vector3 minus(Vector3 *a, Vector3 *b);

class Can {
private:
  void      add_acceleration(Vector3 *a);
  void      debug(Location loc, Vector3 *a, Vector3 *gy, float pressure, float temperature);
  Vector3*  acceleration_difference();

public:
  Can(uint8_t r_rx, uint8_t r_gx, uint8_t r_baudrate, uint8_t g_rx, uint8_t g_tx, uint8_t g_baudrate);
  uint8_t   begin();
  Location  getLocation();
  void      getGy(Vector3 *a, Vector3 *gy);
  void      tick();

  // The state the can is in:
  // 0: Default, not deployed
  // 1: Deployed (falling down)
  // 2: Deployed, parachute deployed
  // 3: Landed
  // 4: Landed, deploying solar cells
  // 5: Landed, solar cells deployed, all good
  uint8_t UI8_STAGE;
  Vector3 acceleration_history[30]; // This array has the acceleration of the past 3 seconds, calculate the difference to check if the parachutes were deployed properly/if the can has landed properly

  SoftwareSerial    *radio;
  SoftwareSerial    *gpsSerial;
  TinyGPS           *gps;
  Adafruit_BMP280   *bmp;
  Adafruit_MPU6050  *mpu;
};
