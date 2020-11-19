#include <Arduino.h>
#include <string.h>
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

  // Split at every comma
  char delim[] = ",";
  char *ptr = strtok(buf, delim);

  char temp[16];

  uint8_t i = 0;

  while(ptr != NULL) {
    sprintf(temp, "%s", ptr);
    Serial.print("i: ");
    Serial.println(i);

    if (i == 0 && strcmp(temp, "$GPGGA")) return false; // Not the message we want (with the right information) so return false
    if (i == 1) { // Time should be here in the ssmmhh format
      if (strlen(temp) < 6) return false; // Not enough chars so return false
      nmea->time.h = uint8_t(temp[0] + temp[1]);
      nmea->time.m = uint8_t(temp[2] + temp[3]);
      nmea->time.s = uint8_t(temp[4] + temp[5]);
      return true;
    }
    i++;
    ptr = strtok(NULL, delim);
  }
  return true;
}
