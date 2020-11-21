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
void BMP280::getTemperature(float *temp) {
    float t = bmp->readTemperature();
    temp = &t;
}

// Read the pressure from the BMP sensor
void BMP280::getPressure(float *pressure) {
    float p = bmp->readPressure();
    pressure = &p;
}

// Get the altitude from the BMP sensor with seaLevelhPa
void BMP280::getAltitude(float *altitude) {
    float alt = bmp->readAltitude(seaLevelhPa);
    altitude = &alt;
}
