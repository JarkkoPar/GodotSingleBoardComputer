#ifndef I2C_PCA9685_H_INCLUDED
#define I2C_PCA9685_H_INCLUDED

//#include <core/templates/vset.h>
#include "I2CRawDevice.h"

namespace godot {

// This is a "raw" i2c device node, which allows a more low-level
// communication interface with any i2c device.
class I2CPCA9685 : public I2CRawDevice {
    GDCLASS(I2CPCA9685, I2CRawDevice)

private:
    int _pwm_frequency_hz; 
    bool _is_pca9685_initialized;

    uint8_t led_values[16];
    
protected: 
    static void _bind_methods();

public:

    enum PCA9685Registers {
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

    enum PCA9685Mode {
        NORMAL = 0x00,
        TOTEMPOLE = 0x04,
        SLEEP = 0x10,
        RESTART = 0x80
    };

    I2CPCA9685();
    ~I2CPCA9685();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.
    void set_pwm_frequency_hz( int new_frequency_hz );
    int  get_pwm_frequency_hz() const;    

    
    // Device handling.
    //uint8_t get_register_value( uint8_t pca9685_register );
    //void set_register_value( uint8_t pca9685_register, uint8_t mode );
    virtual void _initialize_device() override;
    void on_timer_finished_finalize_initialize();
    //void set_slot_value( uint8_t slot_index, uint8_t value );

    void wake_pca9685();
    void sleep_pca9685();
    void restart_pca9685();

    // Getters and setters for led values.
    int get_led0_value();
    int get_led1_value();
    int get_led2_value();
    int get_led3_value();
    int get_led4_value();
    int get_led5_value();
    int get_led6_value();
    int get_led7_value();
    int get_led8_value();
    int get_led9_value();
    int get_led10_value();
    int get_led11_value();
    int get_led12_value();
    int get_led13_value();
    int get_led14_value();
    int get_led15_value();

    void set_led0_value(int new_led_value);
    void set_led1_value(int new_led_value);
    void set_led2_value(int new_led_value);
    void set_led3_value(int new_led_value);
    void set_led4_value(int new_led_value);
    void set_led5_value(int new_led_value);
    void set_led6_value(int new_led_value);
    void set_led7_value(int new_led_value);
    void set_led8_value(int new_led_value);
    void set_led9_value(int new_led_value);
    void set_led10_value(int new_led_value);
    void set_led11_value(int new_led_value);
    void set_led12_value(int new_led_value);
    void set_led13_value(int new_led_value);
    void set_led14_value(int new_led_value);
    void set_led15_value(int new_led_value);
};


//VARIANT_ENUM_CAST(I2CPCA9685::PCA9685Registers);
//VARIANT_ENUM_CAST(I2CPCA9685::PCA9685Mode);


}



#endif 