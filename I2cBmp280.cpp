#include "I2cBmp280.h"

#include <godot_cpp/classes/engine.hpp>

using namespace godot;


I2cBmp280::I2cBmp280() {
    _i2c_device_address = 0x76;
    _preset_modes_setting = 0;
    _temperature_measurement_oversampling = BMP280TemperatureMeasurementSetting::TEMPERATURE_OVERSAMPLING_X01;
    _pressure_measurement_oversampling = BMP280PressureMeasurementSetting::STANDARD_RESOLUTION;
    _filtering_coefficient = BMP280FilterSetting::FILTER_OFF;
    _power_mode = BMP280PowerMode::NORMAL_POWER_MODE;
}

I2cBmp280::~I2cBmp280() {
    
}


void I2cBmp280::_bind_methods() {
    // Preset modes setting.
    ClassDB::bind_method(D_METHOD("set_preset_modes_setting", "preset_modes_setting"), &I2cBmp280::set_preset_modes_setting);
	ClassDB::bind_method(D_METHOD("get_preset_modes_setting"), &I2cBmp280::get_preset_modes_setting);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "preset_modes_setting", PROPERTY_HINT_ENUM, "Custom_settings:0,Handheld_device_low_power:1,Handheld_device_dynamic:2,Weather_monitoring:3,Elevator_or_floor_change_detection:4,Drop_detection:5,Indoor_navigation:6"), "set_preset_modes_setting", "get_preset_modes_setting");


    // Pressure measurement oversampling.
    ClassDB::bind_method(D_METHOD("set_pressure_measurement_oversampling", "pressure_oversamplig"), &I2cBmp280::set_pressure_measurement_oversampling);
	ClassDB::bind_method(D_METHOD("get_pressure_measurement_oversampling"), &I2cBmp280::get_pressure_measurement_oversampling);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "pressure_measurement_oversampling", PROPERTY_HINT_ENUM, "Measurement_Skipped:0,Ultra_Low_Power:4,Low_Power:8,Standard_Resolution:12,High_Resolution:16,Ultra_High_Resolution:20"), "set_pressure_measurement_oversampling", "get_pressure_measurement_oversampling");

    // Temperature measurement oversampling.
    ClassDB::bind_method(D_METHOD("set_temperature_measurement_oversampling", "temperature_oversamplig"), &I2cBmp280::set_temperature_measurement_oversampling);
	ClassDB::bind_method(D_METHOD("get_temperature_measurement_oversampling"), &I2cBmp280::get_temperature_measurement_oversampling);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "temperature_measurement_oversampling", PROPERTY_HINT_ENUM, "Measurement_Skipped:0,Oversampling_X1:32,Oversampling_X2:64,Oversampling_X4:96,Oversampling_X8:128,Oversampling_X16:160"), "set_temperature_measurement_oversampling", "get_temperature_measurement_oversampling");

    // Filtering coefficient.
    ClassDB::bind_method(D_METHOD("set_filtering_coefficient", "filtering_coefficient"), &I2cBmp280::set_filtering_coefficient);
	ClassDB::bind_method(D_METHOD("get_filtering_coefficient"), &I2cBmp280::get_filtering_coefficient);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "filtering_coefficient", PROPERTY_HINT_ENUM, "Filtering_OFF:0,Coefficient_2:1,Coefficient_4:2,Coefficient_8:3,Coefficient_16:4"), "set_filtering_coefficient", "get_filtering_coefficient");

    // Power mode.
    ClassDB::bind_method(D_METHOD("set_power_mode", "power_mode"), &I2cBmp280::set_power_mode);
	ClassDB::bind_method(D_METHOD("get_power_mode"), &I2cBmp280::get_power_mode);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "power_mode", PROPERTY_HINT_ENUM, "Sleep:0,Forced:1,Normal:3"), "set_power_mode", "get_power_mode");


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
    //_write_byte_to_device(_i2c_device_address, BMP280Registers::CONFIG, BMP280StandByTime::STAND_BY_0_5_MS|BMP280FilterSetting::FILTER_COEFFICIENT_02|BMP280ThreeWireSpiSetting::DISABLE_3_WIRE_SPI);
    //_write_byte_to_device(_i2c_device_address, BMP280Registers::CTRL_MEAS, BMP280PressureMeasurementSetting::ULTRA_LOW_POWER|BMP280TemperatureMeasurementSetting::TEMPERATURE_OVERSAMPLING_X01|BMP280PowerMode::NORMAL_POWER_MODE);

    _write_byte_to_device(_i2c_device_address, BMP280Registers::CONFIG, BMP280StandByTime::STAND_BY_0_5_MS|_filtering_coefficient|BMP280ThreeWireSpiSetting::DISABLE_3_WIRE_SPI);
    _write_byte_to_device(_i2c_device_address, BMP280Registers::CTRL_MEAS, _pressure_measurement_oversampling|_temperature_measurement_oversampling|_power_mode);

} 

void I2cBmp280::_self_test() {

}

