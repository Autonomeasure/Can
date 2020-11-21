/*
 * File: GPS.h
 * Class: GPS
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
  char  *get_time();                            // Get the time and put it in the char array, format: HHMMSSmm (length = 8)
  void  get_position(float *flat, float *flon); // Get the current location of the can
  float get_altitude();                         // Get the current altitude
  float get_velocity();                         // Get the current velocity in m/s

  bool  begin(uint8_t baudrate = 9600);
};
#endif
