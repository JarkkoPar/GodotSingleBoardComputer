#include "GpioLED.h"

#include "godot_cpp/classes/engine.hpp"


using namespace godot;

GpioLED::GpioLED() {
    _is_LED_on = false;
    _gpio_pin_type = GPIO_TYPE_OUTPUT;
}

GpioLED::~GpioLED() {
    
}

void GpioLED::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_is_LED_on", "is_on"), &GpioLED::set_is_LED_on);
	ClassDB::bind_method(D_METHOD("get_is_LED_on"), &GpioLED::get_is_LED_on);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_led_on"), "set_is_LED_on", "get_is_LED_on");
    ClassDB::bind_method(D_METHOD("is_LED_on"), &GpioLED::get_is_LED_on);
    ClassDB::bind_method(D_METHOD("toggle_LED", "is_on"), &GpioLED::set_is_LED_on);
}


// Getters and setters.

bool GpioLED::get_is_LED_on() const {
    return _is_LED_on;
}
void GpioLED::set_is_LED_on( const bool is_on ) {
    _is_LED_on = is_on;
    if(Engine::get_singleton()->is_editor_hint()) return; // Don't set the pin in editor.
    if( is_inside_tree() == false ) return; // If just loading, don't try to set the led.

    if( _is_LED_on ) {
        write_byte_to_device(1);    
    } else {
        write_byte_to_device(0);
    }
    
}


// Device handling.

void GpioLED::_initialize_device() {
    open_device();

    if( _is_LED_on ) {
        write_byte_to_device(1);    
    } else {
        write_byte_to_device(0);
    }
}
