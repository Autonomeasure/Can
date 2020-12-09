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
#include "Can.h"

Can::Can(float seaLevelhPa, uint8_t radio_rx, uint8_t radio_tx, uint8_t gps_rx, uint8_t gps_tx) {
  radio = new SoftwareSerial(radio_rx, radio_tx);
  bmp = new Adafruit_BMP280();
  mpu = new Adafruit_MPU6050();
  mpu->setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu->setGyroRange(MPU6050_RANGE_250_DEG);
  mpu->setFilterBandwidth(MPU6050_BAND_21_HZ);
}

// Call all the begin methods and return a boolean to check if something failed
bool Can::begin() {
    radio->begin(9600);
    bool m = mpu->begin();
    bool b = bmp->begin();
    Serial.print("m: ");
    Serial.println(m);
    Serial.print("b: ");
    Serial.println(b); 
    return b && m;
}

// Configure the APC220 module to the right settings
void Can::configureRadio() {
  // The APC220 setPin is D38
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW); // Set the APC220 in config mode
  delay(50);

  // Frequency: 434000 kHz
  // Air rate is 2400 bps = 1
  // Output power = 4
  // UART baudrate = 3
  // Byte check parity = 0

  radio->print("WR 434000 1 4 3 0");
  radio->write(0x0D);
  radio->write(0x0A);
  delay(10);

  digitalWrite(38, HIGH); // Set the APC220 in "production" mode
}


// This method gets called every 100ms (at least)
void Can::tick() {
    // Read all the sensor values...
    // Send a radio message with all the data
    // Check if parachutes need to be deployed
    // Check if parachutes are deployed successful if needed
    // Check if we have landed yet

    float mpuTemp, bmpTemp, pressure;
    getBMPPressure(&pressure);
    getBMPTemperature(&bmpTemp);

    Vector3 a, g;

    getGy(&a, &g, &mpuTemp);

    String msg;
    msg += "id="; msg += id; msg += ";";
    msg += "p="; msg += pressure; msg += ";";
    msg += "tb="; msg += bmpTemp; msg += ";";
    msg += "tm="; msg += mpuTemp; msg += ";";
    msg += "ax="; msg += a.x; msg += ";";
    msg += "ay="; msg += a.y; msg += ";";
    msg += "az="; msg += a.z; msg += ";";
    msg += "gx="; msg += g.x * (180 / PI); msg += ";";
    msg += "gy="; msg += g.y * (180 / PI); msg += ";";
    msg += "gz="; msg += g.z * (180 / PI); msg += ";";
    radio->println(msg);
   Serial.println(msg);
    id++;
    
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

// Get the data (acceleration, gyroscope and temperature) data from the MPU6050 module
void Can::getGy(Vector3 *a, Vector3 *gy, float *temp) {
  sensors_event_t at, gt, t;
  mpu->getEvent(&at, &gt, &t);

  a->x = at.acceleration.x;
  a->y = at.acceleration.y;
  a->z = at.acceleration.z;
  gy->x = gt.gyro.x;
  gy->y = gt.gyro.y;
  gy->z = gt.gyro.z;
  *temp = t.temperature;
}

// Get the temperature data from the BMP280 module
void Can::getBMPTemperature(float *temperature) {
  float temp = bmp->readTemperature();
  *temperature = temp;  
}

// Get the pressure data from the BMP280 module
void Can::getBMPPressure(float *pressure) {
  float p = bmp->readPressure();
    *pressure = p;
}
