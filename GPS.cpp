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

// Call the begin function and set the UART baudrate
void GPS::begin(uint8_t baudrate = 9600) {
	gpsSerial->begin(baudrate);
}

// Read gpsSerial and encode it using gps::encode
void GPS::read() {
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

// Get the current ground speed in m/s
uint8_t GPS::get_ground_speed(Error *errors, double *speed) {
  uint8_t amountOfErrors;
	if (!gps.speed.isValid()) {
		Error err;
    err.errorID = 7;
    errors[amountOfErrors] = err;
    return amountOfErrors;
	}

	*speed = gps.speed.mps();
  return amountOfErrors;
}
