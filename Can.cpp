#include "Can.h"
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include "Adafruit_BMP280.h"

Can::Can(uint8_t r_rx, uint8_t r_tx, int r_baudrate, int g_rx, int g_tx, int g_baudrate) {
  // The Can constructor
  // Re-enable this code when the A4/A5 bug is fixed
//  radio = new SoftwareSerial(r_rx, r_tx);
//  radio->begin(r_baudrate);
//  gpsSerial = new SoftwareSerial(g_rx, g_tx);
//  gpsSerial->begin(g_baudrate);
//  gps = new TinyGPS();
//  bmp = new Adafruit_BMP280();

  // Setup the accelerometer/gyroscope
  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x6b);
  Wire.write(0);
  Wire.endTransmission(true);
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
  // Get the acceleration from the gy module
  Wire.beginTransmission(0x68);
  Wire.write(0x3b);
  Wire.endTransmission(true);
  Wire.requestFrom(0x68, 12, true);

//  Vector3 a;
  a->x = Wire.read() << 8 | Wire.read();
  a->y = Wire.read() << 8 | Wire.read();
  a->z = Wire.read() << 8 | Wire.read();

//  Vector3 gy;
  gy->x = Wire.read() << 8 | Wire.read();
  gy->y = Wire.read() << 8 | Wire.read();
  gy->z = Wire.read() << 8 | Wire.read();

//  return {a, gy};
}
