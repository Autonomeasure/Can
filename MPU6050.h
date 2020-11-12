#if !defined(MPU6050_H)
#define MPU6050_H
#endif
#include <Arduino.h>
#include "Adafruit_MPU6050.h"

struct Vector3 {
  float x;
  float y;
  float z;
};

class MPU6050 {
private:
    Adafruit_MPU6050 *mpu;
public:
    MPU6050();
    ~MPU6050();

    bool begin(); // Call the Adafruit_MPU6050::begin method and return
    void getGy(Vector3 *a, Vector3 *gy, float *temp); // Get the acceleration, gyro data and temperature
};