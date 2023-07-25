#include "AdcTemperatureSensor.h"
#include <godot_cpp/classes/engine.hpp>
#include <math.h>

using namespace godot;


void AdcTemperatureSensor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_temperature_kelvin", "kelvin"), &AdcTemperatureSensor::set_temperature_kelvin);
	ClassDB::bind_method(D_METHOD("get_temperature_kelvin"), &AdcTemperatureSensor::get_temperature_kelvin);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "kelvin", PROPERTY_HINT_NONE), "set_temperature_kelvin", "get_temperature_kelvin");

    ClassDB::bind_method(D_METHOD("set_temperature_celsius", "celsius"), &AdcTemperatureSensor::set_temperature_celsius);
	ClassDB::bind_method(D_METHOD("get_temperature_celsius"), &AdcTemperatureSensor::get_temperature_celsius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "celsius", PROPERTY_HINT_NONE), "set_temperature_celsius", "get_temperature_celsius");

    ClassDB::bind_method(D_METHOD("set_temperature_fahrenheit", "fahrenheit"), &AdcTemperatureSensor::set_temperature_fahrenheit);
	ClassDB::bind_method(D_METHOD("get_temperature_fahrenheit"), &AdcTemperatureSensor::get_temperature_fahrenheit);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "fahrenheit", PROPERTY_HINT_NONE), "set_temperature_fahrenheit", "get_temperature_fahrenheit");

    ADD_GROUP("Sensor configuration", "");

    ClassDB::bind_method(D_METHOD("set_internal_resistance", "ohms"), &AdcTemperatureSensor::set_internal_resistance);
	ClassDB::bind_method(D_METHOD("get_internal_resistance"), &AdcTemperatureSensor::get_internal_resistance);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "internal_resistance", PROPERTY_HINT_RANGE, "0,1000000,1,or_greater"), "set_internal_resistance", "get_internal_resistance");

    ClassDB::bind_method(D_METHOD("set_a", "a"), &AdcTemperatureSensor::set_a);
	ClassDB::bind_method(D_METHOD("get_a"), &AdcTemperatureSensor::get_a);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "a_coefficient", PROPERTY_HINT_RANGE, "0.0000000000000000,1.0000000000000000,0.0000000000000001,or_greater,or_less"), "set_a", "get_a");

    ClassDB::bind_method(D_METHOD("set_b", "b"), &AdcTemperatureSensor::set_b);
	ClassDB::bind_method(D_METHOD("get_b"), &AdcTemperatureSensor::get_b);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "b_coefficient", PROPERTY_HINT_RANGE, "0.0000000000000000,1.0000000000000000,0.0000000000000001,or_greater,or_less"), "set_b", "get_b");

    ClassDB::bind_method(D_METHOD("set_c", "c"), &AdcTemperatureSensor::set_c);
	ClassDB::bind_method(D_METHOD("get_c"), &AdcTemperatureSensor::get_c);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "c_coefficient", PROPERTY_HINT_RANGE, "0.0000000000000000,1.0000000000000000,0.0000000000000001,or_greater,or_less"), "set_c", "get_c");

    ClassDB::bind_method(D_METHOD("set_is_averaged", "is_averaged"), &AdcTemperatureSensor::set_is_averaged);
	ClassDB::bind_method(D_METHOD("get_is_averaged"), &AdcTemperatureSensor::get_is_averaged);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_averaged", PROPERTY_HINT_NONE), "set_is_averaged", "get_is_averaged");


    ClassDB::bind_method(D_METHOD("set_min_temperature_celsius", "celsius"), &AdcTemperatureSensor::set_min_temperature_celsius);
	ClassDB::bind_method(D_METHOD("get_min_temperature_celsius"), &AdcTemperatureSensor::get_min_temperature_celsius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_celsius", PROPERTY_HINT_RANGE, "-1000.0,1000.0,0.01,suffix:C"), "set_min_temperature_celsius", "get_min_temperature_celsius");

    ClassDB::bind_method(D_METHOD("set_max_temperature_celsius", "celsius"), &AdcTemperatureSensor::set_max_temperature_celsius);
	ClassDB::bind_method(D_METHOD("get_max_temperature_celsius"), &AdcTemperatureSensor::get_max_temperature_celsius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_celsius", PROPERTY_HINT_RANGE, "-1000.0,1000.0,0.01,suffix:C"), "set_max_temperature_celsius", "get_max_temperature_celsius");

 }

AdcTemperatureSensor::AdcTemperatureSensor() {
    _temperature_celsius = 0.0f;
    _temperature_fahrenheit = 0.0f;
    _temperature_kelvin = 0.0f;

    _min_temperature_celsius = -55.0f;
    _max_temperature_celsius = 125.0f;

    _internal_resistance = 10000.0f;
    
    _A = 0.001129148f;
    _B = 0.000234125f;
    _C = 0.0000000876741f;
    
    _is_averaged = false;


    
}


AdcTemperatureSensor::~AdcTemperatureSensor() {

}

// Godot virtuals.
void AdcTemperatureSensor::_process(double delta) {

}

void AdcTemperatureSensor::_physics_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) return;
    read_value_from_pin();

    // todo: add averaged version
    float fpin_value = (float)_adc_pin_value; 
    float thermistor_r = 99999999.0f;
    if( _adc_pin_value != 1023 ) {
        thermistor_r = _internal_resistance * fpin_value / (1023.0f - fpin_value);
    }
    float logr = logf(thermistor_r);
    float one_divided_by_T = _A + _B * logr + _C * logr * logr * logr;
    if( one_divided_by_T != 0.0f ) {
        _temperature_kelvin = 1.0f / one_divided_by_T;
    } else {
        _temperature_kelvin = 0.0f;
    }

    _temperature_celsius = _temperature_kelvin - 273.15f;
    _temperature_fahrenheit = _temperature_celsius * 1.8f + 32.0f;
    
}

