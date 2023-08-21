#ifndef I2C_BMP280_H_INCLUDED
#define I2C_BMP280_H_INCLUDED

#include "I2cDevice.h"


namespace godot {


// This is a control node for the AK8963 Magnetometer.
class I2cBmp280 : public I2cDevice {
    GDCLASS(I2cBmp280, I2cDevice)

private:
    double _bmp280_update_frame_delay;
    double _bmp280_update_wait_time;
    
    int    _measurement_mode;
    int    _measurement_bits;

    int       _preset_modes_setting;
    uint8_t   _pressure_measurement_oversampling;
    uint8_t   _temperature_measurement_oversampling;
    uint8_t   _filtering_coefficient;
    uint8_t   _power_mode;
    
    int     _pressure_measurement;
    int     _temperature_measurement;

protected: 
    static void _bind_methods();

    virtual void _configure_i2c_device() override;
public:

    enum BMP280Registers {
        TEMP_XLSB = 0xFC,
        TEMP_LSB  = 0xFB,
        TEMP_MSB  = 0xFA,
        PRESS_XLSB = 0xF9,
        PRESS_LSB  = 0xF8,
        PRESS_MSB  = 0xF7,
        CONFIG     = 0xF5,
        CTRL_MEAS  = 0xF4,
        STATUS     = 0xF3,
        RESET      = 0xE0,
        ID         = 0xD0,
        CALIB25    = 0xA1,
        CALIB24    = 0xA0,
        CALIB23    = 0x9F,
        CALIB22    = 0x9E,
        CALIB21    = 0x9D,
        CALIB20    = 0x9C,
        CALIB19    = 0x9B,
        CALIB18    = 0x9A,
        CALIB17    = 0x99,
        CALIB16    = 0x98,
        CALIB15    = 0x97,
        CALIB14    = 0x96,
        CALIB13    = 0x95,
        CALIB12    = 0x94,
        CALIB11    = 0x93,
        CALIB10    = 0x92,
        CALIB09    = 0x91,
        CALIB08    = 0x90,
        CALIB07    = 0x8F,
        CALIB06    = 0x8E,
        CALIB05    = 0x8D,
        CALIB04    = 0x8C,
        CALIB03    = 0x8B,
        CALIB02    = 0x8A,
        CALIB01    = 0x89,
        CALIB00    = 0x88
    };

    enum BMP280Status {
        //                                        |  |          
        //                                    76543210

        NORMAL                            = 0b00000000, // Measurements can be fetched
        MEASURING                         = 0b00000001, // Converting current measurement data
        NVM_DATA_IS_BEING_COPIED          = 0b00001000  // Copying over current measurement data
    };
    
    enum BMP280TemperatureMeasurementSetting {
        //                                    |||          
        //                                    76543210

        TEMPERATURE_MEASUREMENT_SKIPPED   = 0b00000000,
        TEMPERATURE_OVERSAMPLING_X01      = 0b00100000, // resolution 16 bit
        TEMPERATURE_OVERSAMPLING_X02      = 0b01000000, // resolution 17 bit
        TEMPERATURE_OVERSAMPLING_X04      = 0b01100000, // resolution 18 bit
        TEMPERATURE_OVERSAMPLING_X08      = 0b10000000, // resolution 19 bit
        TEMPERATURE_OVERSAMPLING_X16      = 0b10100000  // resolution 20 bit
    };

    enum BMP280PressureMeasurementSetting {
        //                                       |||
        //                                    76543210

        PRESSURE_MEASUREMENT_SKIPPED      = 0b00000000, // Pressure Oversampling Skipped
        ULTRA_LOW_POWER                   = 0b00000100, // Pressure Oversampling x1, resolution 16 bit
        LOW_POWER                         = 0b00001000, // Pressure Oversampling x2, resolution 17 bit
        STANDARD_RESOLUTION               = 0b00001100, // Pressure Oversampling x4, resolution 18 bit
        HIGH_RESOLUTION                   = 0b00010000, // Pressure Oversampling x8, resolution 19 bit
        ULTRA_HIGH_RESOLUTION             = 0b00010100  // Pressure Oversampling x16, resolution 20 bit
    };

    enum BMP280ThreeWireSpiSetting {
        //                                           |          
        //                                    76543210

        ENABLE_3_WIRE_SPI                 = 0b00000001,
        DISABLE_3_WIRE_SPI                = 0b00000000  
    };

    enum BMP280FilterSetting {
        //                                         |||          
        //                                    76543210

        FILTER_OFF                        = 0b00000000,
        FILTER_COEFFICIENT_02             = 0b00000001,
        FILTER_COEFFICIENT_04             = 0b00000010,
        FILTER_COEFFICIENT_08             = 0b00000011,
        FILTER_COEFFICIENT_16             = 0b00000100
    };

    enum BMP280PowerMode {
        //                                          ||          
        //                                    76543210

        SLEEP_POWER_MODE                  = 0b00000000,
        FORCED_POWER_MODE                 = 0b00000001,
        FORCED_POWER_MODE_2               = 0b00000010,
        NORMAL_POWER_MODE                 = 0b00000011
    };

    enum BMP280StandByTime {
        //                                         |||          
        //                                    76543210

        STAND_BY_0_5_MS                   = 0b00000000, // Default
        STAND_BY_62_5_MS                  = 0b00000001,
        STAND_BY_125_MS                   = 0b00000010,
        STAND_BY_250_MS                   = 0b00000011,
        STAND_BY_500_MS                   = 0b00000100,
        STAND_BY_1000_MS                  = 0b00000101,
        STAND_BY_2000_MS                  = 0b00000110,
        STAND_BY_4000_MS                  = 0b00000111

    };


    I2cBmp280();
    ~I2cBmp280();

    // Godot virtuals.
    
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    
    // Getters and setters.

    void set_pressure_measurement_oversampling( int pressure_oversampling );
    int  get_pressure_measurement_oversampling() const;

    void set_temperature_measurement_oversampling( int temperature_oversampling );
    int  get_temperature_measurement_oversampling() const;

    void set_filtering_coefficient( int filtering_coefficient );
    int  get_filtering_coefficient() const;

    void set_power_mode( int power_mode );
    int  get_power_mode() const;

    void set_preset_modes_setting( int preset_modes_setting );
    int  get_preset_modes_setting() const;


    //void set_measurement_bits( int measurement_bits );
    //int  get_measurement_bits() const;

    //void set_measurement_mode( int measurement_mode );
    //int  get_measurement_mode() const;

    //void  set_magnetic_field_x(float x );
    //float get_magnetic_field_x() const;
    
    //void  set_magnetic_field_y(float y );
    //float get_magnetic_field_y() const;
    
    //void  set_magnetic_field_z(float z );
    //float get_magnetic_field_z() const;
    

    // Device handling.
    
    void _self_test();

    void _read_sensor_data();

    int   sensor_x, sensor_y, sensor_z;
    float magnetic_field_x, magnetic_field_y, magnetic_field_z;
};




}



#endif 