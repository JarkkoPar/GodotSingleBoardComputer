#ifndef I2C_MPU9250_H_INCLUDED
#define I2C_MPU9250_H_INCLUDED

#include "I2cDevice.h"
//#include <thread>
//#include <mutex>

namespace godot {

//                                          |          
//                                          FEDCBA9876543210
#define MPU9250_START_CONVERSION          0b1000000000000000
#define MPU9250_PERFORMING_CONVERSION     0b0000000000000000
#define MPU9250_NOT_PERFORMING_CONVERSION 0b1000000000000000



// This is a control node for the MPU9250 Motion sensor.
class I2cMpu9250 : public I2cDevice {
    GDCLASS(I2cMpu9250, I2cDevice)

private:
    double _mpu9520_update_frame_delay;
    double _mpu9250_update_wait_time;
    int    _magnetometer_i2c_address;

    int    _factory_trim_gyro_x;
    int    _factory_trim_gyro_y;
    int    _factory_trim_gyro_z;

    int    _factory_trim_accel_x;
    int    _factory_trim_accel_y;
    int    _factory_trim_accel_z;

    int    _measurement_gyro_x;
    int    _measurement_gyro_y;
    int    _measurement_gyro_z;

    int    _measurement_accel_x;
    int    _measurement_accel_y;
    int    _measurement_accel_z;
    
    float  gyro_x;
    float  gyro_y;
    float  gyro_z;

    float  acceleration_x;
    float  acceleration_y;
    float  acceleration_z;
    
    float  temperature_celsius;
    
    float  _room_temperature_offset;
    float  _temperature_sensitivity;
    float  _one_over_temperature_sensitivity;

    float  temperature_kelvin; // Converted from Celsius.
    float  temperature_fahrenheit; // Converted from Celsius.

protected: 
    static void _bind_methods();

    virtual bool _configure_i2c_device() override;
public:



