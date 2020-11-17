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

public:
  char            raw[82] = "$GPGGA,170834,4124.8963,N,08151.6838,W,1,05,1.5,280.2,M,-34.0,M,,,*75\n\r\0\0\0\0\0\0\0\0\0\0"; // A NMEA_0183 message has a maxium of 82 characters
  GPS(uint8_t rxPin, uint8_t txPin);

  bool    begin(uint8_t baudrate = 9600);
  
  uint8_t read(char *buf); // Read one line from the gpsSerial object and put it into the char array pointer and return the length of the message
  bool    decode(NMEA *nmea, char *buf, uint8_t len); // Decode the raw NMEA sentence to an NMEA object
  
  bool    decode(NMEA *nmea); // Decode the raw NMEA string to an NMEA object
};
#endif
