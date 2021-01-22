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
#include "GPS.h"

// -============= PUBLIC FUNCTIONS =============-
// The constructor, sets the gps serial object
GPS::GPS(HardwareSerial *gpsSerial) {
	// Set GPS.gpsSerial 
	this->gpsSerial = gpsSerial;
}

// Call the begin function of the gpsSerial object and sets the UART baudrate and set the rate at which we will receive GPS data
void GPS::begin(uint8_t rate = 5) {
  // Set the baudrate to 4800 so we can communicate first with the GPS module
	gpsSerial->begin(4800);

  // Set the update frequency
  String sentence = "PCAS02," + String(1000 / rate);
  // Calculate the checksum
  int checksum = 0;
  for (uint8_t i = 0; i < sentence.length(); i++) {
    checksum ^= uint8_t(sentence[i]);
  }
  // Transmit the message
  gpsSerial->println("$" + sentence + "*" + checksum);

  // Set the correct baudrate if necessary 
  if (rate > 1) {
    // Set the UART baudrate of the GPS module to 19200
    gpsSerial->println("$PCAS01,2*1E");

    // End the serial connection and start a new one with the correct baudrate
    gpsSerial->end();
    gpsSerial->begin(19200);
  }
}

// Read gpsSerial and encode it using gps::encode
void GPS::read(void) {
	while (gpsSerial->available()) {
		gps.encode(gpsSerial->read());
	}
}

// Get the current time (hMsm (four chars)
uint8_t GPS::get_time(Error *errors, char *time) {
  uint8_t amountOfErrors;
	if (!gps.time.isValid()) {
    Error err;
    err.errorID = 4;
    errors[amountOfErrors] = err;
    return amountOfErrors;
  }

	time[0] = gps.time.hour();
	time[1] = gps.time.minute();
	time[2] = gps.time.second();
	time[3] = gps.time.centisecond();
	return amountOfErrors;
}

// Get the current location of the Can
uint8_t GPS::get_position(Error *errors, double *lat, double *lon) {
  uint8_t amountOfErrors;
	if (!gps.location.isValid()) {
		Error err;
    err.errorID = 5;
    errors[amountOfErrors] = err;
    return amountOfErrors;
	}

	*lat = gps.location.lat();
	*lon = gps.location.lng();
  return amountOfErrors;
}

// Get the current altitude in meters
uint8_t GPS::get_altitude(Error *errors, double *altitude) {
  uint8_t amountOfErrors;
	if (!gps.altitude.isValid()) {
		Error err;
    err.errorID = 6;
    errors[amountOfErrors] = err;
    return amountOfErrors;
	}

	*altitude = gps.altitude.meters();
  return amountOfErrors;
}

//// Get the current ground speed in m/s
//uint8_t GPS::get_ground_speed(Error *errors, double *speed) {
//  uint8_t amountOfErrors;
//	if (!gps.speed.isValid()) {
//		Error err;
//    err.errorID = 7;
//    errors[amountOfErrors] = err;
//    return amountOfErrors;
//	}
//
//	*speed = gps.speed.mps();
//  return amountOfErrors;
//}
