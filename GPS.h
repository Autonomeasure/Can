/*
 * File: GPS.h
 * Class: GPS
 * 
 * Structs: NMEA (https://en.wikipedia.org/wiki/NMEA_0183)
 */
#if !defined(GPS_H)
#define GPS_H
#include <SoftwareSerial.h>

struct NMEA {
  
};

class GPS {
private:
  SoftwareSerial *gpsSerial;
  char            raw[82]; // A NMEA_0183 message has a maxium of 82 characters

public:
  GPS(uint8_t rxPin, uint8_t txPin);

  bool    begin(uint8_t baudrate = 9600);
  uint8_t read(); // Read one line from the gpsSerial object
  bool    decode(NMEA *nmea); // Decode the raw NMEA string to an NMEA object
};
#endif
