#include "SpiBus.h"


SpiBus::SpiBus() {
    _spi_device_file_index = -1; // -1 is an invalid index.
}


SpiBus::~SpiBus() {
    
}



// Setters and getters.

void SpiBus::set_spi_device_file_index( int new_index ) {
    _spi_device_file_index = new_index;
}

int  SpiBus::get_spi_device_file_index() const {
    return _spi_device_file_index;
}
