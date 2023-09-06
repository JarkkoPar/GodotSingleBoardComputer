#include "I2cMpu9250.h"
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/engine.hpp>


using namespace godot;


// Helper function to find the median. Assumes array size of 5 elements

int find_median( int* array ) {
    int highest_index = 0;
    int lowest_index = 0;
    int middle_index = 1;

    for( int i = 0; i < 5; ++i ) {
        int acc_x = array[i];
        int maxacc_x = array[highest_index];
        int minacc_x = array[lowest_index];

        if( acc_x > maxacc_x ) {
            highest_index = i;
        }
        if( acc_x < minacc_x ) {
            lowest_index = i;
        }
    }
    if( lowest_index == highest_index ) {
        // The values are all equal.
        return array[0];
    }
    int new_array[3];
    int slot = 0;
    for( int i = 0; i < 5; ++i ) {
        if( i != lowest_index && i != highest_index ) {
            new_array[slot] = array[i];
            ++slot;
        }
    }
    lowest_index = 0; 
    highest_index = 0;
    for( int i = 0; i < 3; ++i ) {
        int acc_x = new_array[i];
        int maxacc_x = new_array[highest_index];
        int minacc_x = new_array[lowest_index];

        if( acc_x > maxacc_x ) {
            highest_index = i;
        }
        if( acc_x < minacc_x ) {
            lowest_index = i;
        }
    }
    if( highest_index == lowest_index) {
        // The values are all equal.
        return new_array[0];
    }
    for( int i = 0; i < 3; ++i ) {
        if( i != lowest_index && i != highest_index ){
            return new_array[i];
        }
    }

    return 0;
}


I2cMpu9250::I2cMpu9250() {
    _i2c_device_address = 0x68;
    _magnetometer_i2c_address = 0x0C;

    _measurement_gyro_x = 0;
    _measurement_gyro_y = 0;
    _measurement_gyro_z = 0;

    _measurement_acceleration_x = 0;
    _measurement_acceleration_y = 0;
    _measurement_acceleration_z = 0;

    _acceleration_scale_setting = MPU9250AccelerometerConfiguration::ACCELEROMETER_CONFIGURATION_RESET; // 2G scale
    _gyro_dps_setting = MPU9250GyroscopeConfiguration::GYROSCOPE_CONFIGURATION_RESET; // 250 DPS

    _is_dlpf_enabled = false;

    _pitch_degrees = 0.0f;
    _pitch_radians = 0.0f;

    _roll_degrees = 0.0f;
    _roll_radians = 0.0f;

    _node_filtering_setting = MPU9250NodeFilteringSelection::MEDIAN_FILTER;
    _history_index = 0;
    for( int i = 0; i < 5; ++i ) {
        _history_measurement_acceleration_x[i] = 0;
        _history_measurement_acceleration_y[i] = 0;
        _history_measurement_acceleration_z[i] = 0;

        _history_measurement_gyro_x[i] = 0;
        _history_measurement_gyro_y[i] = 0;
        _history_measurement_gyro_z[i] = 0;
    }
}


I2cMpu9250::~I2cMpu9250() {

}



