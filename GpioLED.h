#ifndef GPIO_LED_H_INCLUDED 
#define GPIO_LED_H_INCLUDED

#include "GpioRawDevice.h"

namespace godot {


class GpioLED : public GpioRawDevice {
    GDCLASS(GpioLED, GpioRawDevice)

private:
    bool _is_LED_on;
    
protected: 

    static void _bind_methods();

public: 
    GpioLED();
    ~GpioLED();

    // Getters and setters.
    bool get_is_LED_on() const;
    void set_is_LED_on( const bool is_on );

    // Device handling.
    virtual void _initialize_device() override;


};

}

#endif