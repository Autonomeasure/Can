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
#include "SDCard.h"

SDCard::SDCard() {
  // Reserve enough memory for the buffer (is 512 bytes enough?)
  radio_log_buffer.reserve(512);
  test_buffer.reserve(512);
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

// Open the SDCard::test File
bool SDCard::open_test_file() {
  radio_log_file = SD.open("test.log", FILE_WRITE);
  return radio_log_file ? true : false;
}

void SDCard::write_to_test() {
  unsigned int test_chunk_size = test.availableForWrite();
  if (test_chunk_size && test_buffer.length() >= test_chunk_size) {
    test.write(test_buffer.c_str(), test_chunk_size);

    // Flush the file so everything will be saved
    test.flush();

    test_buffer.remove(0, test_chunk_size);
    free(test_chunk_size);
  }
}

// Check if we can write enough to clear the whole buffer and if so write the buffer to the file and clear the buffer
void SDCard::tick() {
  unsigned int radio_log_chunk_size = radio_log_file.availableForWrite();
  if (radio_log_chunk_size && radio_log_buffer.length() >= radio_log_chunk_size) {
    // All good, write to the file
    radio_log_file.write(radio_log_buffer.c_str(), radio_log_chunk_size);

    // Flush the file so everything will be saved
    radio_log_file.flush();

    // Remove the written data from the buffer
    radio_log_buffer.remove(0, radio_log_chunk_size);
    // Free everything that is not necessary anymore
    free(radio_log_chunk_size);
  }
}
