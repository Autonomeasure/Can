#include <Arduino.h>
#include "GPS.h"

GPS::GPS(uint8_t rxPin, uint8_t txPin) {
//  gpsSerial = new SoftwareSerial(rxPin, txPin);
}

bool GPS::begin(uint8_t baudrate = 9600) {
  gpsSerial->begin(baudrate);
}



// Read one line from the gpsSerial object and put it into the char array pointer and return the length of the message
uint8_t GPS::read(char *buf) {
  uint8_t len;
  while (gpsSerial->available()) {
    buf[len] = gpsSerial->read();
    if (buf[len] == '\r') { // \r should be the last char
      if (buf[len - 1] == '\n') { // And \n should be the char before \r
        // This is the end of the NMEA sentence
        return len;
      }
    }
    len++;
  }
}

// Decode the raw NMEA sentence to an NMEA object
bool GPS::decode(NMEA *nmea, char *buf, uint8_t len) {
  if (len < 1 || len > 82) return false; // Sentence is too short or too long
  if (buf[0] != '$') return false;       // Invalid start of the NMEA sentence
}



// Decode the raw NMEA string to an NMEA object
bool GPS::decode(NMEA *nmea) {
  // Check if this is a valid sentence
  Serial.println(raw);
  if (raw[0] != '$') return false;

  // Split at every comma
  char temp[12][32];
  uint8_t prev_i;
  uint8_t count;
  for (uint8_t i = 0; i < sizeof(raw) / sizeof(char); i++) {
    if (raw[i] == '\r') {
      if (raw[i - 1] == '\n') {
        // At the end :)
        memcpy(&temp[count], raw[prev_i], i - prev_i);
        count++;
        break;
      }
    }
    if (raw[i] == ',') {
      // Comma found, split
      memcpy(&temp[count], raw[prev_i], i - prev_i);
      count++;
    }
  }
  for (uint8_t i = 0; i < count; i++) {
    Serial.println(temp[i]);
  }
}
