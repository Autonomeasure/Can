#include <Arduino.h>
#include "MPU6050.h"

MPU::MPU() {
    mpu = new Adafruit_MPU6050();
    mpu->setAccelerometerRange(MPU6050_RANGE_16_G);
	mpu->setGyroRange(MPU6050_RANGE_250_DEG);
	mpu->setFilterBandwidth(MPU6050_BAND_21_HZ);
}

MPU::~MPU() {

}

// Call the Adafruit_MPU::begin method and return
bool MPU::begin() {
    return mpu->begin();
}

// Get the acceleration, gyro data and temperature
void MPU::getGy(Vector3 *a, Vector3 *gy, float *temp) {
    sensors_event_t at, gt, t;
	mpu->getEvent(&at, &gt, &t);

	a->x = at.acceleration.x;
	a->y = at.acceleration.y;
	a->z = at.acceleration.z;
	gy->x = gt.gyro.x;
	gy->y = gt.gyro.y;
	gy->z = gt.gyro.z;
    *temp = t.temperature;
}
