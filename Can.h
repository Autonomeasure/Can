#if !defined(CAN_H)
#define CAN_H
#include <Arduino.h>

#include "Vector3.h"
#include "BMP280.h"
#include "MPU6050.h"
#include "Radio.h"

class Can {
private:
    BMP280 *bmp;
    MPU6050 *mpu;
    
    // The state of the can:
    // 00000000 (0): Nothing yet..
    // 00000001 (1): Flight in progress
    // 00000011 (3): Flight in progress, parachute should get deployed
    // 00000111 (7): Flight in progress, parachute deployed succesfully
    // 00001110 (14): Landing successful
    uint8_t STATE;

public:
    Can(float seaLevelhPa);
    ~Can();

    float altitude_history[10];

    bool    setState(uint8_t state); // Set the state of the Can::STATE variable
    uint8_t getState(); // Get Can::STATE
    bool    begin(); // Call all the begin methods and return a boolean to check if something failed
    void    tick(); // This method gets called every 100ms

    void    getAcceleration(Vector3 *a); // Get the acceleration from the MPU module
    void    getGy(Vector3 *gy); // Get the gyroscope data from the MPU module
    void    getTemperature(float *bmpTemp, float *mpuTemp); // Get the temperatures from the BMP and MPU module
    void    getAltitude(float *altitude); // Get the altitude from the BMP module
};
#endif
