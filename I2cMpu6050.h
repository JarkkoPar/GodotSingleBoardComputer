#ifndef I2C_MPU6050_H_INCLUDED
#define I2C_MPU6050_H_INCLUDED

#include "I2cDevice.h"
//#include <thread>
//#include <mutex>

namespace godot {

//                                          |          
//                                          FEDCBA9876543210
//#define MPU9250_START_CONVERSION          0b1000000000000000
//#define MPU9250_PERFORMING_CONVERSION     0b0000000000000000
//#define MPU9250_NOT_PERFORMING_CONVERSION 0b1000000000000000



// This is a control node for the MPU9250 Motion sensor.
class I2cMpu6050 : public I2cDevice {
    GDCLASS(I2cMpu6050, I2cDevice)

private:
    double _mpu6020_update_frame_delay;
    double _mpu6050_update_wait_time;
    
    uint8_t _selftest_measurement_gyro_x;
    uint8_t _selftest_measurement_gyro_y;
    uint8_t _selftest_measurement_gyro_z;

    uint8_t _selftest_measurement_acceleration_x;
    uint8_t _selftest_measurement_acceleration_y;
    uint8_t _selftest_measurement_acceleration_z;

    int    _selftest_gyro_x;
    int    _selftest_gyro_y;
    int    _selftest_gyro_z;

    int    _selftest_acceleration_x;
    int    _selftest_acceleration_y;
    int    _selftest_acceleration_z;

    float  _factory_trim_gyro_x;
    float  _factory_trim_gyro_y;
    float  _factory_trim_gyro_z;

    float  _factory_trim_accel_x;
    float  _factory_trim_accel_y;
    float  _factory_trim_accel_z;

    int    _measurement_gyro_x;
    int    _measurement_gyro_y;
    int    _measurement_gyro_z;

    int    _measurement_acceleration_x;
    int    _measurement_acceleration_y;
    int    _measurement_acceleration_z;

    uint8_t _acceleration_scale_setting;
    uint8_t _gyro_dps_setting;
    
    float  _gyro_x;
    float  _gyro_y;
    float  _gyro_z;

    float  _acceleration_x;
    float  _acceleration_y;
    float  _acceleration_z;
    
    bool    _is_dlpf_enabled;

    
    float  _temperature_celsius;
    
    float  _room_temperature_offset;
    float  _temperature_sensitivity;
    float  _one_over_temperature_sensitivity;

    float  _temperature_kelvin; // Converted from Celsius.
    float  _temperature_fahrenheit; // Converted from Celsius.
    
protected: 
    static void _bind_methods();

    virtual bool _configure_i2c_device() override;
public:



