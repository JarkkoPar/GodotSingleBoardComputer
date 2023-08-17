#ifndef GPIO_DEVICE_H_INCLUDED
#define GPIO_DEVICE_H_INCLUDED 

#include "SBCDevice.h"


namespace godot {

#define GPIO_TYPE_INPUT 0
#define GPIO_TYPE_OUTPUT 1

// This is a "raw" gpio device node, which allows a more low-level
// communication interface with any gpio pin connected device.
class GpioDevice : public SBCDevice {
    GDCLASS(GpioDevice, SBCDevice)

private:
    
protected: 
    
    static void _bind_methods();

    virtual void _initialize_device() override;
    virtual void _deinitialize_device() override;

    virtual void _open_gpio_device();
    virtual void _configure_gpio_device();

public:

    enum {
        INPUT = 0,
        OUTPUT
    } GpioPinType;

    GpioDevice();
    ~GpioDevice();

     // Godot virtuals.
     
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.

    
    // Device handling.

    int request_pin_number( int pin_number, int pin_type, const char* consumer_name );

    void write_byte_to_device( int gpio_pin_fd, uint8_t data );
    int read_byte_from_device( int gpio_pin_fd, uint8_t* result );
};

}

#endif 