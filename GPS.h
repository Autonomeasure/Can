#if !defined(GPS_H)
#define GPS_H
#include <SoftwareSerial.h>

struct Location {
  
};

class GPS {
private:
  SoftwareSerial *gpsSerial;
  String          raw;

public:
  GPS(uint8_t rxPin, uint8_t txPin);

  bool begin(uint8_t baudrate = 9600);
  void read(); // Read one line from the gpsSerial object
};
#endif
