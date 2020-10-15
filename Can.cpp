#include "Can.h"
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include "Adafruit_BMP280.h"
#include "Adafruit_MPU6050.h"

Can::Can(uint8_t r_rx, uint8_t r_tx, int r_baudrate, int g_rx, int g_tx, int g_baudrate) {
  // The Can constructor
  // Re-enable this code when the A4/A5 bug is fixed
//  radio = new SoftwareSerial(r_rx, r_tx);
//  radio->begin(r_baudrate);
//  gpsSerial = new SoftwareSerial(g_rx, g_tx);
//  gpsSerial->begin(g_baudrate);
//  gps = new TinyGPS();
//  bmp = new Adafruit_BMP280();

  mpu = new Adafruit_MPU6050();

  mpu->setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu->setGyroRange(MPU6050_RANGE_250_DEG);
  mpu->setFilterBandwidth(MPU6050_BAND_21_HZ);
}

Location Can::getLocation() {
  // Get the location from the GPS module and put it in a Location object and return that object
  if (gpsSerial->available()) {
    if (gps->encode(gpsSerial->read())) {
      Location location;
      gps->f_get_position(&location.lat, &location.lon);
      return location;
    }
  }
}

void Can::getGy(Vector3* a, Vector3* gy) {
  sensors_event_t at, gt, temp;
  mpu->getEvent(&at, &gt, &temp);

  a->x = at.acceleration.x;
  a->y = at.acceleration.y;
  a->z = at.acceleration.z;
  gy->x = gt.gyro.x;
  gy->y = gt.gyro.y;
  gy->z = gt.gyro.z;
}
