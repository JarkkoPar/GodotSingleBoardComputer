#include "AdcTemperatureSensor.h"
#include <godot_cpp/classes/engine.hpp>

using namespace godot;


void AdcTemperatureSensor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_temperature_kelvin", "kelvin"), &AdcTemperatureSensor::set_temperature_kelvin);
	ClassDB::bind_method(D_METHOD("get_temperature_kelvin"), &AdcTemperatureSensor::get_temperature_kelvin);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Kelvin", PROPERTY_HINT_NONE), "set_temperature_kelvin", "get_temperature_kelvin");

    ClassDB::bind_method(D_METHOD("set_temperature_celsius", "celsius"), &AdcTemperatureSensor::set_temperature_celsius);
	ClassDB::bind_method(D_METHOD("get_temperature_celsius"), &AdcTemperatureSensor::get_temperature_celsius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Celsius", PROPERTY_HINT_NONE), "set_temperature_celsius", "get_temperature_celsius");

    ClassDB::bind_method(D_METHOD("set_temperature_fahrenheit", "fahrenheit"), &AdcTemperatureSensor::set_temperature_fahrenheit);
	ClassDB::bind_method(D_METHOD("get_temperature_fahrenheit"), &AdcTemperatureSensor::get_temperature_fahrenheit);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Fahrenheit", PROPERTY_HINT_NONE), "set_temperature_fahrenheit", "get_temperature_fahrenheit");

    ADD_GROUP("Temperature range", "");

    ClassDB::bind_method(D_METHOD("set_min_temperature_celsius", "celsius"), &AdcTemperatureSensor::set_min_temperature_celsius);
	ClassDB::bind_method(D_METHOD("get_min_temperature_celsius"), &AdcTemperatureSensor::get_min_temperature_celsius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Min Celsius", PROPERTY_HINT_NONE), "set_min_temperature_celsius", "get_min_temperature_celsius");

    ClassDB::bind_method(D_METHOD("set_max_temperature_celsius", "celsius"), &AdcTemperatureSensor::set_max_temperature_celsius);
	ClassDB::bind_method(D_METHOD("get_max_temperature_celsius"), &AdcTemperatureSensor::get_max_temperature_celsius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Max Celsius", PROPERTY_HINT_NONE), "set_max_temperature_celsius", "get_max_temperature_celsius");

 }

AdcTemperatureSensor::AdcTemperatureSensor() {
    _temperature_celsius = 0.0f;
    _temperature_fahrenheit = 0.0f;
    _temperature_kelvin = 0.0f;

    _min_temperature_celsius = -55.0f;
    _max_temperature_celsius = 125.0f;

    /*A = 0.001129148f;
    B = 0.000234125f;
    C = 0.0000000876741f;
    lnR = 9.21034037198f;*/
}


AdcTemperatureSensor::~AdcTemperatureSensor() {

}

// Godot virtuals.
void AdcTemperatureSensor::_process(double delta) {

}

void AdcTemperatureSensor::_physics_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) return;
    read_value_from_pin();

    _temperature_celsius = _min_temperature_celsius + (_max_temperature_celsius - _min_temperature_celsius) * (1.0f - (float)_adc_pin_value * 0.00097656f);
    _temperature_fahrenheit = _temperature_celsius * 33.8f;
    _temperature_kelvin = _temperature_celsius * 274.15f;
}

void AdcTemperatureSensor::_notification(int p_what) {

}

// Getters and setters.

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

