#include "I2cMpu9250.h"


using namespace godot;



I2cMpu9250::I2cMpu9250() {
    _i2c_device_address = 0x68;
    _magnetometer_i2c_address = 0x0C;
}


I2cMpu9250::~I2cMpu9250() {

}



void I2cMpu9250::_bind_methods() {

}




// Godot virtuals.
    
void I2cMpu9250::_process(double delta) {}

void I2cMpu9250::_physics_process(double delta) {}

void I2cMpu9250::_notification(int p_what) {}

    
// Getters and setters.
    
    
// Device handling.

void I2cMpu9250::_configure_i2c_device() {
    // Configure the Magnetometer, gyroscope and finally the accelerometer.
    uint8_t accel_config[] = {0x18};
    _write_bytes_to_device(_i2c_device_address, MPU9250Registers::ACCEL_CONFIG, 1, accel_config );
    _write_byte_array_to_device_register(MPU9250Registers::ACCEL_CONFIG, accel_config);
    uint8_t gyro_config[] = {0x18};
    _write_byte_array_to_device_register(MPU9250Registers::GYRO_CONFIG, gyro_config);
    uint8_t magnetometer_config[] = {0x02};
    _write_byte_array_to_device_register(MPU9250Registers::INT_PIN_CFG, magnetometer_config);
    //write_byte_to_device_register_at_device_address( _magnetometer_i2c_address, , uint8_t value );
    //_write_byte_array_to_device_register(MPU9250Registers::INT_PIN_CFG, magnetometer_config);
    
}

