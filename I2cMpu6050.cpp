#include "I2cMpu6050.h"
#include <godot_cpp/classes/os.hpp>

using namespace godot;



I2cMpu6050::I2cMpu6050() {
    _i2c_device_address = 0x68;
    //_magnetometer_i2c_address = 0x0C;

    _measurement_gyro_x = 0;
    _measurement_gyro_y = 0;
    _measurement_gyro_z = 0;

    _measurement_accel_x = 0;
    _measurement_accel_y = 0;
    _measurement_accel_z = 0;

    _is_dlpf_enabled = false;
}


I2cMpu6050::~I2cMpu6050() {

}



void I2cMpu6050::_bind_methods() {


    // The measurement data.
    ClassDB::bind_method(D_METHOD("set_measurement_gyro_x", "x"), &I2cMpu6050::set_measurement_gyro_x);
	ClassDB::bind_method(D_METHOD("get_measurement_gyro_x"), &I2cMpu6050::get_measurement_gyro_x);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_gyro_x", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_gyro_x", "get_measurement_gyro_x");

    ClassDB::bind_method(D_METHOD("set_measurement_gyro_y", "y"), &I2cMpu6050::set_measurement_gyro_y);
	ClassDB::bind_method(D_METHOD("get_measurement_gyro_y"), &I2cMpu6050::get_measurement_gyro_y);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_gyro_y", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_gyro_y", "get_measurement_gyro_y");

    ClassDB::bind_method(D_METHOD("set_measurement_gyro_z", "z"), &I2cMpu6050::set_measurement_gyro_z);
	ClassDB::bind_method(D_METHOD("get_measurement_gyro_z"), &I2cMpu6050::get_measurement_gyro_z);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_gyro_z", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_gyro_z", "get_measurement_gyro_z");

    ClassDB::bind_method(D_METHOD("set_measurement_accel_x", "x"), &I2cMpu6050::set_measurement_accel_x);
	ClassDB::bind_method(D_METHOD("get_measurement_accel_x"), &I2cMpu6050::get_measurement_accel_x);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_accel_x", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_accel_x", "get_measurement_accel_x");

    ClassDB::bind_method(D_METHOD("set_measurement_accel_y", "y"), &I2cMpu6050::set_measurement_accel_y);
	ClassDB::bind_method(D_METHOD("get_measurement_accel_y"), &I2cMpu6050::get_measurement_accel_y);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_accel_y", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_accel_y", "get_measurement_accel_y");

    ClassDB::bind_method(D_METHOD("set_measurement_accel_z", "z"), &I2cMpu6050::set_measurement_accel_z);
	ClassDB::bind_method(D_METHOD("get_measurement_accel_z"), &I2cMpu6050::get_measurement_accel_z);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_accel_z", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_accel_z", "get_measurement_accel_z");

    ClassDB::bind_method(D_METHOD("set_temperature_celsius", "celsius"), &I2cMpu6050::set_temperature_celsius);
	ClassDB::bind_method(D_METHOD("get_temperature_celsius"), &I2cMpu6050::get_temperature_celsius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "temperature_celsius", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_temperature_celsius", "get_temperature_celsius");

}




// Godot virtuals.
    
void I2cMpu6050::_process(double delta) {}

void I2cMpu6050::_physics_process(double delta) {}

void I2cMpu6050::_notification(int p_what) {}

    
// Getters and setters.


void I2cMpu6050::set_measurement_gyro_x( int x ) {
    _measurement_gyro_x = x;
}

int I2cMpu6050::get_measurement_gyro_x() const {
    return _measurement_gyro_x;
}

void I2cMpu6050::set_measurement_gyro_y( int y ) {
    _measurement_gyro_x = y;
}

int I2cMpu6050::get_measurement_gyro_y() const {
    return _measurement_gyro_y;
}

void I2cMpu6050::set_measurement_gyro_z( int z ) {
    _measurement_gyro_z = z;
}

int I2cMpu6050::get_measurement_gyro_z() const {
    return _measurement_gyro_z;
}


