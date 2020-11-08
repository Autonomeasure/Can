#include "Can.h"
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include "Adafruit_BMP280.h"
#include "Adafruit_MPU6050.h"

Vector3 minus(Vector3* a, Vector3* b) {
	Vector3 ret;
	ret.x = a->x - b->x;
	ret.y = a->y - b->y;
	ret.z = a->z - b->z;
	return ret;
}

Can::Can(uint8_t r_rx, uint8_t r_gx, uint8_t r_baudrate, uint8_t g_rx, uint8_t g_tx, uint8_t g_baudrate) {
	UI8_STAGE = 0;
	//  radio = new SoftwareSerial(r_rx, r_tx);
	//  radio->begin(r_baudrate);
	//  gpsSerial = new SoftwareSerial(g_rx, g_tx);
	//  gpsSerial->begin(g_baudrate);
	//  gps = new TinyGPS();
	//
	//  mpu = new Adafruit_MPU6050();
	//  mpu->setAccelerometerRange(MPU6050_RANGE_16_G);
	//  mpu->setGyroRange(MPU6050_RANGE_250_DEG);
	//  mpu->setFilterBandwidth(MPU6050_BAND_21_HZ);

	// Vecor3 test[30];
	for (uint8_t i = 0; i < 30; i++) {
		Vector3 t;
		t.x = i;
		t.y = i;
		t.z = i;
		// test[i] = t;
		Can::acceleration_history[i] = t;
		// add_acceleration(&t);
	}

	bmp = new Adafruit_BMP280();
}

// PRIVATE FUNCTIONS
void Can::add_acceleration(Vector3 *a) {
	// First shift all the values one to the left
	memcpy(acceleration_history, &acceleration_history[1], sizeof(acceleration_history) / sizeof(Vector3));

	// Put the new value in the array at the last element
	acceleration_history[sizeof(acceleration_history) / sizeof(Vector3) - 1] = *a;
}

void Can::debug(Location loc, Vector3 *a, Vector3 *gy, float pressure, float temperature) {
	Serial.print("Lat: ");
	Serial.println(loc.lat);
	Serial.print("Lon: ");
	Serial.println(loc.lon);

	Serial.print("Accelerometer: ");
	Serial.print("x = ");
	Serial.print(a->x);
	Serial.print(" | y = ");
	Serial.print(a->y);
	Serial.print(" | z = ");
	Serial.println(a->z);

	Serial.print("Gyroscope: ");
	Serial.print("x = ");
	Serial.print(gy->x);
	Serial.print(" | y = ");
	Serial.print(gy->y);
	Serial.print(" | z = ");
	Serial.println(gy->z);

	Serial.print("Pressure: ");
	Serial.println(pressure);

	Serial.print("Temperature: ");
	Serial.println(temperature);
}

// This method returns a pointer to an Vector3 array with the difference
Vector3 * Can::acceleration_difference() {
	Vector3 ret[30];
	for (uint8_t i = 0; i < sizeof(Can::acceleration_history) / sizeof(Vector3); i++) {
		if (i == 0) {
			Vector3 temp;
			temp.x = 0;
			temp.y = 0;
			temp.z = 0;
			ret[i] = temp;
		}
		else {
			ret[i] = minus(&Can::acceleration_history[i], &Can::acceleration_history[i - 1]);
		}
	}

	return ret;
}

// PUBLIC FUNCTIONS
uint8_t Can::begin() {
	// if (!mpu->begin()) {
	// 	return 1;
	// } if (!bmp->begin()) {
	// 	return 2;
	// }
	// return 0;
	Serial.println("HISTORY");
	for (uint8_t i = 0; i < sizeof(Can::acceleration_history) / sizeof(Vector3); i++) {
		Vector3 *a = &Can::acceleration_history[i];
		Serial.print("x = ");
		Serial.print(a->x);
		Serial.print(" | y = ");
		Serial.print(a->y);
		Serial.print(" | z = ");
		Serial.println(a->z);
	}

	Serial.println("DIFFERENCE");
	Vector3 *test = acceleration_difference();
	for (uint8_t i = 0; i < 30; i++) {
		Vector3 *a = test + i;
		Serial.print("x = ");
		Serial.print(a->x);
		Serial.print(" | y = ");
		Serial.print(a->y);
		Serial.print(" | z = ");
		Serial.println(a->z);
	}
	return 0;
}

Location Can::getLocation() {
	// Get the location from the GPS module, put it in a Location object and return that object
	if (gpsSerial->available()) {
		if (gps->encode(gpsSerial->read())) {
			Location location;
			gps->f_get_position(&location.lat, &location.lon);
			return location;
		}
	}
}

void Can::getGy(Vector3 *a, Vector3 *gy) {
	sensors_event_t at, gt, temp;
	mpu->getEvent(&at, &gt, &temp);

	a->x = at.acceleration.x;
	a->y = at.acceleration.y;
	a->z = at.acceleration.z;
	gy->x = gt.gyro.x;
	gy->y = gt.gyro.y;
	gy->z = gt.gyro.z;
}

// This function gets called every 100ms
// It sends a radio message with all the data
void Can::tick() {
	// Get all the data from the sensors
	// Location loc = getLocation();
	// Vector3 *a;
	// Vector3 *gy;
	// getGy(a, gy);
	float pressure = bmp->readPressure();
	float temperature = bmp->readTemperature();

	// debug(loc, a, gy, pressure, temperature);

	// Vector3 *aDiff = Can::acceleration_difference();
	// for (uint8_t i = 0; i < 30; i++) {
	// 	Vector3 t = *(aDiff + i);
	// 	Serial.println(*t);
	// }
}
