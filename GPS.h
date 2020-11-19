/*
 * File: GPS.h
 * Class: GPS
 * 
 * Structs: NMEA (https://en.wikipedia.org/wiki/NMEA_0183)
 */
#if !defined(GPS_H)
#define GPS_H
#include <SoftwareSerial.h>

struct Time {
  uint8_t s;
  uint8_t m;
  uint8_t h;
};

struct NMEA {
  Time time;
};

class GPS {
private:
  SoftwareSerial *gpsSerial;

public:
  GPS(uint8_t rxPin, uint8_t txPin);

  bool    begin(uint8_t baudrate = 9600);
  
  uint8_t read(char *buf); // Read one line from the gpsSerial object and put it into the char array pointer and return the length of the message
  bool    decode(NMEA *nmea, char *buf, uint8_t len); // Decode the raw NMEA sentence to a NMEA object  
};
#endif
