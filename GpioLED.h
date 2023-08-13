#ifndef GPIO_LED_H_INCLUDED 
#define GPIO_LED_H_INCLUDED

#include "GpioDevice.h"

namespace godot {


class GpioLED : public GpioDevice {
    GDCLASS(GpioLED, GpioDevice)

private:
    int  _led_pin_index;
    bool _is_LED_on;
    int  _led_fd;
protected: 

    static void _bind_methods();

public: 
    GpioLED();
    ~GpioLED();

    // Getters and setters.

    bool get_is_LED_on() const;
    void set_is_LED_on( const bool is_on );

    int  get_LED_pin_index() const;
    void set_LED_pin_index( const int pin_number );


    // Device handling.
    virtual void _initialize_device() override;
    void open_device();

};

}

#endif