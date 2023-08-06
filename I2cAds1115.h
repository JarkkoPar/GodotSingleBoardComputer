#ifndef I2C_SDA1115_H_INCLUDED
#define I2C_SDA1115_H_INCLUDED

//#include <core/templates/vset.h>
#include "I2cDevice.h"

namespace godot {

// This is a control node for the PCA9685 servo driver.
class I2cAds1115 : public I2cDevice {
    GDCLASS(I2cAds1115, I2cDevice)

private:
    int _pwm_frequency_hz; 
    int _pwm_prescale_value;
    double _pwm_oscillator_frequency;
    bool _is_ads1115_initialized;
    double _ads1115_update_frame_delay;
    double _ads1115_update_wait_time;

    
protected: 
    static void _bind_methods();

public:

    enum ADS1115Registers {
        CONVERSION = 0x00,
        CONFIGURATION = 0x01,
        TRESHOLD_LOW = 0x02,
        TRESHOLD_HIGH = 0x03
    };

    enum ADS1115Mode {
        CONTINUOUS = 0x00,
        SINGLE_SHOT = 0x80
    };

    enum ADS1115SampleRate {
        SPS_8 = 0x00,
        SPS_16 = 0x80,
        SPS_32 = 0x81,
        SPS_64 = 0x82,
        SPS_128 = 0x83,
        SPS_250 = 0x84,
        SPS_475 = 0x85,
        SPS_860 = 0x86
    };

    enum ADS1115AlertMode {
        SET_ALERT_AFTER_1_CONVERSION = 0x0,
        SET_ALERT_AFTER_2_CONVERSIONS = 0x1,
        SET_ALERT_AFTER_4_CONVERSIONS = 0x2,
        SET_ALERT_DISABLED = 0x3
    };

    enum ADS1115AlertLatchMode {
        DO_NOT_LATCH = 0,
        LATCH_UNTIL_READ = 0x1
    };

    enum ADS1115AlertPolarity {
        ACTIVE_LOW = 0x0,
        ACTIVE_HIGH = 0x1
    };

    enum ADS1115ComparatorMode {
        TRADITIONAL = 0x0,
        WINDOW = 0x1
    };



    I2cAds1115();
    ~I2cAds1115();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    
    // Getters and setters.
    //void set_pwm_frequency_hz( int new_frequency_hz );
    //int  get_pwm_frequency_hz() const;    

    
    // Device handling.
    virtual void _initialize_device() override;
    
    //void wake_ads1115();
    //void sleep_ads1115();
    //void restart_ads1115();


};




}



#endif 