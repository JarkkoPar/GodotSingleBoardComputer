#include "GpioPin.h"


using namespace godot;



GpioPin::GpioPin() {
    _gpio_device_file_index = -1; // -1 is an illegal index 
    _adc_device_file_index = -1; // -1 is an illegal index
    _pin_offset = -1; // -1 is an illegal offset
}

GpioPin::~GpioPin() {

}


// Getters and setters. 

void GpioPin::set_gpio_device_file_index( int new_index ) {
    _gpio_device_file_index = new_index;
}

int GpioPin::get_gpio_device_file_index() const {
    return _gpio_device_file_index;
}

void GpioPin::set_adc_device_file_index( int new_index ) {
    _adc_device_file_index = new_index;
}

int GpioPin::get_adc_device_file_index() const {
    return _adc_device_file_index;
}


void GpioPin::set_pin_offset( int new_offset ) {
    _pin_offset = new_offset;
}

int  GpioPin::get_pin_offset() const {
    return _pin_offset;
}

void GpioPin::set_primary_function( GpioPinFunction new_function ) {
    _primary_function = new_function;
}

GpioPin::GpioPinFunction GpioPin::get_primary_function() const {
    return _primary_function;
}

void GpioPin::set_secondary_function( GpioPinFunction new_function ) {
    _secondary_function = new_function;
}

GpioPin::GpioPinFunction GpioPin::get_secondary_function() const {
    return _secondary_function;
}
