#ifndef SINGLEBOARDCOMPUTER_H_INCLUDED
#define SINGLEBOARDCOMPUTER_H_INCLUDED 

#include "GpioPin.h"
#include "I2CBus.h"
#include "SPIBus.h"

#include <godot_cpp/classes/node.hpp>

namespace godot {

class SingleBoardComputer : public Node {
    GDCLASS(SingleBoardComputer, Node)

private:
    int _board_id;

    // General setup information
    // These don't actually contain anything
    // about the pins or buses, they are configs
    // that are used to check which device-nodes
    // will be functional as childs.
    // These will also validate inputs given
    // to the device-nodes.
    int _num_gpio_pins; // Number of GPIO pins on the sbc - Now assumed 40 in Raspberry PI layout
    int _num_pwm_pins;  // Number of PWM pins on the sbc
    int _num_i2c_buses; // Number of I2C buses on the sbc
    int _num_spi_buses; // Number of SPI buses on the sbc

    // The configs for the gpio, pwm, ic2, etc...
    GpioPin* _gpio_pins; // An array of gpio pins. 
    I2CBus* _i2c_buses;  // Array of i2c buses.
    SPIBus* _spi_buses;  // Array of spi buses.

    // The device file descriptors.
    int* _opened_gpio_device_files;
    int* _opened_gpio_device_file_gpio_numbers;
    int  _num_opened_gpio_device_files; 

    int* _opened_i2c_bus_device_files;
    int* _opened_i2c_bus_device_file_i2c_bus_numbers ;
    int  _num_opened_i2c_bus_device_files;

    void _setup_board();
    void _initialize_child_devices();
protected:
    static void _bind_methods();

public:
    SingleBoardComputer();
    ~SingleBoardComputer();

    // Getters and setters for attributes.
    void set_board_id( int input );
    int get_board_id() const;

    // Gpio related handers.
    int get_num_gpio_pins() const;
    int get_gpio_pin_offset(int pin_index) const;

    // I2C related handlers.
    int get_num_i2c_buses() const;
    I2CBus* get_i2c_bus( int bus_index ) const;

    // File handle management.
    int request_i2c_device_file( int bus_index );
    int request_gpio_device_file( int pin_index );

    // Godot virtuals.
    void _ready();
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);


    //PackedStringArray get_configuration_warnings() const override;
};

}


#endif 