void I2cMpu9250::_bind_methods() {

    // Settings.

    ClassDB::bind_method(D_METHOD("set_gyro_dps_setting", "gyro_dps_setting"), &I2cMpu9250::set_gyro_dps_setting);
	ClassDB::bind_method(D_METHOD("get_gyro_dps_setting"), &I2cMpu9250::get_gyro_dps_setting);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "gyro_dps_setting", PROPERTY_HINT_ENUM, "250 DPS:0,500 DPS:8,1000 DPS:16,2000 DPS:24"), "set_gyro_dps_setting", "get_gyro_dps_setting");

    ClassDB::bind_method(D_METHOD("set_accelerometer_scale_setting", "accelerometer_scale_setting"), &I2cMpu9250::set_accelerometer_scale_setting);
	ClassDB::bind_method(D_METHOD("get_accelerometer_scale_setting"), &I2cMpu9250::get_accelerometer_scale_setting);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "accelerometer_scale_setting", PROPERTY_HINT_ENUM, "2g:0,4g:8,8g:16,16g:24"), "set_accelerometer_scale_setting", "get_accelerometer_scale_setting");


    // The measurement data.

    ClassDB::bind_method(D_METHOD("set_measurement_gyro_x", "x"), &I2cMpu9250::set_measurement_gyro_x);
	ClassDB::bind_method(D_METHOD("get_measurement_gyro_x"), &I2cMpu9250::get_measurement_gyro_x);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_gyro_x", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_gyro_x", "get_measurement_gyro_x");

    ClassDB::bind_method(D_METHOD("set_measurement_gyro_y", "y"), &I2cMpu9250::set_measurement_gyro_y);
	ClassDB::bind_method(D_METHOD("get_measurement_gyro_y"), &I2cMpu9250::get_measurement_gyro_y);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_gyro_y", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_gyro_y", "get_measurement_gyro_y");

    ClassDB::bind_method(D_METHOD("set_measurement_gyro_z", "z"), &I2cMpu9250::set_measurement_gyro_z);
	ClassDB::bind_method(D_METHOD("get_measurement_gyro_z"), &I2cMpu9250::get_measurement_gyro_z);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_gyro_z", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_gyro_z", "get_measurement_gyro_z");

    ClassDB::bind_method(D_METHOD("set_gyro_x", "x"), &I2cMpu9250::set_gyro_x);
	ClassDB::bind_method(D_METHOD("get_gyro_x"), &I2cMpu9250::get_gyro_x);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gyro_x", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_gyro_x", "get_gyro_x");

    ClassDB::bind_method(D_METHOD("set_gyro_y", "y"), &I2cMpu9250::set_gyro_y);
	ClassDB::bind_method(D_METHOD("get_gyro_y"), &I2cMpu9250::get_gyro_y);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gyro_y", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_gyro_y", "get_gyro_y");

    ClassDB::bind_method(D_METHOD("set_gyro_z", "z"), &I2cMpu9250::set_gyro_z);
	ClassDB::bind_method(D_METHOD("get_gyro_z"), &I2cMpu9250::get_gyro_z);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gyro_z", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_gyro_z", "get_gyro_z");


    ClassDB::bind_method(D_METHOD("set_measurement_acceleration_x", "x"), &I2cMpu9250::set_measurement_acceleration_x);
	ClassDB::bind_method(D_METHOD("get_measurement_acceleration_x"), &I2cMpu9250::get_measurement_acceleration_x);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_acceleration_x", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_acceleration_x", "get_measurement_acceleration_x");

    ClassDB::bind_method(D_METHOD("set_measurement_acceleration_y", "y"), &I2cMpu9250::set_measurement_acceleration_y);
	ClassDB::bind_method(D_METHOD("get_measurement_acceleration_y"), &I2cMpu9250::get_measurement_acceleration_y);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_acceleration_y", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_acceleration_y", "get_measurement_acceleration_y");

    ClassDB::bind_method(D_METHOD("set_measurement_acceleration_z", "z"), &I2cMpu9250::set_measurement_acceleration_z);
	ClassDB::bind_method(D_METHOD("get_measurement_acceleration_z"), &I2cMpu9250::get_measurement_acceleration_z);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "measurement_acceleration_z", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_measurement_acceleration_z", "get_measurement_acceleration_z");

    ClassDB::bind_method(D_METHOD("set_acceleration_x", "x"), &I2cMpu9250::set_acceleration_x);
	ClassDB::bind_method(D_METHOD("get_acceleration_x"), &I2cMpu9250::get_acceleration_x);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration_x", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_acceleration_x", "get_acceleration_x");

    ClassDB::bind_method(D_METHOD("set_acceleration_y", "y"), &I2cMpu9250::set_acceleration_y);
	ClassDB::bind_method(D_METHOD("get_acceleration_y"), &I2cMpu9250::get_acceleration_y);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration_y", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_acceleration_y", "get_acceleration_y");

    ClassDB::bind_method(D_METHOD("set_acceleration_z", "z"), &I2cMpu9250::set_acceleration_z);
	ClassDB::bind_method(D_METHOD("get_acceleration_z"), &I2cMpu9250::get_acceleration_z);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration_z", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_acceleration_z", "get_acceleration_z");

    ClassDB::bind_method(D_METHOD("set_roll_angle_radians", "roll_angle_radians"), &I2cMpu9250::set_roll_angle_radians);
	ClassDB::bind_method(D_METHOD("get_roll_angle_radians"), &I2cMpu9250::get_roll_angle_radians);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "roll_angle_radians", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_roll_angle_radians", "get_roll_angle_radians");

    ClassDB::bind_method(D_METHOD("set_roll_angle_degrees", "roll_angle_degrees"), &I2cMpu9250::set_roll_angle_degrees);
	ClassDB::bind_method(D_METHOD("get_roll_angle_degrees"), &I2cMpu9250::get_roll_angle_degrees);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "roll_angle_degrees", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_roll_angle_degrees", "get_roll_angle_degrees");

    ClassDB::bind_method(D_METHOD("set_pitch_angle_radians", "pitch_angle_radians"), &I2cMpu9250::set_pitch_angle_radians);
	ClassDB::bind_method(D_METHOD("get_pitch_angle_radians"), &I2cMpu9250::get_pitch_angle_radians);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch_angle_radians", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_pitch_angle_radians", "get_pitch_angle_radians");

    ClassDB::bind_method(D_METHOD("set_pitch_angle_degrees", "pitch_angle_degrees"), &I2cMpu9250::set_pitch_angle_degrees);
	ClassDB::bind_method(D_METHOD("get_pitch_angle_degrees"), &I2cMpu9250::get_pitch_angle_degrees);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch_angle_degrees", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_pitch_angle_degrees", "get_pitch_angle_degrees");

    ClassDB::bind_method(D_METHOD("set_temperature_celsius", "celsius"), &I2cMpu9250::set_temperature_celsius);
	ClassDB::bind_method(D_METHOD("get_temperature_celsius"), &I2cMpu9250::get_temperature_celsius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "temperature_celsius", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_temperature_celsius", "get_temperature_celsius");

    ClassDB::bind_method(D_METHOD("set_temperature_kelvin", "kelvin"), &I2cMpu9250::set_temperature_kelvin);
	ClassDB::bind_method(D_METHOD("get_temperature_kelvin"), &I2cMpu9250::get_temperature_kelvin);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "temperature_kelvin", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_temperature_kelvin", "get_temperature_kelvin");

    ClassDB::bind_method(D_METHOD("set_temperature_fahrenheit", "fahrenheit"), &I2cMpu9250::set_temperature_fahrenheit);
	ClassDB::bind_method(D_METHOD("get_temperature_fahrenheit"), &I2cMpu9250::get_temperature_fahrenheit);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "temperature_fahrenheit", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "set_temperature_fahrenheit", "get_temperature_fahrenheit");

}




