#if !defined(SDCARD_H)
#define SDCARD_H
#include <Arduino.h>
#include <SD.h>

class SDCard {
private:
  File radio_log_file; // Log every radio message to this file

public:
  SDCard();
  ~SDCard();

  String radio_log_buffer;

  bool begin(); // Call the SD.begin method and return

  bool open_radio_log_file(); // Open the SDCard::radio_log_file File
  void tick(); // Check if we can write enough to clear the whole buffer and if so write the buffer to the file and clear the buffer
};

#endif
