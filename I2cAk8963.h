#ifndef I2C_AK8963_H_INCLUDED
#define I2C_AK8963_H_INCLUDED

#include "I2cDevice.h"


namespace godot {


// This is a control node for the AK8963 Magnetometer.
class I2cAk8963 : public I2cDevice {
    GDCLASS(I2cAk8963, I2cDevice)

private:
    double _ak8963_update_frame_delay;
    double _ak8963_update_wait_time;
    
    int    _measurement_mode;
    int    _measurement_bits;
    
    float  _calibration_x;
    float  _calibration_y;
    float  _calibration_z;

protected: 
    static void _bind_methods();

    virtual bool _configure_i2c_device() override;
public:

    enum AK8963Registers {
        WIA    = 0x00,  // Device ID (fixed value of 0x48)
        INFO   = 0x01,  // Information
        ST1    = 0x02,  // Status 1 (data status, 0=normal, 1=data is ready)
        HXL    = 0x03,  // Measurement data
        HXH    = 0x04,  
        HYL    = 0x05,
        HYH    = 0x06,
        HZL    = 0x07,
        HZH    = 0x08,
        ST2    = 0x09,  // Status 2 (data status)
        CNTL1  = 0x0A,  // Control 1
        CNTL2  = 0x0B,  // Control 2
        ASTC   = 0x0C,  // Self-test
        TS1    = 0x0D,  // Test 1
        TS2    = 0x0E,  // Test 2
        I2CDIS = 0x0F,  // I2C Disable
        ASAX   = 0x10,  // X-axis sensitivity adjustment value
        ASAY   = 0x11,  // Y-axis sensitivity adjustment value
        ASAZ   = 0x12,  // Z-axis sensitivity adjustment value
        RSV    = 0X13   // Reserved (DO NOT ACCESS)
    };

    enum AK8963Status1 {
        //                                          ||          
        //                                    76543210
        NORMAL                            = 0b00000000,
        DATA_IS_READY                     = 0b00000001,
        DATA_OVERRUN                      = 0b00000010
    };


    
    enum AK8963Control1Mode {
        //                                        ||||          
        //                                    76543210

        POWER_DOWN_MODE                   = 0b00000000,
        SINGLE_MEASUREMENT_MODE           = 0b00000001,
        CONTINUOUS_MEASUREMENT_MODE_1     = 0b00000010, //   8 Hz measurement mode
        CONTINUOUS_MEASUREMENT_MODE_2     = 0b00000110, // 100 Hz measurement mode
        EXTERNAL_TRIGGER_MEASUREMENT_MODE = 0b00000100,
        SELF_TEST_MODE                    = 0b00001000,
        FUSE_ROM_ACCESS_MODE              = 0b00001111
    };

    enum AK8963Control1OutputBitSetting {
        //                                       |          
        //                                    76543210
        OUTPUT_14_BIT                     = 0b00000000,
        OUTPUT_16_BIT                     = 0b00010000
    };

    enum AK8963Control2SoftReset {
        //                                           |          
        //                                    76543210
        NORMAL_NO_RESET                   = 0b00000000,
        RESET                             = 0b00000001
    };

    enum AK8963SelfTestControl {
        //                                        |          
        //                                       76543210
        NORMAL_NO_SELF_TEST                   = 0b00000000,
        GENERATE_MAGNETIC_FIELD_FOR_SELF_TEST = 0b01000000
    };

    enum AK8963I2CDisable {
        //                                    ||||||||          
        //                                    76543210
        DISABLE                           = 0b00011011
    };



    I2cAk8963();
    ~I2cAk8963();

    // Godot virtuals.
    
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    
    // Getters and setters.

    void set_measurement_bits( int measurement_bits );
    int  get_measurement_bits() const;

    void set_measurement_mode( int measurement_mode );
    int  get_measurement_mode() const;

    void  set_magnetic_field_x(float x );
    float get_magnetic_field_x() const;
    
    void  set_magnetic_field_y(float y );
    float get_magnetic_field_y() const;
    
    void  set_magnetic_field_z(float z );
    float get_magnetic_field_z() const;
    

    // Device handling.
    
    void _self_test();

    void _read_sensor_data();

    int   sensor_x, sensor_y, sensor_z;
    float magnetic_field_x, magnetic_field_y, magnetic_field_z;
};




}



#endif 