void I2cMpu6050::set_measurement_accel_x( int x ) {
    _measurement_accel_x = x;
}

int I2cMpu6050::get_measurement_accel_x() const {
    return _measurement_accel_x;
}

void I2cMpu6050::set_measurement_accel_y( int y ) {
    _measurement_accel_x = y;
}

int I2cMpu6050::get_measurement_accel_y() const {
    return _measurement_accel_y;
}

void I2cMpu6050::set_measurement_accel_z( int z ) {
    _measurement_accel_z = z;
}

int I2cMpu6050::get_measurement_accel_z() const {
    return _measurement_accel_z;
}

/*
void I2cMpu6050::set_temperature_celsius( float celsius ) {
    temperature_celsius = celsius;
}

float I2cMpu6050::get_temperature_celsius() const {
    return temperature_celsius;
}
*/


// Device handling.

bool I2cMpu6050::_configure_i2c_device() {
    // Start by resetting the device to get it to defaults and waking it up.
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::PWR_MGMT_1, MPU6050PowerManagement1::POWER_MANAGEMENT_1_RESET );
    OS::get_singleton()->delay_msec(100);
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::PWR_MGMT_1, MPU6050PowerManagement1::POWER_MANAGEMENT_1_CLKSEL_AUTO );  
    OS::get_singleton()->delay_msec(200);

    //_write_byte_to_device( _i2c_device_address, MPU6050Registers::)
    //_write_byte_to_device( _i2c_device_address, MPU6050Registers::GYRO_CONFIG, MPU6050GyroscopeConfiguration::Fchoice_b_GYRO_USE_DLPF_CFG );
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::CONFIG, 0x03 ); // 44 Hz accelerometer bandwidth and 42 Hz gyro bandwith
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::SMPLRT_DIV, 0x04 ); // 200 Hz


    // Configure the Magnetometer, gyroscope and finally the accelerometer.
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::ACCEL_CONFIG, MPU6050AccelerometerConfiguration::ACCEL_FS_SEL_16G  );
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::GYRO_CONFIG, MPU6050GyroscopeConfiguration::XGYRO_FS_SEL_1000_DPS );    

    // Enable bypass to allow additional sensors to be 
    // controlled through the same i2c bus through Godot.
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::INT_PIN_CFG, MPU6050IntPinCfgSettings::BYPASS_EN | MPU6050IntPinCfgSettings::INT_ANYRD_2CLEAR ); 
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::INT_ENABLE, 0x01 );
    //OS::get_singleton()->delay_msec(100);

    return true;
}


void I2cMpu6050::_reset_device() {
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::PWR_MGMT_1, MPU6050PowerManagement1::POWER_MANAGEMENT_1_RESET );
    OS::get_singleton()->delay_msec(100);
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::PWR_MGMT_1, MPU6050PowerManagement1::POWER_MANAGEMENT_1_CLKSEL_AUTO );  
    OS::get_singleton()->delay_msec(200);
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::CONFIG, MPU6050Configuration::DLPF_CFG );  
    
}


void I2cMpu6050::_self_test() {
    _reset_device();
    _self_test_gyro();
    _self_test_accelerator();
}

