#if !defined(MPU6050_H)
#define MPU6050_H
#include <Arduino.h>
#include "Adafruit_MPU6050.h"
#include "Vector3.h"

class MPU {
private:
    Adafruit_MPU6050 *mpu;
public:
    MPU();
    ~MPU();

    bool begin(); // Call the Adafruit_MPU6050::begin method and return
    void getGy(Vector3 *a, Vector3 *gy, float *temp); // Get the acceleration, gyro data and temperature
};
#endif
