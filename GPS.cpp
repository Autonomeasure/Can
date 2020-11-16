#include "GPS.h"

GPS::GPS(uint8_t rxPin, uint8_t txPin) {
  gpsSerial = new SoftwareSerial(rxPin, txPin);  
}

bool GPS::begin(uint8_t baudrate = 9600) {
  gpsSerial->begin(baudrate);
}

// Read one line from the gpsSerial object
uint8_t GPS::read() {
  uint8_t cur;
  // Clear the GPS.raw array
  memset(raw, 0, sizeof(raw));
  // Read the serial line and put it in the right place in GPS.raw
  while (gpsSerial->available()) {
    raw[cur] = gpsSerial->read();
    if (raw[cur] == '\n') {
      if (raw[cur - 1] == '\r') {
        // End of the NMEA sentence
        return cur;
      }
    }
    cur++;
  }
  return sizeof(raw) / sizeof(char);
}

// Decode the raw NMEA string to an NMEA object
bool GPS::decode(NMEA *nmea) {
  // Check if this is a valid sentence
  if (raw[0] != '$') return false;

  // Split at every comma
}
