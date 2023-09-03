#ifndef I2C_MULTITAP_PULLPIN_H_INCLUDED
#define I2C_MULTITAP_PULLPIN_H_INCLUDED

#include "I2cDevice.h"

#include "SingleBoardComputer.h"
#include "GpioPin.h"
#include "I2cBus.h"

namespace godot {


// This is a node that allows multiple I2C devices or sensors
// with the same address to be connected to the i2c bus. The
// logic to select the active device must be that some pin is
// pulled either up or down on the i2c device.
class I2cMultitapPullPin : public I2cDevice {
    GDCLASS(I2cMultitapPullPin, I2cDevice)

private:
    SingleBoardComputer* _singleboardcomputer;

protected: 
    static void _bind_methods();

    virtual bool _configure_i2c_device() override;
public:

    I2cMultitapPullPin();
    ~I2cMultitapPullPin();

    // Gpio related handers.
    int get_num_gpio_pins() const;
    GpioPin* get_gpio_pins() const;
    int get_gpio_pin_offset(int pin_index) const;

    // I2C related handlers.
    int get_num_i2c_buses() const;
    I2cBus* get_i2c_bus( int bus_index ) const;

    // File handle management.
    int request_i2c_device_file( int bus_index );
    int request_gpio_device_file( int pin_index );
    //int request_adc_device_file( int pin_index );


    // Godot virtuals.
    
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    
    // Getters and setters.



    // Device handling.
    
   
};




}



#endif 