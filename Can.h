#if !defined(CAN_H)
#define CAN_H
#endif
#include <Arduino.h>
#include "BMP280.h"
#include "MPU6050.h"

class Can {
private:
    BMP280 *bmp;
    MPU6050 *mpu;
    
    // The state of the can:
    // 00000001 (1): Flight in progress
    // 00000011 (3): Flight in progress, parachute should get deployed
    // 00000111 (7): Flight in progress, parachute deployed succesfully
    // 00001110 (14): Landing successful 

    uint8_t STATE;

public:
    Can(float seaLevelhPa);
    ~Can();
    bool    begin(); // Call all the begin methods and return a boolean to check if something failed
    void    tick(); // This method gets called every 100ms
};