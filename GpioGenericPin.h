#ifndef GPIO_GENERICPIN_H_INCLUDED
#define GPIO_GENERICPIN_H_INCLUDED 

#include "GpioRawDevice.h"

namespace godot {

/**
class GpioGenericPin : public GpioRawDevice {
    GDCLASS(GpioGenericPin, GpioRawDevice)

private:
    
protected: 
    
    int _gpio_device_fd;          // The file descriptor for the gpio device, only used when running the app, not in the editor
    int _gpio_pin_index;          // Index in the internal array of the SingleBoardComputer class
    int _gpio_pin_fd;             // The file descriptor for the pin/pins, only used when running the app, not in the editor
    //int _gpio_pin_type;           // The type of this pin (INPUT_L, INPUT_H, OUTPUT_L, OUTPUT_H)
    static void _bind_methods();

public:

    enum {
        INPUT_LOW = 0,
        INPUT_HIGH,
        OUTPUT_LOW,
        OUTPUT_HIGH
    } GpioPinType;

    GpioGenericPin();
    ~GpioGenericPin();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.

    void set_gpio_pin_index( int pin_index );
    int  get_gpio_pin_index() const;
    
    //void set_gpio_pin_type( int pin_type );
    //int  get_gpio_pin_type() const;

    // Device handling.
    virtual void _initialize_device() override;

    void open_device();
    void close_device();
};

/**/

}


#endif