    enum MPU6050Registers {
        SELF_TEST_X        = 0x0D,
        SELF_TEST_Y        = 0x0E,
        SELF_TEST_Z        = 0x0F,
        SELF_TEST_A        = 0x10,
        SMPLRT_DIV         = 0x19,
        CONFIG             = 0x1A,
        GYRO_CONFIG        = 0x1B,
        ACCEL_CONFIG       = 0x1C,
        FIFO_EN            = 0x23,
        I2C_MST_CTRL       = 0x24,
        I2C_SLV0_ADDR      = 0x25,
        I2C_SLV0_REG       = 0x26,
        I2C_SLV0_CTRL      = 0x27,
        I2C_SLV1_ADDR      = 0x28,
        I2C_SLV1_REG       = 0x29,
        I2C_SLV1_CTRL      = 0x2A,
        I2C_SLV2_ADDR      = 0x2B,
        I2C_SLV2_REG       = 0x2C,
        I2C_SLV2_CTRL      = 0x2D,
        I2C_SLV3_ADDR      = 0x2E,
        I2C_SLV3_REG       = 0x2F,
        I2C_SLV3_CTRL      = 0x30,
        I2C_SLV4_ADDR      = 0x31,
        I2C_SLV4_REG       = 0x32,
        I2C_SLV4_DO        = 0x33,
        I2C_SLV4_CTRL      = 0x34,
        I2C_SLV4_DI        = 0x35,
        I2C_MST_STATUS     = 0x36,
        INT_PIN_CFG        = 0x37,
        INT_ENABLE         = 0x38,
        INT_STATUS         = 0x3A,
        ACCEL_XOUT_H       = 0x3B,
        ACCEL_XOUT_L       = 0x3C,
        ACCEL_YOUT_H       = 0x3D,
        ACCEL_YOUT_L       = 0x3E,
        ACCEL_ZOUT_H       = 0x3F,
        ACCEL_ZOUT_L       = 0x40,
        TEMP_OUT_H         = 0x41,
        TEMP_OUT_L         = 0x42,
        GYRO_XOUT_H        = 0x43,
        GYRO_XOUT_L        = 0x44,
        GYRO_YOUT_H        = 0x45,
        GYRO_YOUT_L        = 0x46,
        GYRO_ZOUT_H        = 0x47,
        GYRO_ZOUT_L        = 0x48,
        EXT_SENS_DATA_00   = 0x49,
        EXT_SENS_DATA_01   = 0x4A,
        EXT_SENS_DATA_02   = 0x4B,
        EXT_SENS_DATA_03   = 0x4C,
        EXT_SENS_DATA_04   = 0x4D,
        EXT_SENS_DATA_05   = 0x4E,
        EXT_SENS_DATA_06   = 0x4F,
        EXT_SENS_DATA_07   = 0x50,
        EXT_SENS_DATA_08   = 0x51,
        EXT_SENS_DATA_09   = 0x52,
        EXT_SENS_DATA_10   = 0x53,
        EXT_SENS_DATA_11   = 0x54,
        EXT_SENS_DATA_12   = 0x55,
        EXT_SENS_DATA_13   = 0x56,
        EXT_SENS_DATA_14   = 0x57,
        EXT_SENS_DATA_15   = 0x58,
        EXT_SENS_DATA_16   = 0x59,
        EXT_SENS_DATA_17   = 0x5A,
        EXT_SENS_DATA_18   = 0x5B,
        EXT_SENS_DATA_19   = 0x5C,
        EXT_SENS_DATA_20   = 0x5D,
        EXT_SENS_DATA_21   = 0x5E,
        EXT_SENS_DATA_22   = 0x5F,
        EXT_SENS_DATA_23   = 0x60,
        I2C_SLV0_DO        = 0x63,
        I2C_SLV1_DO        = 0x64,
        I2C_SLV2_DO        = 0x65,
        I2C_SLV3_DO        = 0x66,
        I2C_MST_DELAY_CTRL = 0x67,
        SIGNAL_PATH_RESET  = 0x68,
        MOT_DETECT_CTRL    = 0x69,
        USER_CTRL          = 0x6A,
        PWR_MGMT_1         = 0x6B, // Reset value 0x40
        PWR_MGMT_2         = 0x6C,
        FIFO_COUNTH        = 0x72,
        FIFO_COUNTL        = 0x73,
        FIFO_R_W           = 0x74,
        WHO_AM_I           = 0x75, // Reset value 0x68
    };

    /**
    XG_OFFSET_H       = 0x13,
        XG_OFFSET_L       = 0x14,
        YG_OFFSET_H       = 0x15,
        YG_OFFSET_L       = 0x16,
        ZG_OFFSET_H       = 0x17,
        ZG_OFFSET_L       = 0x18,
        ACCEL_CONFIG2     = 0x1D,
        LP_ACCEL_ODR      = 0x1E,
        WOM_THR           = 0x1F,
        */

    enum MPU6050Configuration {
        //                                  |||          
        //                             76543210

        FIFO_MODE                  = 0b01000000, // When set to 1, when the fifo is full, additional writes will not be written to FIFO. If 0, new writes will be written and old values will get replaced. 
        EXT_SYNC_SET_1             = 0b00001000, // TEMP_OUT_L[0]
        EXT_SYNC_SET_2             = 0b00010000, // GYRO_XOUT_L[0]
        EXT_SYNC_SET_3             = 0b00011000, // GYRO_YOUT_L[0]
        EXT_SYNC_SET_4             = 0b00100000, // GYRO_ZOUT_L[0]
        EXT_SYNC_SET_5             = 0b00101000, // ACCEL_XOUT_L[0]
        EXT_SYNC_SET_6             = 0b00110000, // ACCEL_YOUT_L[0]
        EXT_SYNC_SET_7             = 0b00111000, // ACCEL_ZOUT_L[0]
        DLPF_CFG                   = 0b00000011, // Use DLPF, 
        CONFIGURATION_RESET        = 0b00000000
    };

