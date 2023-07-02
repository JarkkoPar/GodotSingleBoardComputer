#include "SPIBus.h"


SPIBus::SPIBus() {
    _spi_device_file_index = -1; // -1 is an invalid index.
}


SPIBus::~SPIBus() {
    
}



// Setters and getters.

void SPIBus::set_spi_device_file_index( int new_index ) {
    _spi_device_file_index = new_index;
}

int  SPIBus::get_spi_device_file_index() const {
    return _spi_device_file_index;
}
