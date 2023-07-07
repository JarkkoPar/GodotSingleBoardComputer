#ifndef GPIOPIN_H_INCLUDED
#define GPIOPIN_H_INCLUDED 


namespace godot {



class GpioPin{
    
protected:

public:

    enum GpioPinFunction {
        GPF_NONE,
        GPF_GND,
        GPF_3_3V,
        GPF_5_0V,
        GPF_PIN,
        GPF_I2C_SDA,
        GPF_I2C_SCL,
        GPF_I2C_CLK,
        GPF_SPI_CLK,
        GPF_SPI_CSn,
        GPF_SPI_TXD,
        GPF_SPI_RXD,
        GPF_PWM,
        GPF_UART_TXD,
        GPF_UART_RXD,
        GPF_I2S_SCLK,
        GPF_I2S_LRCK_TX,
        GPF_I2S_LRCK_RX,
        GPF_I2S_SDI,
        GPF_I2S_SDO,
        GPF_SPDIF_TX ,
        GPF_ADC_IN0
    };

    GpioPin();
    ~GpioPin();

    // Getters and setters for attributes.

    void set_gpio_device_file_index( int new_index );
    int get_gpio_device_file_index() const;

    void set_pin_offset( int new_offset );
    int  get_pin_offset() const;

    void set_primary_function( GpioPinFunction new_function );
    GpioPinFunction get_primary_function() const;

    void set_secondary_function( GpioPinFunction new_function );
    GpioPinFunction get_secondary_function() const;

private:
    int             _gpio_device_file_index;
    GpioPinFunction _primary_function;
    GpioPinFunction _secondary_function; 

    int             _pin_offset;
};

}


#endif 