#include "I2cVl53l0x.h"
#include <godot_cpp/classes/engine.hpp>


using namespace godot;

I2cVl53l0x::I2cVl53l0x() {
    _is_vl53l0x_initialized = false;
    _vl53l0x_update_frame_delay = 0.25;
    _vl53l0x_update_wait_time = 0.0;

    _reading_mode = (int)Vl53l0xReadingMode::BACK_TO_BACK;
    _distance_mm = 0.0f;
}

I2cVl53l0x::~I2cVl53l0x() {
    
}

void I2cVl53l0x::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_reading_mode", "mode"), &I2cVl53l0x::set_reading_mode);
	ClassDB::bind_method(D_METHOD("get_reading_mode"), &I2cVl53l0x::get_reading_mode);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "reading_mode", PROPERTY_HINT_ENUM, "One_shot:0,Continuous:2"), "set_reading_mode", "get_reading_mode");

    ClassDB::bind_method(D_METHOD("set_distance_mm", "distance"), &I2cVl53l0x::set_distance_mm);
	ClassDB::bind_method(D_METHOD("get_distance_mm"), &I2cVl53l0x::get_distance_mm);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance_mm", PROPERTY_HINT_RANGE, "0.0,4000.0,allow_greater"), "set_distance_mm", "get_distance_mm");

}

// Godot virtuals.
void I2cVl53l0x::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) return;


    _vl53l0x_update_wait_time -= delta;
    if( _vl53l0x_update_wait_time > 0.0 ) return;
    _vl53l0x_update_wait_time = _vl53l0x_update_frame_delay;

    ERR_FAIL_COND_MSG( !_is_vl53l0x_initialized, "VL53L0X is not yet initialized, cannot update distance in _process().");
    
    //write_byte_to_device_register(0x1e, )
    uint8_t msb = read_byte_from_device_register((uint8_t)(0x1e));
    uint8_t lsb = read_byte_from_device_register((uint8_t)(0x1e + 10));
    //_read_bytes_from_device(1);
    
    _distance_mm = (float)( (uint16_t)(msb << 8) | (uint16_t)(lsb) );
}

void I2cVl53l0x::_physics_process(double delta) {

}

void I2cVl53l0x::_notification(int p_what) {

}


// Getters and setters.
void I2cVl53l0x::set_reading_mode( int mode ) {
    _reading_mode = mode;
}

int  I2cVl53l0x::get_reading_mode() const {
    return _reading_mode;
}

void  I2cVl53l0x::set_distance_mm( float distance_mm ) {
    _distance_mm = distance_mm;
}

float I2cVl53l0x::get_distance_mm() {
    return _distance_mm;
}

    
// Device handling.

void I2cVl53l0x::_initialize_device() {
    write_byte_to_device_register( Vl53l0xRegisters::SYSRANGE_START, (uint8_t)_reading_mode );//Vl53l0xReadingMode::BACK_TO_BACK );


    _is_vl53l0x_initialized = true;
}   
    

