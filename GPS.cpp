#include "GPS.h"

GPS::GPS(uint8_t rxPin, uint8_t txPin) {
  gpsSerial = new SoftwareSerial(rxPin, txPin);  
}

bool GPS::begin(uint8_t baudrate = 9600) {
  gpsSerial->begin(baudrate);
}

// Read one line from the gpsSerial object
void GPS::read() {
  uint8_t cur;
  raw = "";
  while (gpsSerial->available()) {
    cur++;
    raw += gpsSerial->read(); 
  }
}
