#ifndef I2C_SDA1115_H_INCLUDED
#define I2C_SDA1115_H_INCLUDED

#include "I2cDevice.h"
#include <thread>
#include <mutex>

namespace godot {

//                                          |          
//                                          FEDCBA9876543210
#define ADS1115_START_CONVERSION          0b1000000000000000
#define ADS1115_PERFORMING_CONVERSION     0b0000000000000000
#define ADS1115_NOT_PERFORMING_CONVERSION 0b1000000000000000



// This is a control node for the ADS1115 A/D Converter.
class I2cAds1115 : public I2cDevice {
    GDCLASS(I2cAds1115, I2cDevice)

private:
    double _ads1115_update_frame_delay;
    double _ads1115_update_wait_time;

    int _ain0_value;
    int _ain1_value;
    int _ain2_value;
    int _ain3_value;

    uint16_t _config;
    
protected: 
    
    static void _bind_methods();
    virtual bool _configure_i2c_device() override;

public:



    enum ADS1115Registers {
        CONVERSION    = 0x00,
        CONFIGURATION = 0x01,
        TRESHOLD_LOW  = 0x02,
        TRESHOLD_HIGH = 0x03
    };

    
    enum ADS1115InputMultiplexerConfiguration {
        //                |||          
        //               FEDCBA9876543210
        P_AIN0_N_AIN1 = 0b0000000000000000, // DEFAULT
        P_AIN0_N_AIN3 = 0b0010000000000000,
        P_AIN1_N_AIN3 = 0b0100000000000000,
        P_AIN2_N_AIN3 = 0b0110000000000000,
        P_AIN0_N_GND  = 0b1000000000000000,
        P_AIN1_N_GND  = 0b1010000000000000,
        P_AIN2_N_GND  = 0b1100000000000000,
        P_AIN3_N_GND  = 0b1110000000000000
    };

    enum ADS1115ProgrammableGainAmplifierConfiguration {
        //                   |||          
        //               FEDCBA9876543210
        GAIN_2_DIV_3 = 0b0000000000000000,
        GAIN_1       = 0b0000001000000000,
        GAIN_2       = 0b0000010000000000, // DEFAULT
        GAIN_4       = 0b0000011000000000,
        GAIN_8       = 0b0000100000000000,
        GAIN_16      = 0b0000101000000000
    };

    enum ADS1115DeviceOperatingMode {
        //                     |
        //              FEDCBA9876543210
        CONTINUOUS  = 0b0000000000000000,
        SINGLE_SHOT = 0b0000000100000000 // DEFAULT
    };

    enum ADS1115DataRate {
        //                  |||
        //          FEDCBA9876543210
        SPS_8   = 0b0000000000000000,
        SPS_16  = 0b0000000000100000,
        SPS_32  = 0b0000000001000000,
        SPS_64  = 0b0000000001100000,
        SPS_128 = 0b0000000010000000, // DEFAULT
        SPS_250 = 0b0000000010100000,
        SPS_475 = 0b0000000011000000,
        SPS_860 = 0b0000000011100000
    };

    enum ADS1115ComparatorMode {
        //                         |          
        //              FEDCBA9876543210
        TRADITIONAL = 0b0000000000000000,
        WINDOW      = 0b0000000000010000
    };

    enum ADS1115ComparatorPolarity {
        //                          |          
        //              FEDCBA9876543210
        ACTIVE_LOW  = 0b0000000000000000, // DEFAULT
        ACTIVE_HIGH = 0b0000000000001000 
    };

    enum ADS1115LatchingComparator {
        //                                |          
        //                   FEDCBA9876543210
        NON_LATCHING     = 0b0000000000000000, // DEFAULT
        LATCHING         = 0b0000000000000100
    };

    enum ADS1115ComparatorQueueAndDisable {
        //                                           ||          
        //                             FEDCBA9876543210
        ASSERT_AFTER_1_CONVERSIONS = 0b0000000000000000,
        ASSERT_AFTER_2_CONVERSIONS = 0b0000000000000001,
        ASSERT_AFTER_3_CONVERSIONS = 0b0000000000000010,
        DISABLE                    = 0b0000000000000011 // DEFAULT
    };




    I2cAds1115();
    ~I2cAds1115();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    
    // Getters and setters.

    
    
    // Device handling.
    

};




}



#endif 