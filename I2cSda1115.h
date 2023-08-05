#ifndef I2C_SDA1115_H_INCLUDED
#define I2C_SDA1115_H_INCLUDED

//#include <core/templates/vset.h>
#include "I2cDevice.h"

namespace godot {

// This is a control node for the PCA9685 servo driver.
class I2cSda1115 : public I2cDevice {
    GDCLASS(I2cSda1115, I2cDevice)

private:
    int _pwm_frequency_hz; 
    int _pwm_prescale_value;
    double _pwm_oscillator_frequency;
    bool _is_sda1115_initialized;
    double _sda1115_update_frame_delay;
    double _sda1115_update_wait_time;

    
protected: 
    static void _bind_methods();

public:

    enum SDA1115Registers {
        MODE1 = 0x00,
        MODE2 = 0x01,
        SUBADDR1 = 0x02,
        SUBADDR2 = 0x03,
        SUBADDR3 = 0X04,
        BROADCAST = 0x05,
        LED0 = 0x6,
        LED0_ON_L = 0x6,
        LED0_ON_H = 0x7,
        LED0_OFF_L = 0x8,
        LED0_OFF_H = 0x9,
        PRESCALE = 0xFE
    }; // LED_MULTIPLIER 4

    enum SDA1115Mode {
        NORMAL = 0x00,
        TOTEMPOLE = 0x04,
        SLEEP = 0x10,
        RESTART = 0x80
    };

    I2cSda1115();
    ~I2cSda1115();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    
    // Getters and setters.
    //void set_pwm_frequency_hz( int new_frequency_hz );
    //int  get_pwm_frequency_hz() const;    

    
    // Device handling.
    virtual void _initialize_device() override;
    
    //void wake_sda1115();
    //void sleep_sda1115();
    //void restart_sda1115();


};




}



#endif 