    enum MPU6050GyroscopeConfiguration {
        //                             ||||| ||          
        //                             76543210

        XG_ST                      = 0b10000000, // X Gyro self-test
        YG_ST                      = 0b01000000, // Y Gyro self-test
        ZG_ST                      = 0b00100000, // Z Gyro self-test
        //XGYRO_FS_SEL_250_DPS       = 0b00000000,  
        XGYRO_FS_SEL_500_DPS       = 0b00001000, // Gyro full-scale select 
        XGYRO_FS_SEL_1000_DPS      = 0b00010000,  
        XGYRO_FS_SEL_2000_DPS      = 0b00011000,  
        // Bit 2 is reserved
        //Fchoice_b_GYRO_8800_HZ_TEMPERATURE_4000_HZ = 0b00000011, // Used to bypass DLPF 
        //Fchoice_b_GYRO_3600_HZ_TEMPERATURE_4000_HZ = 0b00000010, // Used to bypass DLPF 
        //Fchoice_b_GYRO_USE_DLPF_CFG = 0b00000000, // Used to bypass DLPF 

        GYROSCOPE_CONFIGURATION_RESET = 0b00000000
    };

    enum MPU6050AccelerometerConfiguration {
        //                             |||||          
        //                             76543210

        ax_st_en                   = 0b10000000, 
        ay_st_en                   = 0b01000000, 
        az_st_en                   = 0b00100000,
        
        // ACCEL_FS_SEL_2G is selected by default.
        // ACCEL_FS_SEL_2G         = 0b00000000,
        ACCEL_FS_SEL_4G            = 0b00001000,
        ACCEL_FS_SEL_8G            = 0b00010000,
        ACCEL_FS_SEL_16G           = 0b00011000,

        ACCELEROMETER_CONFIGURATION_RESET = 0b00000000
        
    };

    enum MPU6050AccelerometerConfiguration2 {
        //                               ||||||          
        //                             76543210

        accel_fchoice_b            = 0b00001000, 
        
        // A_DLPFCFG_0 is selected by default.
        // A_DLPFCFG_0             = 0b00000000,
        A_DLPFCFG_F_CHOICE         = 0b00000100,
        A_DLPFCFG_1                = 0b00000001,
        A_DLPFCFG_2                = 0b00000010,
        A_DLPFCFG_3                = 0b00000011,
        A_DLPFCFG_4                = 0b00000100,
        A_DLPFCFG_5                = 0b00000101,
        A_DLPFCFG_6                = 0b00000110,
        A_DLPFCFG_7                = 0b00000111,
        ACCELEROMETER_CONFIGURATION2_RESET = 0b00000000
        
    };


    enum MPU6050I2cMasterDelayControl {
        //                                  |||          
        //                             76543210

        DELAY_ES_SHADOW            = 0b10000000, // Delays shadowing of external sensor data until all data is received
        // bit 6 and 5 are reserved
        I2C_LSV4_DLY_EN            = 0b00010000, // When enabled, slave 4 will only be accessed 1+I2C_MST_DLY samples as determined by SMPLRT_DIV_ and DLPF_CFG.
        I2C_LSV3_DLY_EN            = 0b00001000, // When enabled, slave 3 will only be accessed 1+I2C_MST_DLY samples as determined by SMPLRT_DIV_ and DLPF_CFG.
        I2C_LSV2_DLY_EN            = 0b00000100, // When enabled, slave 2 will only be accessed 1+I2C_MST_DLY samples as determined by SMPLRT_DIV_ and DLPF_CFG.
        I2C_LSV1_DLY_EN            = 0b00000010, // When enabled, slave 1 will only be accessed 1+I2C_MST_DLY samples as determined by SMPLRT_DIV_ and DLPF_CFG.
        I2C_LSV0_DLY_EN            = 0b00000001, // When enabled, slave 0 will only be accessed 1+I2C_MST_DLY samples as determined by SMPLRT_DIV_ and DLPF_CFG.
        MASTER_DELAY_CONTROL_RESET = 0b00000000
    };

    
    enum MPU6050SignalPathReset {
        //                                  |||          
        //                             76543210

        GYRO_RST                   = 0b00000100, // Reset the gyros
        ACCEL_RST                  = 0b00000010, // Reset the accelerometers
        TEMP_RST                   = 0b00000001  // Reset the temperature sensor
    };

