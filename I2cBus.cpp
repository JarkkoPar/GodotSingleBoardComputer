#include "I2cBus.h"


I2cBus::I2cBus() {
    _i2c_device_file_index = -1; // -1 is an invalid index
    _gpio_reserved_pins = nullptr;
    _num_gpio_reserved_pins = 0;
}

I2cBus::~I2cBus() {
    if( _gpio_reserved_pins != nullptr ) {
        delete[] _gpio_reserved_pins;
        _gpio_reserved_pins = nullptr;
    }
    _num_gpio_reserved_pins = 0;
}


// Setters and getters.

void I2cBus::set_i2c_device_file_index( int new_index ) {
    _i2c_device_file_index = new_index;
}

int  I2cBus::get_i2c_device_file_index() const {
    return _i2c_device_file_index;
}


void I2cBus::add_gpio_reserved_pin( int new_pin_index ) {
    // Simply create a new array and copy over the
    // previously added pins to the new one, and then
    // delete the old array.
    int* new_array = new int[_num_gpio_reserved_pins + 1];
    if( new_array == nullptr ) {
        // todo: notify error
        return;
    }
    for( int i = 0; i < _num_gpio_reserved_pins; ++i ) {
        new_array[i] = _gpio_reserved_pins[i];
    }

    if( _gpio_reserved_pins != nullptr ) {
        delete[] _gpio_reserved_pins;
        _gpio_reserved_pins = nullptr;
    }

    _gpio_reserved_pins = new_array;
    _gpio_reserved_pins[_num_gpio_reserved_pins] = new_pin_index;
    _num_gpio_reserved_pins += 1;
}

int* I2cBus::get_gpio_reserved_pins() const {
    return _gpio_reserved_pins;
}