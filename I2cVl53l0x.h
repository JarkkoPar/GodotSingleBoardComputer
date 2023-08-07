#ifndef I2C_VL53L0X_H_INCLUDED
#define I2C_VL53L0X_H_INCLUDED

#include "I2cDevice.h"

namespace godot {

// This is a control node for the Vl53l0x servo driver.
class I2cVl53l0x : public I2cDevice {
    GDCLASS(I2cVl53l0x, I2cDevice)

private:
    bool _is_vl53l0x_initialized;
    double _vl53l0x_update_frame_delay;
    double _vl53l0x_update_wait_time;

    //uint8_t _custom_vl53l0x_i2c_device_address;
    bool _is_active;
    int _reading_mode; 
    float _distance_mm, _distance_inch;
    
protected: 
    static void _bind_methods();

public:
//https://github.com/GrimbiXcode/VL53L0X-Register-Map

    enum Vl53l0xRegisters {
        SYSRANGE_START = 0x00,
        SYSTEM_SEQUENCE_CONFIG = 0x01,
        SYSTEM_INTERMEASUREMENT_PERIOD = 0x04,
        SYSTEM_RANGE_CONFIG = 0x09,
        SYSTEM_INTERRUPT_GPIO_CONFIG = 0x0A,
        SYSTEM_INTERRUPT_CLEAR = 0x0B,
        SYSTEM_TRESH_HIGH = 0x0C,
        SYSTEM_TRESH_LOW = 0x0E,
        I2C_MODE = 0x88,
        I2C_SLAVE_DEVICE_ADDRESS = 0x8a,
        RESULT_INTERRUPT_STATUS = 0x13,
        RESULT_RANGE_STATUS = 0x14,
        RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN = 0xBC,
        RESULT_CORE_RANGING_TOTAL_EVENTS_RTN = 0xC0,
        RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF = 0xD0,
        RESULT_CORE_RANGING_TOTAL_EVENTS_REF = 0xD4,
        RESULT_PEAK_SIGNAL_RATE_REF = 0xB6,
        MEASUREMENT_MOST_SIGNIFICANT_BIT = 0x1e,
        MEASUREMENT_LEAST_SIGNIFICANT_BIT = 0x1f
    }; 

    enum Vl53l0xReadingMode {
        SINGLE_SHOT = 0x00,
        BACK_TO_BACK = 0x02,
        TIMED = 0x04,
        HISTOGRAM = 0x08
    };

    enum Vl53l0xState {
        POWERDOWN = 0x00,
        WAIT_STATICINT = 0x01,
        STANDBY = 0x02,
        IDLE = 0x03,
        RUNNING = 0x04,
        UNKNOWN = 0x62,
        ERROR = 0x63
    };

    /**
    enum Vl53l0xPowerMode {
        STANDBY_LEVEL1 = 0x0,
        STANDBY_LEVEL2 = 0x1,
        IDLE_LEVEL1 = 0x2,
        IDLE_LEVEL2 = 0x3
    };
    /**/

    I2cVl53l0x();
    ~I2cVl53l0x();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);


    // Getters and setters.

    void set_is_active( bool is_active );
    bool get_is_active() const;

    void set_reading_mode( int mode );
    int  get_reading_mode() const;
    
    void  set_distance_mm( float distance_mm );
    float get_distance_mm();

    void  set_distance_inch( float distance_inch );
    float get_distance_inch();

    void  set_custom_vl53l0x_i2c_device_address( int new_i2c_device_address );
    //int   get_custom_vl53l0x_i2c_device_address();

    // Device handling.
    virtual void _initialize_device() override;
    
    //void wake_Vl53l0x();
    //void sleep_Vl53l0x();
    //void restart_Vl53l0x();


};


}



#endif 