#ifndef GPIO_RAWDEVICE_H_INCLUDED
#define GPIO_RAWDEVICE_H_INCLUDED 

#include "SBCDevice.h"


namespace godot {

#define GPIO_TYPE_INPUT 0
#define GPIO_TYPE_OUTPUT 1

// This is a "raw" gpio device node, which allows a more low-level
// communication interface with any gpio pin connected device.
class GpioRawDevice : public SBCDevice {
    GDCLASS(GpioRawDevice, SBCDevice)

private:
    
protected: 
    
    int _gpio_device_fd;          // The file descriptor for the gpio device, only used when running the app, not in the editor
    int _gpio_pin_index;          // Index in the internal array of the SingleBoardComputer class
    int _gpio_pin_fd;             // The file descriptor for the pin/pins, only used when running the app, not in the editor
    int _gpio_pin_type;           // The type of this pin (INPUT_L, INPUT_H, OUTPUT_L, OUTPUT_H)
    //int _gpio_pin_offset;         // The offset for the pin

    static void _bind_methods();

public:

    enum {
        INPUT = 0,
        OUTPUT
    } GpioPinType;

    GpioRawDevice();
    ~GpioRawDevice();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.

    void set_gpio_pin_index( int pin_index );
    int  get_gpio_pin_index() const;
    
    void set_gpio_pin_type( int pin_type );
    int  get_gpio_pin_type() const;

    // Device handling.
    virtual void _initialize_device() override;

    void open_device();
    void close_device();

    void write_byte_to_device( uint8_t data );
    int read_byte_from_device( uint8_t* result );
};

}

#endif 