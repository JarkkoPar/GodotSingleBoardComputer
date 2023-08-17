#include "GpioLED.h"

#include "godot_cpp/classes/engine.hpp"


using namespace godot;

GpioLED::GpioLED() {
    _led_pin_index = 8;
    _is_LED_on = false;
    _led_fd = -1;
}

GpioLED::~GpioLED() {
    
}

void GpioLED::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_led_pin_index", "led_pin_number"), &GpioLED::set_LED_pin_index );
	ClassDB::bind_method(D_METHOD("get_led_pin_index"), &GpioLED::get_LED_pin_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "gpio_LED_pin_number", PROPERTY_HINT_RANGE, "1,40"), "set_led_pin_index", "get_led_pin_index");

    ClassDB::bind_method(D_METHOD("set_is_LED_on", "is_on"), &GpioLED::set_is_LED_on);
	ClassDB::bind_method(D_METHOD("get_is_LED_on"), &GpioLED::get_is_LED_on);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_led_on"), "set_is_LED_on", "get_is_LED_on");
    ClassDB::bind_method(D_METHOD("is_LED_on"), &GpioLED::get_is_LED_on);
    ClassDB::bind_method(D_METHOD("toggle_LED", "is_on"), &GpioLED::set_is_LED_on);
}


// Getters and setters.

int  GpioLED::get_LED_pin_index() const {
    return _led_pin_index + 1;
}
void GpioLED::set_LED_pin_index( const int pin_number ) {
    _led_pin_index = pin_number - 1;
}


bool GpioLED::get_is_LED_on() const {
    return _is_LED_on;
}


void GpioLED::set_is_LED_on( const bool is_on ) {
    _is_LED_on = is_on;
    if(Engine::get_singleton()->is_editor_hint()) return; // Don't set the pin in editor.
    if( is_inside_tree() == false ) return; // If just loading, don't try to set the led.

    if( _is_LED_on ) {
        write_byte_to_device(_led_fd, 1);    
    } else {
        write_byte_to_device(_led_fd, 0);
    }
    
}


// Device handling.

void GpioLED::_open_gpio_device() {
    _led_fd = request_pin_number(_led_pin_index + 1, GPIO_TYPE_OUTPUT, (char *)this->get_name().to_ascii_buffer().ptr());
}


void GpioLED::_configure_gpio_device() {
    ERR_FAIL_COND_MSG(_led_fd < 0, "LED configuration failed because the device file is not opened.");
    
    if( _is_LED_on ) {
        write_byte_to_device(_led_fd, 1);    
    } else {
        write_byte_to_device(_led_fd, 0);
    }
}

