#include "Can.h"

Can::Can(float seaLevelhPa, uint8_t radio_rx, uint8_t radio_tx, uint8_t gps_rx, uint8_t gps_tx) {
    radio = new SoftwareSerial(radio_rx, radio_tx);
    bmp = new BMP280(seaLevelhPa);
    mpu = new MPU();
}

Can::~Can() {

}

// Call all the begin methods and return a boolean to check if something failed
bool Can::begin() {
    radio->begin(9600);
    bool m = mpu->begin();
    bool b = bmp->begin();
    return b && m;
}

// This method gets called every 100ms (at least)
void Can::tick() {
    // Read all the sensor values...
    // Send a radio message with all the data
    // Check if parachutes need to be deployed
    // Check if parachutes are deployed successful if needed
    // Check if we have landed yet
}

// Set the state of the Can::STATE variable and set it in the EEPROM memory
bool Can::setState(uint8_t state) {
    // Check if the new state is lower than the last state (so we're going back??) if so return true
    if (state < STATE) {
        STATE = state;
        return true;
    }
    STATE = state;
    EEPROM.put(STATE_ADDRESS, state);
    return false;
}

// Get Can::STATE
uint8_t Can::getState() {
    if (STATE == 0b00000000) {
      // STATE is not set, get it from the EEPROM (and if that is empty set both to 00000001)
      EEPROM.get(STATE_ADDRESS, STATE);
      if (STATE == 0b00000000) {
        // STATE is still equal to 0, set both to 00000001
        setState(0b0000001);
      }
    }
    return STATE;
}

// Get the acceleration from the MPU module
void Can::getAcceleration(Vector3 *a) {
    mpu->getGy(a, nullptr, nullptr);
}

// Get the gyroscope data from the MPU module
void Can::getGy(Vector3 *gy) {
    mpu->getGy(nullptr, gy, nullptr);
}

// Get the temperatures from the BMP and MPU module
void Can::getTemperature(float *bmpTemp, float *mpuTemp) {
    bmp->getTemperature(bmpTemp);
    mpu->getGy(nullptr, nullptr, mpuTemp);
}

// Get the altitude from the BMP module
void Can::getAltitude(float *altitude) {
    bmp->getAltitude(altitude);

    // Shift everything to the left
    memcpy(altitude_history, &altitude_history[1], sizeof(altitude_history) / sizeof(float));
    altitude_history[sizeof(altitude_history) / sizeof(float) - 1] = *altitude;
}
