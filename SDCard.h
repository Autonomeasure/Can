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
#if !defined(SDCARD_H)
#define SDCARD_H
#include <Arduino.h>
#include <SD.h>

class SDCard {
private:
  File radio_log_file; // Log every radio message to this file

public:
  SDCard();

  File test;
  
  String test_buffer;
  String radio_log_buffer;

  bool begin(); // Call the SD.begin method and return

  bool open_radio_log_file(); // Open the SDCard::radio_log_file File
  bool open_test_file();      // Open the SDCard::test File

  void write_to_test();
  
  void tick(); // Check if we can write enough to clear the whole buffer and if so write the buffer to the file and clear the buffer
};

#endif
