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
// The constructor
GPS::GPS(HardwareSerial *gpsSerial) {
	// Set GPS.gpsSerial 
	this->gpsSerial = gpsSerial;
}

// Call the begin function and set the UART baudrate
void GPS::begin(uint8_t baudrate = 9600) {
	gpsSerial->begin(baudrate);
}

// Read gpsSerial and encode it using gps.encode
void GPS::read() {
	while (gpsSerial->available()) {
		gps.encode(gpsSerial->read());
	}
}

// Get the current time (hMsm (four chars)
bool GPS::get_time(char *hMsm) {
	if (!gps.time.isValid()) {
		return false;
	}

	hMsm[0] = gps.time.hour();
	hMsm[1] = gps.time.minute();
	hMsm[2] = gps.time.second();
	hMsm[3] = gps.time.centisecond();
	return true;
}

// Get the current location of the Can
bool GPS::get_position(double *lat, double *lon) {
	if (!gps.location.isValid()) {
		return false;
	}

	*lat = gps.location.lat();
	*lon = gps.location.lng();
	return true;
}

// Get the current altitude in meters
bool GPS::get_altitude(double *altitude) {
	if (!gps.altitude.isValid()) {
		return false;
	}

	*altitude = gps.altitude.meters();
	return true;
}

// Get the current ground speed in m/s
bool GPS::get_ground_speed(double *speed) {
	if (!gps.speed.isValid()) {
		return false;
	}

	*speed = gps.speed.mps();
	return true;
}

// Get the current air speed in m/s
bool GPS::get_air_speed(double *speed) {
	
}