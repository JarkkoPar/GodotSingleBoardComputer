#ifndef GPIO_HC_SR04_H_INCLUDED
#define GPIO_HC_SR04_H_INCLUDED 

#include "GpioDevice.h"


namespace godot {


// This is a the HC-SR04 ultrasound distance sensor device.
class GpioHcSr04 : public GpioDevice {
    GDCLASS(GpioHcSr04, GpioDevice)

private:
    
protected: 

    bool _is_hcsr04_initialized;
    bool _is_active;
    
    int _gpio_trig_pin_index;     // The triggering pin index
    int _gpio_trig_pin_type;      // The triggering pin type
    int _gpio_trig_pin_device_fd; 
    int _gpio_trig_pin_fd;

    int _gpio_echo_pin_index;     // The echoing pin index
    int _gpio_echo_pin_type;      // The echoing pin type
    int _gpio_echo_pin_device_fd; 
    int _gpio_echo_pin_fd;

    bool _run_trigger_echo_loop;

    float _distance_mm;
    float _distance_inch;

    //int _gpio_device_fd;          // The file descriptor for the gpio device, only used when running the app, not in the editor
    //int _gpio_pin_index;          // Index in the internal array of the SingleBoardComputer class
    //int _gpio_pin_fd;             // The file descriptor for the pin/pins, only used when running the app, not in the editor
    //int _gpio_pin_type;           // The type of this pin (INPUT_L, INPUT_H, OUTPUT_L, OUTPUT_H)
    //int _gpio_pin_offset;         // The offset for the pin

    static void _bind_methods();

public:

    enum {
        INPUT = 0,
        OUTPUT
    } GpioPinType;

    GpioHcSr04();
    ~GpioHcSr04();

    // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.

    void set_is_active( bool is_active );
    bool get_is_active() const;

    void set_gpio_trig_pin_index( int trig_pin_number );
    int  get_gpio_trig_pin_index() const;
    
    void set_gpio_echo_pin_index( int echo_pin_number );
    int  get_gpio_echo_pin_index() const;
    
    void  set_distance_mm( float distance_mm );
    float get_distance_mm();

    void  set_distance_inch( float distance_inch );
    float get_distance_inch();


    // Device handling.

    virtual void _initialize_device() override;

    void open_device();
    void close_device();

    void trigger_echo_loop();

    //void write_byte_to_device( uint8_t data );
    //int read_byte_from_device( uint8_t* result );
};

}

#endif 