#include "I2CBus.h"


I2CBus::I2CBus() {
    _i2c_device_file_index = -1; // -1 is an invalid index
}

I2CBus::~I2CBus() {

}


// Setters and getters.

void I2CBus::set_i2c_device_file_index( int new_index ) {
    _i2c_device_file_index = new_index;
}

int  I2CBus::get_i2c_device_file_index() const {
    return _i2c_device_file_index;
}