    enum MPU9250Registers {
        SELF_TEST_X_GYRO  = 0x00,
        SELF_TEST_Y_GYRO  = 0x01,
        SELF_TEST_Z_GYRO  = 0x02,
        SELF_TEST_X_ACCEL = 0x0D,
        SELF_TEST_Y_ACCEL = 0x0E,
        SELF_TEST_Z_ACCEL = 0x0F,
        XG_OFFSET_H       = 0x13,
        XG_OFFSET_L       = 0x14,
        YG_OFFSET_H       = 0x15,
        YG_OFFSET_L       = 0x16,
        ZG_OFFSET_H       = 0x17,
        ZG_OFFSET_L       = 0x18,
        SMPLRT_DIV        = 0x19,
        CONFIG            = 0x1A,
        GYRO_CONFIG       = 0x1B,
        ACCEL_CONFIG      = 0x1C,
        ACCEL_CONFIG2     = 0x1D,
        LP_ACCEL_ODR      = 0x1E,
        WOM_THR           = 0x1F,
        FIFO_EN           = 0x23,
        I2C_MST_CTRL      = 0x24,
        I2C_SLV0_ADDR     = 0x25,
        I2C_SLV0_REG      = 0x26,
        I2C_SLV0_CTRL     = 0x27,
        I2C_SLV1_ADDR     = 0x28,
        I2C_SLV1_REG      = 0x29,
        I2C_SLV1_CTRL     = 0x2A,
        I2C_SLV2_ADDR     = 0x2B,
        I2C_SLV2_REG      = 0x2C,
        I2C_SLV2_CTRL     = 0x2D,
        I2C_SLV3_ADDR     = 0x2E,
        I2C_SLV3_REG      = 0x2F,
        I2C_SLV3_CTRL     = 0x30,
        I2C_SLV4_ADDR     = 0x31,
        I2C_SLV4_REG      = 0x32,
        I2C_SLV4_DO       = 0x33,
        I2C_SLV4_CTRL     = 0x34,
        I2C_SLV4_DI       = 0x35,
        I2C_MST_STATUS    = 0x36,
        INT_PIN_CFG       = 0x37,
        INT_ENABLE        = 0x38,
        INT_STATUS        = 0x3A,
        ACCEL_XOUT_H      = 0x3B,
        ACCEL_XOUT_L      = 0x3C,
        ACCEL_YOUT_H      = 0x3D,
        ACCEL_YOUT_L      = 0x3E,
        ACCEL_ZOUT_H      = 0x3F,
        ACCEL_ZOUT_L      = 0x40,
        TEMP_OUT_H        = 0x41,
        TEMP_OUT_L        = 0x42,
        GYRO_XOUT_H       = 0x43,
        GYRO_XOUT_L       = 0x44,
        GYRO_YOUT_H       = 0x45,
        GYRO_YOUT_L       = 0x46,
        GYRO_ZOUT_H       = 0x47,
        GYRO_ZOUT_L       = 0x48,
        EXT_SENS_DATA_00  = 0x49,
        EXT_SENS_DATA_01  = 0x4A,
        EXT_SENS_DATA_02  = 0x4B,
        EXT_SENS_DATA_03  = 0x4C,
        EXT_SENS_DATA_04  = 0x4D,
        EXT_SENS_DATA_05  = 0x4E,
        EXT_SENS_DATA_06  = 0x4F,
        EXT_SENS_DATA_07  = 0x50,
        EXT_SENS_DATA_08  = 0x51,
        EXT_SENS_DATA_09  = 0x52,
        EXT_SENS_DATA_10  = 0x53,
        EXT_SENS_DATA_11  = 0x54,
        EXT_SENS_DATA_12  = 0x55,
        EXT_SENS_DATA_13  = 0x56,
        EXT_SENS_DATA_14  = 0x57,
        EXT_SENS_DATA_15  = 0x58,
        EXT_SENS_DATA_16  = 0x59,
        EXT_SENS_DATA_17  = 0x5A,
        EXT_SENS_DATA_18  = 0x5B,
        EXT_SENS_DATA_19  = 0x5C,
        EXT_SENS_DATA_20  = 0x5D,
        EXT_SENS_DATA_21  = 0x5E,
        EXT_SENS_DATA_22  = 0x5F,
        EXT_SENS_DATA_23  = 0x60,
        I2C_SLV0_DO       = 0x63,
        I2C_SLV1_DO       = 0x64,
        I2C_SLV2_DO       = 0x65,
        I2C_SLV3_DO       = 0x66,
        I2C_MST_DELAY_CTRL = 0x67,
        SIGNAL_PATH_RESET  = 0x68,
        MOT_DETECT_CTRL    = 0x69,
        USER_CTRL          = 0x6A,
        PWR_MGMT_1         = 0x6B,
        PWR_MGMT_2         = 0x6C,
        FIFO_COUNTH        = 0x72,
        FIFO_COUNTL        = 0x73,
        FIFO_R_W           = 0x74,
        WHO_AM_I           = 0x75,
        XA_OFFSET_H        = 0x77,
        XA_OFFSET_L        = 0x78,
        YA_OFFSET_H        = 0x7A,
        YA_OFFSET_L        = 0x7B,
        ZA_OFFSET_H        = 0x7D,
        ZA_OFFSET_L        = 0x7E
    };

    
    enum ADS1115ComparatorQueueAndDisable {
        //                                           ||          
        //                             FEDCBA9876543210
        ASSERT_AFTER_1_CONVERSIONS = 0b0000000000000000,
        ASSERT_AFTER_2_CONVERSIONS = 0b0000000000000001,
        ASSERT_AFTER_3_CONVERSIONS = 0b0000000000000010,
        DISABLE                    = 0b0000000000000011 // DEFAULT
    };




    I2cMpu9250();
    ~I2cMpu9250();

    // Godot virtuals.
    
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    
    // Getters and setters.

    void set_measurement_gyro_x( int x );
    int  get_measurement_gyro_x() const;
    
    void set_measurement_gyro_y( int y );
    int  get_measurement_gyro_y() const;

    void set_measurement_gyro_z( int z );
    int  get_measurement_gyro_z() const;

    void set_measurement_accel_x( int x );
    int  get_measurement_accel_x() const;
    
    void set_measurement_accel_y( int y );
    int  get_measurement_accel_y() const;

    void set_measurement_accel_z( int z );
    int  get_measurement_accel_z() const;

    void  set_temperature_celsius( float celsius );
    float get_temperature_celsius() const;


    // Device handling.
    
    void _self_test();

    void _read_sensor_data();
};




}



#endif 