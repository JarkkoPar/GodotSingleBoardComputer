#include "I2cBmp280.h"

#include <godot_cpp/classes/engine.hpp>

using namespace godot;


I2cBmp280::I2cBmp280() {
    _i2c_device_address = 0x76;
}

I2cBmp280::~I2cBmp280() {
    
}

void I2cBmp280::_bind_methods() {
    
}



// Godot virtuals.

void I2cBmp280::_process(double delta) {
    if( _update_method != SBCDeviceUpdateMethod::PROCESS ) return;
    if( !_is_device_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    _read_sensor_data();
}

void I2cBmp280::_physics_process(double delta) {
    if( _update_method != SBCDeviceUpdateMethod::PHYSICS_PROCESS ) return;
    if( !_is_device_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    _read_sensor_data();
}

void I2cBmp280::_notification(int p_what) {

}
    
// Device handling.

void I2cBmp280::_configure_i2c_device() {
    _write_byte_to_device(_i2c_device_address, BMP280Registers::CONFIG, BMP280StandByTime::STAND_BY_0_5_MS|BMP280FilterSetting::FILTER_COEFFICIENT_02|BMP280ThreeWireSpiSetting::DISABLE_3_WIRE_SPI);
    _write_byte_to_device(_i2c_device_address, BMP280Registers::CTRL_MEAS, BMP280PressureMeasurementSetting::ULTRA_LOW_POWER|BMP280TemperatureMeasurementSetting::TEMPERATURE_OVERSAMPLING_X01|BMP280PowerMode::NORMAL_POWER_MODE);

} 

void I2cBmp280::_self_test() {

}

void I2cBmp280::_read_sensor_data() {
    uint8_t status = _read_byte_from_device(_i2c_device_address, BMP280Registers::STATUS );
    if( status & BMP280Status::MEASURING || status & BMP280Status::NVM_DATA_IS_BEING_COPIED ) return;

    uint8_t data[6] = {0};
    _read_bytes_from_device(_i2c_device_address, BMP280Registers::PRESS_MSB, 6, data );

    // Todo: Get the measurement based on bits used.
    uint32_t pressure_measurement = (uint32_t)data[6];
}


// Getters and setters.

/**
void I2cAk8963::set_measurement_bits( int measurement_bits ) {
    _measurement_bits = measurement_bits;
}

int  I2cAk8963::get_measurement_bits() const {
    return _measurement_bits;
}

void I2cAk8963::set_measurement_mode( int measurement_mode ) {
    _measurement_mode = measurement_mode;
}

int  I2cAk8963::get_measurement_mode() const {
    return _measurement_mode;
}


void  I2cAk8963::set_magnetic_field_x(float x ) {
    magnetic_field_x = x;
}

float I2cAk8963::get_magnetic_field_x() const {
    return magnetic_field_x;
}
 
void  I2cAk8963::set_magnetic_field_y(float y ) {
    magnetic_field_y = y;
}

float I2cAk8963::get_magnetic_field_y() const {
    return magnetic_field_y;
}

void  I2cAk8963::set_magnetic_field_z(float z ) {
    magnetic_field_z = z;
}

float I2cAk8963::get_magnetic_field_z() const {
    return magnetic_field_z;
}


/**/

