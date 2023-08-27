#include "I2cMpu9250.h"


using namespace godot;



I2cMpu9250::I2cMpu9250() {
    _i2c_device_address = 0x68;
    _magnetometer_i2c_address = 0x0C;

    _measurement_gyro_x = 0;
    _measurement_gyro_y = 0;
    _measurement_gyro_z = 0;

    _measurement_accel_x = 0;
    _measurement_accel_y = 0;
    _measurement_accel_z = 0;
}


I2cMpu9250::~I2cMpu9250() {

}



void I2cMpu9250::_bind_methods() {


    // The measurement data.
    ClassDB::bind_method(D_METHOD("set_measurement_gyro_x", "x"), &I2cMpu9250::set_measurement_gyro_x);
	ClassDB::bind_method(D_METHOD("get_measurement_gyro_x"), &I2cMpu9250::get_measurement_gyro_x);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "measurement_gyro_x", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_gyro_x", "get_measurement_gyro_x");

    ClassDB::bind_method(D_METHOD("set_measurement_gyro_y", "y"), &I2cMpu9250::set_measurement_gyro_y);
	ClassDB::bind_method(D_METHOD("get_measurement_gyro_y"), &I2cMpu9250::get_measurement_gyro_y);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "measurement_gyro_y", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_gyro_y", "get_measurement_gyro_y");

    ClassDB::bind_method(D_METHOD("set_measurement_gyro_z", "z"), &I2cMpu9250::set_measurement_gyro_z);
	ClassDB::bind_method(D_METHOD("get_measurement_gyro_z"), &I2cMpu9250::get_measurement_gyro_z);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "measurement_gyro_z", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_gyro_z", "get_measurement_gyro_z");

}




// Godot virtuals.
    
void I2cMpu9250::_process(double delta) {}

void I2cMpu9250::_physics_process(double delta) {}

void I2cMpu9250::_notification(int p_what) {}

    
// Getters and setters.


void I2cMpu9250::set_measurement_gyro_x( int x ) {
    _measurement_gyro_x = x;
}

int I2cMpu9250::get_measurement_gyro_x() const {
    return _measurement_gyro_x;
}

void I2cMpu9250::set_measurement_gyro_y( int y ) {
    _measurement_gyro_x = y;
}

int I2cMpu9250::get_measurement_gyro_y() const {
    return _measurement_gyro_y;
}

void I2cMpu9250::set_measurement_gyro_z( int z ) {
    _measurement_gyro_z = z;
}

int I2cMpu9250::get_measurement_gyro_z() const {
    return _measurement_gyro_z;
}



// Device handling.

bool I2cMpu9250::_configure_i2c_device() {
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
    
    return true;
}


void I2cMpu9250::_self_test() {

}


void I2cMpu9250::_read_sensor_data() {
        // Use the normal read sequence:
    //  1. Check Data Ready is set
    //  2. Read measurement data
    //  3. Read ST2 register for overflows

    //uint8_t st1 = _read_byte_from_device( _i2c_device_address, AK8963Registers::ST1 );
    //if( (st1 & AK8963Status1::DATA_IS_READY) == 0 ) return;

    uint8_t data[14] = {0};
    _read_bytes_from_device( _i2c_device_address, MPU9250Registers::ACCEL_XOUT_H, 14, data );

    //uint8_t st2 = _read_byte_from_device( _i2c_device_address, AK8963Registers::ST2 );
    //if( st2 & AK8963Status1::DATA_OVERRUN ) {
        // todo: send a signal
    //}

    // Set the measurement values.
    int16_t gx = (int16_t)data[0] << 8 | ((int16_t)data[1]);
    int16_t gy = (int16_t)data[2] << 8 | ((int16_t)data[3]);
    int16_t gz = (int16_t)data[4] << 8 | ((int16_t)data[5]);
    
    uint16_t temp = (int16_t)data[6] << 8 | ((int16_t)data[7]);

    int16_t ax = (int16_t)data[ 8] << 8 | ((int16_t)data[ 9]);
    int16_t ay = (int16_t)data[10] << 8 | ((int16_t)data[11]);
    int16_t az = (int16_t)data[12] << 8 | ((int16_t)data[13]);

    //GYRO_XOUT = Gyro_Sensitivity * X_angular_rate
    //Nominal
    //Conditions
    //FS_SEL = 0
    //Gyro_Sensitivity = 131 LSB/( º/s )    

    //TEMP_degC = ((TEMP_OUT –
    //RoomTemp_Offset)/Temp_Sensitivity)
    //+ 21degC
    temperature_celsius = (((float)temp - _room_temperature_offset)*_one_over_temperature_sensitivity) + 21.0f;

    /*sensor_x = mx;
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
    /**/
}