    enum MPU6050AccelIntelCtrl {
        //                             ||          
        //                             76543210

        ACCEL_INTEL_EN             = 0b10000000, // Enables the Wake-on-Motion detection logic
        ACCEL_INTEL_MODE           = 0b01000000, // This will define the mode to compare current sample with the previous sample
        ACCEL_INTEL_RESET          = 0b00000000
    };

    enum MPU6050PowerManagement1 {
        //                                           ||||||||          
        //                                           76543210

        POWER_MANAGEMENT_1_RESET                 = 0b10000000, // Reset and clear all to default settings
        POWER_MANAGEMENT_1_SLEEP                 = 0b01000000,
        POWER_MANAGEMENT_1_CYCLE                 = 0b00100000,
        POWER_MANAGEMENT_1_GYRO_STANDBY          = 0b00010000,
        POWER_MANAGEMENT_1_PD_PTAT               = 0b00001000,
        POWER_MANAGEMENT_1_CLKSEL_AUTO           = 0b00000001, // Auto-select the best available clock source, PLL if ready, else use the internal oscillator
        POWER_MANAGEMENT_1_CLKSEL_INTERNAL_20MHZ = 0b00000110, // The internal 20 Mhz oscillator
        POWER_MANAGEMENT_1_CLKSEL_STOP_AND_KEEP_RESET = 0b00000111, // Stops the clock and keeps timing generator in reset
        POWER_MANAGEMENT_1_WAKE                  = 0b00000000  // Defaults to the internal 20 Mhz oscillator
    };


    enum MPU6050PowerManagement2 {
        //                               ||||||          
        //                             76543210

        DISABLE_XA                 = 0b00100000, // X accelerometer is disabled
        DISABLE_YA                 = 0b00010000, // Y accelerometer is disabled
        DISABLE_ZA                 = 0b00001000, // Z accelerometer is disabled
        DISABLE_XG                 = 0b00000100, // X gyro is disabled
        DISABLE_YG                 = 0b00000010, // Y gyro is disabled
        DISABLE_ZG                 = 0b00000001, // Z gyro is disabled
        POWER_MANAGEMENT_2_RESET   = 0b00000000  // Reset sets all on (to bit 0).
    };

    enum MPU6050IntPinCfgSettings {
        //                             |||||||          
        //                             76543210

        BYPASS_EN                  = 0b00000010, // When asserted, the i2c_master interface pins (ES_CL and ES_DA) will go into "bypass mode" when the i2c master interface is disabled. The pins will float high due to the internal pull-up if not enabled and the i2c master interface is disabled
        FSYNC_INT_MODE_EN          = 0b00000100, // If 1, enables the FSYNC pin to be used as an interrupt. If 0, the pin is disabled from causing an interrupt.
        ACTL_FSYNC                 = 0b00001000, // If 1, the logic level for the FSUNC pin as an interrupt is active low. If 1, it'll be active high.
        INT_ANYRD_2CLEAR           = 0b00010000, // If 1, then interrupt status is cleared if any read operation is performed. If 0, then interrupt status is cleared only by readin the INT_STATUS register
        LATCH_INT_EN               = 0b00100000, // If 1, then INT pin level is held until interrupt status is cleared. If 0, INT pin indicates interrupt pulse's width is 50us
        OPEN                       = 0b01000000, // If 1, the INT pin is configured as open drain. If 0, then as push-pull
        ACTL                       = 0b10000000  // The logic level for INT pin is active low (if this bit is 0 then active high)

    };


    I2cMpu6050();
    ~I2cMpu6050();

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

    void set_measurement_acceleration_x( int x );
    int  get_measurement_acceleration_x() const;
    
    void set_measurement_acceleration_y( int y );
    int  get_measurement_acceleration_y() const;

    void set_measurement_acceleration_z( int z );
    int  get_measurement_acceleration_z() const;

    void  set_temperature_celsius( float celsius );
    float get_temperature_celsius() const;

    void  set_temperature_kelvin( float kelvin );
    float get_temperature_kelvin() const;

    void  set_temperature_fahrenheit( float fahrenheit );
    float get_temperature_fahrenheit() const;



    // Device handling.
    
    void _reset_device();

    void _self_test();

    void _self_test_gyro();
    void _self_test_accelerator();

    void _read_sensor_data();
};




}



#endif 