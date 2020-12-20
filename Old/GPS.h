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
#if !defined(GPS_H)
#define GPS_H
#include <SoftwareSerial.h>
#include "TinyGPS.h"

class GPS {
private:
  SoftwareSerial *gpsSerial;

public:
  GPS(uint8_t rx_pin, uint8_t tx_pin);

  TinyGPS *gps;

  void  read();                                 // Read gpsSerial and encode it (using gps.encode)
  char  *get_time();                            // Get the time and put it in the char array, format: HHMMSSmm (length = 4 bytes)
  void  get_position(float *flat, float *flon); // Get the current location of the can
  float get_altitude();                         // Get the current altitude
  float get_velocity();                         // Get the current velocity in m/s

  bool  begin(uint8_t baudrate = 9600);
};
#endif
