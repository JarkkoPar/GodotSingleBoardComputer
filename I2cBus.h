#ifndef I2C_BUS_H_INCLUDED
#define I2C_BUS_H_INCLUDED 


class I2cBus {
public:

    I2cBus();
    ~I2cBus();

    void set_i2c_device_file_index( int new_index );
    int  get_i2c_device_file_index() const;

    void add_gpio_reserved_pin( int new_pin_index );
    int* get_gpio_reserved_pins() const;
private:
    int _i2c_device_file_index;     // Which I2C-device number this bus uses
    int* _gpio_reserved_pins;       // Not yet used, but planned to show warning in Godot UI if same pin is used in many places
    int  _num_gpio_reserved_pins;   
};


#endif 