void I2cMpu6050::_self_test_gyro() {
    // Enable self-test.
    // For self-test the dps range must be 250 (bits 00).
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::GYRO_CONFIG, 
     MPU6050GyroscopeConfiguration::GYROSCOPE_CONFIGURATION_RESET
    |MPU6050GyroscopeConfiguration::XG_ST
    |MPU6050GyroscopeConfiguration::YG_ST
    |MPU6050GyroscopeConfiguration::ZG_ST
    ); 
    OS::get_singleton()->delay_msec(100);
    
    // Get the sensor self-test outputs.
    uint8_t selftest_x_raw = _read_byte_from_device( _i2c_device_address, MPU6050Registers::SELF_TEST_X);
    uint8_t selftest_y_raw = _read_byte_from_device( _i2c_device_address, MPU6050Registers::SELF_TEST_Y);
    uint8_t selftest_z_raw = _read_byte_from_device( _i2c_device_address, MPU6050Registers::SELF_TEST_Z);
    uint8_t selftest_a_raw = _read_byte_from_device( _i2c_device_address, MPU6050Registers::SELF_TEST_A);

    _selftest_measurement_acceleration_x = ((selftest_x_raw & 0b11100000) >> 3) | ((selftest_a_raw & 0b00110000) >> 4);
    _selftest_measurement_acceleration_y = ((selftest_y_raw & 0b11100000) >> 3) | ((selftest_a_raw & 0b00001100) >> 2);
    _selftest_measurement_acceleration_z = ((selftest_z_raw & 0b11100000) >> 3) | ((selftest_a_raw & 0b00000011) >> 0);

    _selftest_measurement_gyro_x = (selftest_x_raw & 0b00011111 );
    _selftest_measurement_gyro_y = (selftest_y_raw & 0b00011111 );
    _selftest_measurement_gyro_z = (selftest_z_raw & 0b00011111 );

    // Disable selftest.
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::GYRO_CONFIG, 
     MPU6050GyroscopeConfiguration::GYROSCOPE_CONFIGURATION_RESET );

    // Read the sensor measurements.
    _read_sensor_data();

    _selftest_gyro_x = _selftest_measurement_gyro_x - _measurement_gyro_x;
    _selftest_gyro_y = _selftest_measurement_gyro_y - _measurement_gyro_y;
    _selftest_gyro_z = _selftest_measurement_gyro_z - _measurement_gyro_z;

    _selftest_acceleration_x = _selftest_measurement_acceleration_x - _measurement_accel_x;
    _selftest_acceleration_y = _selftest_measurement_acceleration_y - _measurement_accel_y;
    _selftest_acceleration_z = _selftest_measurement_acceleration_z - _measurement_accel_z;

    // Calculate the factory trims.
    // gyrox 25 * 131 * 1.046^(XG_TEST-1)
    // gyroy -25 * 131 * 1.046^(YG_TEST-1)
    // gyroz 25 * 131 * 1.046^(ZG_TEST-1)


    /**
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::PWR_MGMT_1, MPU6050PowerManagement1::POWER_MANAGEMENT_1_RESET );
    OS::get_singleton()->delay_msec(100);
    _write_byte_to_device( _i2c_device_address, MPU6050Registers::PWR_MGMT_1, MPU6050PowerManagement1::POWER_MANAGEMENT_1_CLKSEL_AUTO );  
    OS::get_singleton()->delay_msec(200);
    /**/
}


void I2cMpu6050::_self_test_accelerator() {

}



void I2cMpu6050::_read_sensor_data() {
        // Use the normal read sequence:
    //  1. Check Data Ready is set
    //  2. Read measurement data
    //  3. Read ST2 register for overflows

    //uint8_t st1 = _read_byte_from_device( _i2c_device_address, AK8963Registers::ST1 );
    //if( (st1 & AK8963Status1::DATA_IS_READY) == 0 ) return;

    uint8_t data[14] = {0};
    _read_bytes_from_device( _i2c_device_address, MPU6050Registers::ACCEL_XOUT_H, 14, data );

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

    _measurement_gyro_x = (int)gx;
    _measurement_gyro_y = (int)gy;
    _measurement_gyro_z = (int)gz;

    _measurement_accel_x = (int)ax;
    _measurement_accel_y = (int)ay;
    _measurement_accel_z = (int)az;

    //GYRO_XOUT = Gyro_Sensitivity * X_angular_rate
    //Nominal
    //Conditions
    //FS_SEL = 0
    //Gyro_Sensitivity = 131 LSB/( º/s )    

    //TEMP_degC = ((TEMP_OUT –
    //RoomTemp_Offset)/Temp_Sensitivity)
    //+ 21degC
    //temperature_celsius = (((float)temp - _room_temperature_offset)*_one_over_temperature_sensitivity) + 21.0f;
    // todo: add conversion to Kelvin and Fahrenheit.

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