// Godot virtuals.
    
void I2cMpu9250::_process(double delta) {
    if( _update_method != SBCDeviceUpdateMethod::PROCESS ) return;
    if( !_is_device_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    _read_sensor_data();
}

void I2cMpu9250::_physics_process(double delta) {
    if( _update_method != SBCDeviceUpdateMethod::PHYSICS_PROCESS ) return;
    if( !_is_device_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    _read_sensor_data();
}

void I2cMpu9250::_notification(int p_what) {}

    
// Getters and setters.

void I2cMpu9250::set_gyro_dps_setting( int gyro_dps_setting ) {
    _gyro_dps_setting = gyro_dps_setting;
}

int  I2cMpu9250::get_gyro_dps_setting() const {
    return _gyro_dps_setting;
}

void I2cMpu9250::set_accelerometer_scale_setting( int accelerometer_scale_setting ) {
    _acceleration_scale_setting = accelerometer_scale_setting;
}

int  I2cMpu9250::get_accelerometer_scale_setting() const{
    return _acceleration_scale_setting;
}

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

void I2cMpu9250::set_gyro_x( float x ) {
    _gyro_x = x;
}

float I2cMpu9250::get_gyro_x() const {
    return _gyro_x;
}

void I2cMpu9250::set_gyro_y( float y ) {
    _gyro_x = y;
}

float I2cMpu9250::get_gyro_y() const {
    return _gyro_y;
}

void I2cMpu9250::set_gyro_z( float z ) {
    _gyro_z = z;
}

float I2cMpu9250::get_gyro_z() const {
    return _gyro_z;
}

void I2cMpu9250::set_measurement_acceleration_x( int x ) {
    _measurement_acceleration_x = x;
}

int I2cMpu9250::get_measurement_acceleration_x() const {
    return _measurement_acceleration_x;
}

void I2cMpu9250::set_measurement_acceleration_y( int y ) {
    _measurement_acceleration_x = y;
}

int I2cMpu9250::get_measurement_acceleration_y() const {
    return _measurement_acceleration_y;
}

void I2cMpu9250::set_measurement_acceleration_z( int z ) {
    _measurement_acceleration_z = z;
}

int I2cMpu9250::get_measurement_acceleration_z() const {
    return _measurement_acceleration_z;
}

void I2cMpu9250::set_acceleration_x( float x ) {
    _acceleration_x = x;
}

float I2cMpu9250::get_acceleration_x() const {
    return _acceleration_x;
}

void I2cMpu9250::set_acceleration_y( float y ) {
    _acceleration_x = y;
}

float I2cMpu9250::get_acceleration_y() const {
    return _acceleration_y;
}

void I2cMpu9250::set_acceleration_z( float z ) {
    _acceleration_z = z;
}

float I2cMpu9250::get_acceleration_z() const {
    return _acceleration_z;
}

void I2cMpu9250::set_roll_angle_radians( float roll_angle_radians ){
    _roll_radians = roll_angle_radians;
}

float I2cMpu9250::get_roll_angle_radians() const{
    return _roll_radians;
}

void I2cMpu9250::set_roll_angle_degrees( float roll_angle_degrees ){
    _roll_degrees = roll_angle_degrees;
}

float I2cMpu9250::get_roll_angle_degrees() const{
    return _roll_degrees;
}

void I2cMpu9250::set_pitch_angle_radians( float pitch_angle_radians ){
    _pitch_radians = pitch_angle_radians;
}

float I2cMpu9250::get_pitch_angle_radians() const{
    return _pitch_radians;
}

void I2cMpu9250::set_pitch_angle_degrees( float pitch_angle_degrees ){
    _pitch_degrees = pitch_angle_degrees;
}

float I2cMpu9250::get_pitch_angle_degrees() const{
    return _pitch_degrees;
}


void I2cMpu9250::set_temperature_celsius( float celsius ) {
    _temperature_celsius = celsius;
}

float I2cMpu9250::get_temperature_celsius() const {
    return _temperature_celsius;
}

void I2cMpu9250::set_temperature_kelvin( float kelvin ) {
    _temperature_kelvin = kelvin;
}

float I2cMpu9250::get_temperature_kelvin() const {
    return _temperature_kelvin;
}

void I2cMpu9250::set_temperature_fahrenheit( float fahrenheit ) {
    _temperature_fahrenheit = fahrenheit;
}

float I2cMpu9250::get_temperature_fahrenheit() const {
    return _temperature_fahrenheit;
}



// Device handling.

bool I2cMpu9250::_configure_i2c_device() {
    // Start by resetting the device to get it to defaults and waking it up.
    _write_byte_to_device( _i2c_device_address, MPU9250Registers::PWR_MGMT_1, MPU9250PowerManagement1::POWER_MANAGEMENT_1_RESET );
    OS::get_singleton()->delay_msec(100);
    _write_byte_to_device( _i2c_device_address, MPU9250Registers::PWR_MGMT_1, MPU9250PowerManagement1::POWER_MANAGEMENT_1_CLKSEL_AUTO );  
    OS::get_singleton()->delay_msec(200);

    //_write_byte_to_device( _i2c_device_address, MPU9250Registers::)
    //_write_byte_to_device( _i2c_device_address, MPU9250Registers::GYRO_CONFIG, MPU9250GyroscopeConfiguration::Fchoice_b_GYRO_USE_DLPF_CFG );
    //_write_byte_to_device( _i2c_device_address, MPU9250Registers::CONFIG, 0x03 );
    //_write_byte_to_device( _i2c_device_address, MPU9250Registers::SMPLRT_DIV, 0x03 );


    // Configure the accelerometer, gyroscope and the magnetometer (to do).
    _write_byte_to_device( _i2c_device_address, MPU9250Registers::ACCEL_CONFIG, _acceleration_scale_setting ); // MPU9250AccelerometerConfiguration::ACCEL_FS_SEL_16G  );
    _write_byte_to_device( _i2c_device_address, MPU9250Registers::GYRO_CONFIG, _gyro_dps_setting ); // MPU9250GyroscopeConfiguration::XGYRO_FS_SEL_1000_DPS );    
    
    // Set the digital low pass filter to 20 Hz.
    _write_byte_to_device( _i2c_device_address, MPU9250Registers::CONFIG, MPU9250Configuration::DLPF_CFG_G020HZ_T020HZ );

    // Enable bypass to allow additional sensors to be 
    // controlled through the same i2c bus through Godot.
    _write_byte_to_device( _i2c_device_address, MPU9250Registers::INT_PIN_CFG, MPU9250IntPinCfgSettings::BYPASS_EN | MPU9250IntPinCfgSettings::INT_ANYRD_2CLEAR ); 
    _write_byte_to_device( _i2c_device_address, MPU9250Registers::INT_ENABLE, 0x01 );
    OS::get_singleton()->delay_msec(100);

    return true;
}


void I2cMpu9250::_self_test() {

}


void I2cMpu9250::_read_sensor_data() {

    uint8_t data[14] = {0};
    _read_bytes_from_device( _i2c_device_address, MPU9250Registers::ACCEL_XOUT_H, 14, data );

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

    _measurement_acceleration_x = (int)ax;
    _measurement_acceleration_y = (int)ay;
    _measurement_acceleration_z = (int)az;

    if( _node_filtering_setting == MPU9250NodeFilteringSelection::MEDIAN_FILTER ){
        // Store the measurement for the filtering.
        _history_measurement_gyro_x[_history_index] = _measurement_gyro_x;
        _history_measurement_gyro_y[_history_index] = _measurement_gyro_y;
        _history_measurement_gyro_z[_history_index] = _measurement_gyro_z;
        _history_measurement_acceleration_x[_history_index] = _measurement_acceleration_x;
        _history_measurement_acceleration_y[_history_index] = _measurement_acceleration_y;
        _history_measurement_acceleration_z[_history_index] = _measurement_acceleration_z;
        ++_history_index;
        if( _history_index > 4 ) _history_index = 0;

        /**
        // Get the median value and use that for the measurement value.
        int _median_acc_x[5];// = {_history_measurement_acceleration_x[0], 0,0,0,0};
        int _median_acc_y[5];// = {_history_measurement_acceleration_y[0], 0,0,0,0};
        int _median_acc_z[5];// = {_history_measurement_acceleration_z[0], 0,0,0,0};
        int _median_gyro_x[5];// = {_history_measurement_gyro_x[0], 0,0,0,0};
        int _median_gyro_y[5];// = {_history_measurement_gyro_y[0], 0,0,0,0};
        int _median_gyro_z[5];// = {_history_measurement_gyro_z[0], 0,0,0,0};
        memcpy( _median_acc_x, _history_measurement_acceleration_x, 5 );
        **/
        _measurement_acceleration_x = find_median(_history_measurement_acceleration_x);
        _measurement_acceleration_y = find_median(_history_measurement_acceleration_y);
        _measurement_acceleration_z = find_median(_history_measurement_acceleration_z);
        _measurement_gyro_x = find_median(_history_measurement_gyro_x);
        _measurement_gyro_y = find_median(_history_measurement_gyro_y);
        _measurement_gyro_z = find_median(_history_measurement_gyro_z);
    }

    // Calculate the actual values based on resolution settings.
    float g_resolution = 0.0f;
    if( _gyro_dps_setting == MPU9250GyroscopeConfiguration::GYROSCOPE_CONFIGURATION_RESET ) {
        g_resolution = 250.0f / 32768.0f;
    } else if( _gyro_dps_setting == MPU9250GyroscopeConfiguration::XGYRO_FS_SEL_500_DPS ) {
        g_resolution = 500.0f / 32768.0f;
    } else if( _gyro_dps_setting == MPU9250GyroscopeConfiguration::XGYRO_FS_SEL_1000_DPS ) {
        g_resolution = 1000.0f / 32768.0f;
    } else {
        g_resolution = 2000.0f / 32768.0f;
    }
    _gyro_x = _measurement_gyro_x * g_resolution;
    _gyro_y = _measurement_gyro_y * g_resolution;
    _gyro_z = _measurement_gyro_z * g_resolution;


    float a_scale = 0.0f;
    if( _acceleration_scale_setting == MPU9250AccelerometerConfiguration::ACCELEROMETER_CONFIGURATION_RESET ) {
        a_scale = 2.0f / 32768.0f;
    } else if( _acceleration_scale_setting == MPU9250AccelerometerConfiguration::ACCEL_FS_SEL_4G ) {
        a_scale = 4.0f / 32768.0f;
    } else if( _acceleration_scale_setting == MPU9250AccelerometerConfiguration::ACCEL_FS_SEL_8G ) {
        a_scale = 8.0f / 32768.0f;
    } else {
        a_scale = 16.0f / 32768.0f;
    }

    _acceleration_x = _measurement_acceleration_x * a_scale;
    _acceleration_y = _measurement_acceleration_y * a_scale;
    _acceleration_z = _measurement_acceleration_z * a_scale;

    // Calculate the roll and pitch.

    _roll_radians = 0.0f;
    _roll_degrees = 0.0f;
    _pitch_radians = 0.0f;
    _pitch_degrees = 0.0f; 
    float acc_x_pow_2 = _acceleration_x * _acceleration_x;
    float acc_y_pow_2 = _acceleration_y * _acceleration_y;
    float acc_z_pow_2 = _acceleration_z * _acceleration_z;
    float acc_x_pow_2_plus_acc_z_pow_2 = acc_x_pow_2 + acc_z_pow_2;
    float acc_y_pow_2_plus_acc_z_pow_2 = acc_y_pow_2 + acc_z_pow_2;
    
    if( acc_x_pow_2_plus_acc_z_pow_2 != 0.0f ) 
    {
        _roll_radians = Math::atan( _acceleration_y / Math::sqrt(acc_x_pow_2_plus_acc_z_pow_2) );
        _roll_degrees = Math::rad_to_deg(_roll_radians);
    }
    
    if( acc_y_pow_2_plus_acc_z_pow_2 != 0.0f ) 
    {
        _pitch_radians = Math::atan( _acceleration_x / Math::sqrt(acc_y_pow_2_plus_acc_z_pow_2) );
        _pitch_degrees = Math::rad_to_deg(_pitch_radians);
    }
    
    
    _temperature_celsius = (((float)temp - _room_temperature_offset)*_one_over_temperature_sensitivity) + 21.0f;
    _temperature_kelvin     = _temperature_celsius + 273.15f;
    _temperature_fahrenheit = _temperature_celsius * 1.8f + 32.0f;
    
}

