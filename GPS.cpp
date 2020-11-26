/*
 * CanSat 2020-2021 Team Autonomeasure - Can
 * Code written by: Joep van Dijk
 * First mission:
 *    Our first mission is to be able to land properly, collect data from the BMP280 sensor and send that data to the groundstation at least one time per second. 
 * 
 * Second mission:
 *    Our second mission is to make a sustainable data collection station. 
 * 
 * Links:
 *    GitHub Autonomeasure:           https://github.com/Autonomeasure
 *    GitHub Can repo:                https://github.com/Autonomeasure/Can
 *    GitHub GroundStation repo:      https://github.com/Autonomeasure/GroundStation
 *    Instagram:                      https://instagram.com/Autonomeasure/
 */
#include "GPS.h"

GPS::GPS(uint8_t rx_pin, uint8_t tx_pin) {
  gpsSerial = new SoftwareSerial(rx_pin, tx_pin);
}

bool GPS::begin(uint8_t baudrate = 9600) {
  gpsSerial->begin(baudrate);
}

// Read gpsSerial and encode it (using gps.encode)
void GPS::read() {
  while (gpsSerial->available()) {
    gps->encode(gpsSerial->read());
  }
}

// Get the time and put it in the char array, format: HHMMSSmm (length = 8)
char *GPS::get_time() {
  byte hour, minute, second, hundredths;
  unsigned long age;
  gps->crack_datetime(NULL, NULL, NULL, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE) {
    return NULL;
  } else {
    char *ret;
    sprintf(ret, "%02d%02d%02d%02d", hour, minute, second, hundredths);
    return ret;
  }
}

// Get the current location of the can
void GPS::get_position(float *flat, float *flon) {
  gps->f_get_position(flat, flon);
}

// Get the current altitude
float GPS::get_altitude() {
  return gps->f_altitude();
}

// Get the current velocity in m/s
float GPS::get_velocity() {
  return gps->f_speed_mps();
}
