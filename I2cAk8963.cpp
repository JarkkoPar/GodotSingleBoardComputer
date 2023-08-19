#include "I2cAk8963.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/os.hpp>

using namespace godot;


I2cAk8963::I2cAk8963() {
    _i2c_device_address = 0x0C;

    _measurement_bits = 16;
    _measurement_mode = (int)AK8963Control1Mode::CONTINUOUS_MEASUREMENT_MODE_1;

    magnetic_field_x = 0.0f; 
    magnetic_field_y = 0.0f;
    magnetic_field_z = 0.0f;
}


I2cAk8963::~I2cAk8963() {
    
}


void I2cAk8963::_bind_methods() {
    // Measurement bits.
    ClassDB::bind_method(D_METHOD("set_measurement_bits", "measurement_bits"), &I2cAk8963::set_measurement_bits);
	ClassDB::bind_method(D_METHOD("get_measurement_bits"), &I2cAk8963::get_measurement_bits);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_bits", PROPERTY_HINT_ENUM, "14bits:0,16bits:16"), "set_measurement_bits", "get_measurement_bits");

    // Measurement mode.
    ClassDB::bind_method(D_METHOD("set_measurement_mode", "measurement_mode"), &I2cAk8963::set_measurement_mode);
	ClassDB::bind_method(D_METHOD("get_measurement_mode"), &I2cAk8963::get_measurement_mode);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_mode", PROPERTY_HINT_ENUM, "8Hz:2,100Hz:6,SingleShot:1"), "set_measurement_mode", "get_measurement_mode");

    // The measurement data.
    ClassDB::bind_method(D_METHOD("set_magnetic_field_x", "x"), &I2cAk8963::set_magnetic_field_x);
	ClassDB::bind_method(D_METHOD("get_magnetic_field_x"), &I2cAk8963::get_magnetic_field_x);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "magnetic_field_x", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_magnetic_field_x", "get_magnetic_field_x");

    ClassDB::bind_method(D_METHOD("set_magnetic_field_y", "y"), &I2cAk8963::set_magnetic_field_y);
	ClassDB::bind_method(D_METHOD("get_magnetic_field_y"), &I2cAk8963::get_magnetic_field_y);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "magnetic_field_y", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_magnetic_field_y", "get_magnetic_field_y");

    ClassDB::bind_method(D_METHOD("set_magnetic_field_z", "z"), &I2cAk8963::set_magnetic_field_z);
	ClassDB::bind_method(D_METHOD("get_magnetic_field_z"), &I2cAk8963::get_magnetic_field_z);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "magnetic_field_z", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_magnetic_field_z", "get_magnetic_field_z");

}




// Godot virtuals.

void I2cAk8963::_process(double delta) {
    if( _update_method != SBCDeviceUpdateMethod::PROCESS ) return;
    if( !_is_device_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    _read_sensor_data();
}

void I2cAk8963::_physics_process(double delta) {
    if( _update_method != SBCDeviceUpdateMethod::PHYSICS_PROCESS ) return;
    if( !_is_device_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    _read_sensor_data();
}

void I2cAk8963::_notification(int p_what) {

}
    
// Device handling.

void I2cAk8963::_configure_i2c_device() {
    // Selftest.
    _self_test();
        
    // Set the measurement mode.
    _write_byte_to_device( _i2c_device_address, AK8963Registers::CNTL1, (uint8_t)_measurement_bits | (uint8_t)_measurement_mode);
    /*if( _measurement_mode == 2 ) {
        _write_byte_to_device( _i2c_device_address, AK8963Registers::CNTL1, AK8963Control1Mode::SINGLE_MEASUREMENT_MODE);    
    }else if( _measurement_mode == 0 ) {
        _write_byte_to_device( _i2c_device_address, AK8963Registers::CNTL1, AK8963Control1Mode::CONTINUOUS_MEASUREMENT_MODE_1);  
    }else{
        }
    */
    OS::get_singleton()->delay_msec(50);
    
} 

void I2cAk8963::_self_test() {
    
    // Power down.
    _write_byte_to_device( _i2c_device_address, AK8963Registers::CNTL1, (uint8_t)_measurement_bits|AK8963Control1Mode::POWER_DOWN_MODE);
    OS::get_singleton()->delay_msec(100);
    // Set the bits to selected value.

    // Set the self test bit on.
    _write_byte_to_device( _i2c_device_address, AK8963Registers::ASTC, AK8963SelfTestControl::GENERATE_MAGNETIC_FIELD_FOR_SELF_TEST);
    //OS::get_singleton()->delay_msec(20);    
    _write_byte_to_device( _i2c_device_address, AK8963Registers::CNTL1, (uint8_t)_measurement_bits|AK8963Control1Mode::SELF_TEST_MODE);
    // Read the results. 
    uint8_t st1 = _read_byte_from_device( _i2c_device_address, AK8963Registers::ST1 );
    // Do a busy-wait
    while( (st1 & AK8963Status1::DATA_IS_READY) == 0 ){
        OS::get_singleton()->delay_msec(20);
    }
    _read_sensor_data();

    _write_byte_to_device( _i2c_device_address, AK8963Registers::ASTC, AK8963SelfTestControl::NORMAL_NO_SELF_TEST);
    _write_byte_to_device( _i2c_device_address, AK8963Registers::CNTL1, (uint8_t)_measurement_bits|AK8963Control1Mode::POWER_DOWN_MODE);
    OS::get_singleton()->delay_msec(100);
    
}

void I2cAk8963::_read_sensor_data() {
    // Use the normal read sequence:
    //  1. Check Data Ready is set
    //  2. Read measurement data
    //  3. Read ST2 register for overflows

    uint8_t st1 = _read_byte_from_device( _i2c_device_address, AK8963Registers::ST1 );
    if( (st1 & AK8963Status1::DATA_IS_READY) == 0 ) return;

    uint8_t data[6] = {0};
    _read_bytes_from_device( _i2c_device_address, AK8963Registers::HXH, 6, data );

    uint8_t st2 = _read_byte_from_device( _i2c_device_address, AK8963Registers::ST2 );
    if( st2 & AK8963Status1::DATA_OVERRUN ) {
        // todo: send a signal
    }

    // Set the measurement values.
    int16_t mx = (int16_t)data[0] | ((int16_t)data[1] << 8);
    int16_t my = (int16_t)data[2] | ((int16_t)data[3] << 8);
    int16_t mz = (int16_t)data[4] | ((int16_t)data[5] << 8);
    sensor_x = mx;
    sensor_y = my;
    sensor_z = mz;

    if( _measurement_bits == AK8963Control1OutputBitSetting::OUTPUT_14_BIT ) {
        magnetic_field_x = 4912.0f * (float)sensor_x / 8190.0f;
        magnetic_field_y = 4912.0f * (float)sensor_y / 8190.0f;
        magnetic_field_z = 4912.0f * (float)sensor_z / 8190.0f;
    } else { // 16 bit.
        magnetic_field_x = 4912.0f * (float)sensor_x / 32760.0f;
        magnetic_field_y = 4912.0f * (float)sensor_y / 32760.0f;
        magnetic_field_z = 4912.0f * (float)sensor_z / 32760.0f;
    }
}


// Getters and setters.

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


