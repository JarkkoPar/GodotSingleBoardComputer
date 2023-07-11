#include "GpioPin.h"
#include <string.h>

using namespace godot;



GpioPin::GpioPin() {
    _gpio_device_file_index = -1; // -1 is an illegal index 
    _adc_device_file_index = -1; // -1 is an illegal index
    _adc_voltage_file_index = -1; // -1 is an illegal index
    _pin_offset = -1; // -1 is an illegal offset
    _adc_max_voltage = 0.0f; 

    _pin_functions = nullptr;
    _num_pin_functions = 0;
}

GpioPin::~GpioPin() {
    if( _pin_functions != nullptr ) { 
        delete[] _pin_functions;
        _pin_functions = nullptr;
    }
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

void GpioPin::set_adc_voltage_file_index( int new_index ) {
    _adc_voltage_file_index = new_index;
}

int GpioPin::get_adc_voltage_file_index() const {
    return _adc_voltage_file_index;
}

void GpioPin::set_adc_max_voltage( float max_voltage ) {
    _adc_max_voltage = max_voltage;
}

float GpioPin::get_adc_max_voltage() const {
    return _adc_max_voltage;
}


void GpioPin::set_pin_offset( int new_offset ) {
    _pin_offset = new_offset;
}

int  GpioPin::get_pin_offset() const {
    return _pin_offset;
}

void GpioPin::add_pin_function( GpioPinFunction new_function ) {
    GpioPinFunction* new_list = new GpioPinFunction[_num_pin_functions + 1];
    if( new_list == nullptr ) {
        return;
    }
    if( _pin_functions != nullptr ) {
        memcpy( new_list, _pin_functions, sizeof(_pin_functions));
        delete[] _pin_functions;
    }
    _pin_functions = new_list;
    _num_pin_functions += 1;
    _pin_functions[_num_pin_functions - 1] = new_function;
}

bool GpioPin::has_pin_function( GpioPinFunction function ) {
    for( int i = 0; i < _num_pin_functions; ++i ) {
        if( _pin_functions[i] == function ) {
            return true;
        }
    }
    return false;
}

/*
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
/**/

