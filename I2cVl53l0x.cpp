#include "I2cVl53l0x.h"
#include <godot_cpp/classes/engine.hpp>


using namespace godot;

I2cVl53l0x::I2cVl53l0x() {
    _is_active = true;
    _is_vl53l0x_initialized = false;
    _vl53l0x_update_frame_delay = 0.25;
    _vl53l0x_update_wait_time = 0.0;
    //_custom_vl53l0x_i2c_device_address = 0x28;

    _reading_mode = (int)Vl53l0xReadingMode::BACK_TO_BACK;
    _distance_mm = 0.0f;
}

I2cVl53l0x::~I2cVl53l0x() {
    
}

void I2cVl53l0x::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_custom_vl53l0x_i2c_device_address", "new_i2c_device_address"), &I2cVl53l0x::set_custom_vl53l0x_i2c_device_address);
	//ClassDB::bind_method(D_METHOD("get_custom_vl53l0x_i2c_device_address"), &I2cVl53l0x::get_custom_vl53l0x_i2c_device_address);

    ClassDB::bind_method(D_METHOD("set_is_active", "is_active"), &I2cVl53l0x::set_is_active);
	ClassDB::bind_method(D_METHOD("get_is_active"), &I2cVl53l0x::get_is_active);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_active", PROPERTY_HINT_NONE ), "set_is_active", "get_is_active");

    ClassDB::bind_method(D_METHOD("set_reading_mode", "mode"), &I2cVl53l0x::set_reading_mode);
	ClassDB::bind_method(D_METHOD("get_reading_mode"), &I2cVl53l0x::get_reading_mode);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "reading_mode", PROPERTY_HINT_ENUM, "One_shot:0,Continuous:2"), "set_reading_mode", "get_reading_mode");

    ClassDB::bind_method(D_METHOD("set_distance_mm", "distance"), &I2cVl53l0x::set_distance_mm);
	ClassDB::bind_method(D_METHOD("get_distance_mm"), &I2cVl53l0x::get_distance_mm);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance_mm", PROPERTY_HINT_RANGE, "0.0,2000.0,allow_greater,suffix:mm", PROPERTY_USAGE_READ_ONLY), "set_distance_mm", "get_distance_mm");

}

// Godot virtuals.
void I2cVl53l0x::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) return;
    if(!_is_active) return;
    if(_reading_mode != (int)Vl53l0xReadingMode::BACK_TO_BACK) return;

    _vl53l0x_update_wait_time -= delta;
    if( _vl53l0x_update_wait_time > 0.0 ) return;
    _vl53l0x_update_wait_time = _vl53l0x_update_frame_delay;

    ERR_FAIL_COND_MSG( !_is_vl53l0x_initialized, "VL53L0X is not yet initialized, cannot update distance_mm in _process().");
    
    uint8_t msb = read_byte_from_device_register((uint8_t)(Vl53l0xRegisters::MEASUREMENT_MOST_SIGNIFICANT_BIT));
    uint8_t lsb = read_byte_from_device_register((uint8_t)(Vl53l0xRegisters::MEASUREMENT_LEAST_SIGNIFICANT_BIT));
    
    _distance_mm = (float)( (uint16_t)(msb << 8) | (uint16_t)(lsb) );
}

void I2cVl53l0x::_physics_process(double delta) {

}

void I2cVl53l0x::_notification(int p_what) {

}


// Getters and setters.
void I2cVl53l0x::set_is_active( bool is_active ) {
    _is_active = is_active;
}

bool I2cVl53l0x::get_is_active() const {
    return _is_active;
}


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


void I2cVl53l0x::set_custom_vl53l0x_i2c_device_address( int new_i2c_device_address ) {
    write_byte_to_device_register(Vl53l0xRegisters::I2C_SLAVE_DEVICE_ADDRESS, (uint8_t)(new_i2c_device_address) & 0x7F);
    //_custom_vl53l0x_i2c_device_address = (uint8_t)new_i2c_device_address;
}

//int  I2cVl53l0x::get_custom_vl53l0x_i2c_device_address() {
    //_custom_vl53l0x_i2c_device_address = read_byte_from_device_register(Vl53l0xRegisters::I2C_SLAVE_DEVICE_ADDRESS);
//    return (int)_custom_vl53l0x_i2c_device_address;
//}
    
// Device handling.

void I2cVl53l0x::_initialize_device() {
    // Only initialize once.
    if( _is_vl53l0x_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    if( !_is_active ) return;
    set_i2c_device_bus_number(_i2c_device_bus_number);
    open_device();
    ERR_FAIL_COND_MSG(_i2c_device_fd < 0, "VL53L0X initialization failed because the device file is not opened.");
    

    write_byte_to_device_register( Vl53l0xRegisters::SYSRANGE_START, (uint8_t)_reading_mode );//Vl53l0xReadingMode::BACK_TO_BACK );

    uint8_t current_device_address = read_byte_from_device_register(Vl53l0xRegisters::I2C_SLAVE_DEVICE_ADDRESS);
    //if( _custom_vl53l0x_i2c_device_address != current_device_address ) {
    //    WARN_PRINT_ONCE("The custom device address is set but it does not match the address on the device.");
    //}
    
    _is_vl53l0x_initialized = true;
}   
    

