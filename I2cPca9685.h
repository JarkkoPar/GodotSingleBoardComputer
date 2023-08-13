#ifndef I2C_PCA9685_H_INCLUDED
#define I2C_PCA9685_H_INCLUDED

//#include <core/templates/vset.h>
#include "I2cDevice.h"

namespace godot {

// This is a control node for the PCA9685 servo driver.
class I2cPca9685 : public I2cDevice {
    GDCLASS(I2cPca9685, I2cDevice)

private:
    //bool _is_active;

    int _pwm_frequency_hz; 
    int _pwm_prescale_value;
    double _pwm_oscillator_frequency;
    bool _is_pca9685_initialized;
    double _pca9685_update_frame_delay;
    double _pca9685_update_wait_time;

    uint8_t led_values[16];
    float servo_angles[16];
    double servo_min_angle_ms[16];
    double servo_max_angle_ms[16];

    int _servo_min_angle_pulses[16];
    int _servo_max_angle_pulses[16];
    int _servo_pulses_between_min_max_angles[16];
    
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

    I2cPca9685();
    ~I2cPca9685();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    void update_servo_min_max_angle_pulse_counts();

    // Getters and setters.

    //void set_is_active( bool is_active );
    //bool get_is_active() const;

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

    void set_led_pulse_range( int led_index, int on_index, int off_index );

    // Getters and setters for servo values.

    void set_servo0_min_angle_ms( const double new_ms );
    void set_servo1_min_angle_ms( const double new_ms );
    void set_servo2_min_angle_ms( const double new_ms );
    void set_servo3_min_angle_ms( const double new_ms );
    void set_servo4_min_angle_ms( const double new_ms );
    void set_servo5_min_angle_ms( const double new_ms );
    void set_servo6_min_angle_ms( const double new_ms );
    void set_servo7_min_angle_ms( const double new_ms );
    void set_servo8_min_angle_ms( const double new_ms );
    void set_servo9_min_angle_ms( const double new_ms );
    void set_servo10_min_angle_ms( const double new_ms );
    void set_servo11_min_angle_ms( const double new_ms );
    void set_servo12_min_angle_ms( const double new_ms );
    void set_servo13_min_angle_ms( const double new_ms );
    void set_servo14_min_angle_ms( const double new_ms );
    void set_servo15_min_angle_ms( const double new_ms );

    double get_servo0_min_angle_ms() const;
    double get_servo1_min_angle_ms() const;
    double get_servo2_min_angle_ms() const;
    double get_servo3_min_angle_ms() const;
    double get_servo4_min_angle_ms() const;
    double get_servo5_min_angle_ms() const;
    double get_servo6_min_angle_ms() const;
    double get_servo7_min_angle_ms() const;
    double get_servo8_min_angle_ms() const;
    double get_servo9_min_angle_ms() const;
    double get_servo10_min_angle_ms() const;
    double get_servo11_min_angle_ms() const;
    double get_servo12_min_angle_ms() const;
    double get_servo13_min_angle_ms() const;
    double get_servo14_min_angle_ms() const;
    double get_servo15_min_angle_ms() const;

    void set_servo0_max_angle_ms( const double new_ms );
    void set_servo1_max_angle_ms( const double new_ms );
    void set_servo2_max_angle_ms( const double new_ms );
    void set_servo3_max_angle_ms( const double new_ms );
    void set_servo4_max_angle_ms( const double new_ms );
    void set_servo5_max_angle_ms( const double new_ms );
    void set_servo6_max_angle_ms( const double new_ms );
    void set_servo7_max_angle_ms( const double new_ms );
    void set_servo8_max_angle_ms( const double new_ms );
    void set_servo9_max_angle_ms( const double new_ms );
    void set_servo10_max_angle_ms( const double new_ms );
    void set_servo11_max_angle_ms( const double new_ms );
    void set_servo12_max_angle_ms( const double new_ms );
    void set_servo13_max_angle_ms( const double new_ms );
    void set_servo14_max_angle_ms( const double new_ms );
    void set_servo15_max_angle_ms( const double new_ms );

    double get_servo0_max_angle_ms() const;
    double get_servo1_max_angle_ms() const;
    double get_servo2_max_angle_ms() const;
    double get_servo3_max_angle_ms() const;
    double get_servo4_max_angle_ms() const;
    double get_servo5_max_angle_ms() const;
    double get_servo6_max_angle_ms() const;
    double get_servo7_max_angle_ms() const;
    double get_servo8_max_angle_ms() const;
    double get_servo9_max_angle_ms() const;
    double get_servo10_max_angle_ms() const;
    double get_servo11_max_angle_ms() const;
    double get_servo12_max_angle_ms() const;
    double get_servo13_max_angle_ms() const;
    double get_servo14_max_angle_ms() const;
    double get_servo15_max_angle_ms() const;


    void set_servo_euler_angle( const int servo_index, const float new_euler_angle );
    void set_servo0_euler_angle( const float new_euler_angle );
    void set_servo1_euler_angle( const float new_euler_angle );
    void set_servo2_euler_angle( const float new_euler_angle );
    void set_servo3_euler_angle( const float new_euler_angle );
    void set_servo4_euler_angle( const float new_euler_angle );
    void set_servo5_euler_angle( const float new_euler_angle );
    void set_servo6_euler_angle( const float new_euler_angle );
    void set_servo7_euler_angle( const float new_euler_angle );
    void set_servo8_euler_angle( const float new_euler_angle );
    void set_servo9_euler_angle( const float new_euler_angle );
    void set_servo10_euler_angle( const float new_euler_angle );
    void set_servo11_euler_angle( const float new_euler_angle );
    void set_servo12_euler_angle( const float new_euler_angle );
    void set_servo13_euler_angle( const float new_euler_angle );
    void set_servo14_euler_angle( const float new_euler_angle );
    void set_servo15_euler_angle( const float new_euler_angle );

    float get_servo_euler_angle( const int servo_index ) const;
    float get_servo0_euler_angle() const;
    float get_servo1_euler_angle() const;
    float get_servo2_euler_angle() const;
    float get_servo3_euler_angle() const;
    float get_servo4_euler_angle() const;
    float get_servo5_euler_angle() const;
    float get_servo6_euler_angle() const;
    float get_servo7_euler_angle() const;
    float get_servo8_euler_angle() const;
    float get_servo9_euler_angle() const;
    float get_servo10_euler_angle() const;
    float get_servo11_euler_angle() const;
    float get_servo12_euler_angle() const;
    float get_servo13_euler_angle() const;
    float get_servo14_euler_angle() const;
    float get_servo15_euler_angle() const;

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

    void set_led_value( int led_index, int new_led_value );
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


//VARIANT_ENUM_CAST(I2cPca9685::PCA9685Registers);
//VARIANT_ENUM_CAST(I2cPca9685::PCA9685Mode);


}



#endif 