void I2cBmp280::_read_sensor_data() {
    uint8_t status = _read_byte_from_device(_i2c_device_address, BMP280Registers::STATUS );
    if( status & BMP280Status::MEASURING || status & BMP280Status::NVM_DATA_IS_BEING_COPIED ) return;

    uint8_t data[6] = {0};
    _read_bytes_from_device(_i2c_device_address, BMP280Registers::PRESS_MSB, 6, data );

    // Todo: Get the measurement based on bits used.
    //if( )
    uint32_t pressure_measurement = (uint32_t)data[6];


}


// Getters and setters.

void I2cBmp280::set_pressure_measurement_oversampling( int pressure_oversampling ) {
    _pressure_measurement_oversampling = (uint8_t)pressure_oversampling;
    _preset_modes_setting = 0; // Fall back to custom settings.
}

int  I2cBmp280::get_pressure_measurement_oversampling() const {
   return  (int)_pressure_measurement_oversampling;
}

void I2cBmp280::set_temperature_measurement_oversampling( int temperature_oversampling ) {
    _temperature_measurement_oversampling = (uint8_t)temperature_oversampling;
    _preset_modes_setting = 0; // Fall back to custom settings.
}

int  I2cBmp280::get_temperature_measurement_oversampling() const {
   return  (int)_temperature_measurement_oversampling;
}

void I2cBmp280::set_filtering_coefficient( int filtering_coefficient ) {
    _filtering_coefficient = (uint8_t)filtering_coefficient;
    _preset_modes_setting = 0; // Fall back to custom settings.
}

int  I2cBmp280::get_filtering_coefficient() const {
    return (int)_filtering_coefficient;
}

void I2cBmp280::set_power_mode( int power_mode ) {
    _power_mode = (uint8_t)power_mode;
    _preset_modes_setting = 0; // Fall back to custom settings.
}

int  I2cBmp280::get_power_mode() const {
    return (int)_power_mode;
}

void I2cBmp280::set_preset_modes_setting( int preset_modes_setting ) {
    _preset_modes_setting = preset_modes_setting;
    switch( _preset_modes_setting ) {
        case 1: // Handheld device low-power.
        {
            _power_mode = BMP280PowerMode::NORMAL_POWER_MODE;
            _pressure_measurement_oversampling = BMP280PressureMeasurementSetting::ULTRA_HIGH_RESOLUTION;
            _temperature_measurement_oversampling = BMP280TemperatureMeasurementSetting::TEMPERATURE_OVERSAMPLING_X02;
            _filtering_coefficient = BMP280FilterSetting::FILTER_COEFFICIENT_04;
        }
        break;
        case 2: // Handheld device dynamic.
        {
            _power_mode = BMP280PowerMode::NORMAL_POWER_MODE;
            _pressure_measurement_oversampling = BMP280PressureMeasurementSetting::STANDARD_RESOLUTION;
            _temperature_measurement_oversampling = BMP280TemperatureMeasurementSetting::TEMPERATURE_OVERSAMPLING_X01;
            _filtering_coefficient = BMP280FilterSetting::FILTER_COEFFICIENT_16;
        }
        break;
        case 3: // Weather monitoring.
        {
            _power_mode = BMP280PowerMode::FORCED_POWER_MODE;
            _pressure_measurement_oversampling = BMP280PressureMeasurementSetting::ULTRA_LOW_POWER;
            _temperature_measurement_oversampling = BMP280TemperatureMeasurementSetting::TEMPERATURE_OVERSAMPLING_X01;
            _filtering_coefficient = BMP280FilterSetting::FILTER_OFF;
        }
        break;
        case 4: // Elevator / floor change detection.
        {
            _power_mode = BMP280PowerMode::NORMAL_POWER_MODE;
            _pressure_measurement_oversampling = BMP280PressureMeasurementSetting::STANDARD_RESOLUTION;
            _temperature_measurement_oversampling = BMP280TemperatureMeasurementSetting::TEMPERATURE_OVERSAMPLING_X01;
            _filtering_coefficient = BMP280FilterSetting::FILTER_COEFFICIENT_04;
        }
        break;
        case 5: // Drop detection.
        {
            _power_mode = BMP280PowerMode::NORMAL_POWER_MODE;
            _pressure_measurement_oversampling = BMP280PressureMeasurementSetting::LOW_POWER;
            _temperature_measurement_oversampling = BMP280TemperatureMeasurementSetting::TEMPERATURE_OVERSAMPLING_X01;
            _filtering_coefficient = BMP280FilterSetting::FILTER_OFF;
        }
        break;
        case 6: // Indoor navigation.
        {
            _power_mode = BMP280PowerMode::NORMAL_POWER_MODE;
            _pressure_measurement_oversampling = BMP280PressureMeasurementSetting::ULTRA_HIGH_RESOLUTION;
            _temperature_measurement_oversampling = BMP280TemperatureMeasurementSetting::TEMPERATURE_OVERSAMPLING_X02;
            _filtering_coefficient = BMP280FilterSetting::FILTER_COEFFICIENT_16;
        }
        break;
        default: break;
        
    }
}

int  I2cBmp280::get_preset_modes_setting() const {
    return _preset_modes_setting;
}
    

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

