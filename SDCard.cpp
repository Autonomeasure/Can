#include "SDCard.h"

SDCard::SDCard() {
  // Reserve enough memory for the buffer (is 512 bytes enough?)
  radio_log_buffer.reserve(512);
}

SDCard::~SDCard() {
  
}

// Call the SD.begin method and return
bool SDCard::begin() {
  return SD.begin();
}

// Open the SDCard::radio_log_file File
bool SDCard::open_radio_log_file() {
  radio_log_file = SD.open("radio.log", FILE_WRITE);
  return radio_log_file ? true : false;
}

// Check if we can write enough to clear the whole buffer and if so write the buffer to the file and clear the buffer
void SDCard::tick() {
  unsigned int radio_log_chunk_size = radio_log_file.availableForWrite();
  if (radio_log_chunk_size && radio_log_buffer.length() >= radio_log_chunk_size) {
    // All good, write to the file
    radio_log_file.write(radio_log_buffer.c_str(), radio_log_chunk_size);

    // Remove the written data from the buffer
    radio_log_buffer.remove(0, radio_log_chunk_size);
    // Free everything that is not necessary anymore
    free(radio_log_chunk_size);
  }
}
