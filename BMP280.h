#if !defined(BMP280_H)
#define BMP280_H
#endif
#include <Arduino.h>
#include "Adafruit_BMP280.h"


class BMP280 {
private:
    Adafruit_BMP280 *bmp;
    float seaLevelhPa;
public:
    BMP280(float seaLevelhPa);
    ~BMP280();

    bool    begin(); // Call Adafruit_BMP280::begin function and return the method
    float   getTemperature(); // Read the temperature from the BMP sensor
    float   getPressure(); // Read the pressure from the BMP sensor
    float   getAltitude(); // Get the altitude from the BMP sensor with seaLevelhPa
};