void AdcTemperatureSensor::_notification(int p_what) {

}

// Getters and setters.


void AdcTemperatureSensor::set_internal_resistance( int ohms ) {
    _internal_resistance = (float)ohms;
}

int AdcTemperatureSensor::get_internal_resistance() const {
    return (int)_internal_resistance;
}

void AdcTemperatureSensor::set_a( double a ) {
    _A = a;
}

double AdcTemperatureSensor::get_a() const {
    return _A;
}

void AdcTemperatureSensor::set_b( double b ) {
    _B = b;
}

double AdcTemperatureSensor::get_b() const {
    return _B;
}

void AdcTemperatureSensor::set_c( double c ) {
    _C = c;
}

double AdcTemperatureSensor::get_c() const {
    return _C;
}

void AdcTemperatureSensor::set_is_averaged( bool is_averaged ) {
    _is_averaged = is_averaged;
}

bool AdcTemperatureSensor::get_is_averaged() const {
    return _is_averaged;
}


void AdcTemperatureSensor::set_min_temperature_celsius( float celsius ) {
    _min_temperature_celsius = celsius;
}

float AdcTemperatureSensor::get_min_temperature_celsius() const {
    return _min_temperature_celsius;
}

void AdcTemperatureSensor::set_max_temperature_celsius( float celsius ) {
    _max_temperature_celsius = celsius;
}

float AdcTemperatureSensor::get_max_temperature_celsius() const {
    return _max_temperature_celsius;
}

void AdcTemperatureSensor::set_temperature_kelvin( float kelvin ) {
    _temperature_kelvin = kelvin;
}

float AdcTemperatureSensor::get_temperature_kelvin() const {
    return _temperature_kelvin;
}

void AdcTemperatureSensor::set_temperature_celsius( float celsius ) {
    _temperature_celsius = celsius;
}

float AdcTemperatureSensor::get_temperature_celsius() const {
    return _temperature_celsius;
}

void AdcTemperatureSensor::set_temperature_fahrenheit( float fahrenheit ) {
    _temperature_fahrenheit = fahrenheit;
}

float AdcTemperatureSensor::get_temperature_fahrenheit() const {
    return _temperature_fahrenheit;
}

