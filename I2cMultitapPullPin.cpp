#include "I2cMultitapPullPin.h"


using namespace godot;


I2cMultitapPullPin::I2cMultitapPullPin() {
    _singleboardcomputer = nullptr;
}

I2cMultitapPullPin::~I2cMultitapPullPin() {
    _singleboardcomputer = nullptr;
}


void I2cMultitapPullPin::_bind_methods() {

}


bool I2cMultitapPullPin::_configure_i2c_device() {


    return true;
}


// Gpio related handers.
int I2cMultitapPullPin::get_num_gpio_pins() const {
    if( _singleboardcomputer == nullptr ) return 0;
    return _singleboardcomputer->get_num_gpio_pins();
}

GpioPin* I2cMultitapPullPin::get_gpio_pins() const {
    if( _singleboardcomputer == nullptr ) return nullptr;
    return _singleboardcomputer->get_gpio_pins();
}

int I2cMultitapPullPin::get_gpio_pin_offset(int pin_index) const {
    if( _singleboardcomputer == nullptr ) return 0;
    return _singleboardcomputer->get_gpio_pin_offset(pin_index);
}


// I2C related handlers.
int I2cMultitapPullPin::get_num_i2c_buses() const {
    if( _singleboardcomputer == nullptr ) return 0;
    return _singleboardcomputer->get_num_i2c_buses();
}

I2cBus* I2cMultitapPullPin::get_i2c_bus( int bus_index ) const {
    if( _singleboardcomputer == nullptr ) return nullptr;
    return _singleboardcomputer->get_i2c_bus( bus_index );
}


// File handle management.
int I2cMultitapPullPin::request_i2c_device_file( int bus_index ) {
    if( _singleboardcomputer == nullptr ) return 0;
    return _singleboardcomputer->request_i2c_device_file( bus_index );
}

int I2cMultitapPullPin::request_gpio_device_file( int pin_index ) {
    if( _singleboardcomputer == nullptr ) return 0;
    return _singleboardcomputer->request_gpio_device_file( pin_index );
}

//int I2cMultitapPullPin::request_adc_device_file( int pin_index ) {}
