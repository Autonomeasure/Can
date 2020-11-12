#include <Arduino.h>
#include "BMP280.h"

BMP280::BMP280(float seaLevelhPa) {
    bmp = new Adafruit_BMP280();
    this->seaLevelhPa = seaLevelhPa;
}

BMP280::~BMP280() {

}

// Call Adafruit_BMP280::begin function and return the method
bool BMP280::begin() {
    return bmp->begin();
}

// Read the temperature from the BMP sensor
float BMP280::getTemperature() {
    return bmp->readTemperature();
}

// Read the pressure from the BMP sensor
float BMP280::getPressure() {
    return bmp->readPressure();
}

// Get the altitude from the BMP sensor with seaLevelhPa
float BMP280::getAltitude() {
    return bmp->readAltitude(seaLevelhPa);
}