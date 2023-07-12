#ifndef ADC_DEVICE_H_INCLUDED
#define ADC_DEVICE_H_INCLUDED

#include "SBCDevice.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {

// This is a "raw" Analog-Digital-Converter (ADC) device node, which allows a more low-level
// interface to SBC pins that allow analog input.
class AdcDevice : public SBCDevice {
    GDCLASS(AdcDevice, SBCDevice)

private:
    
protected: 
    
    int _adc_device_fd;             // The file for the ADC device, only used when running the app, not in editor
    int _adc_gpio_pin_index;        // Index in the internal array of the SingleBoardComputer class for the pin used for ADC
    int _adc_device_raw_file_index; // Index of the deviceX-folder for the voltageX_raw file to read for the input
    int _adc_voltage_raw_file_index; // Index of the voltageX_raw file to read for the input

    float _adc_pin_max_voltage;     // The max voltage read by the pin (based on specsheet/documentation)
    float _adc_pin_voltage;         // The voltage read in the pin
    int _adc_pin_value;             // The pin value as an integer (0..1023)

    static void _bind_methods();

public:

    AdcDevice();
    ~AdcDevice();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.

    void set_adc_gpio_pin_index( int index );
    int  get_adc_gpio_pin_index() const;

    void set_adc_pin_voltage( float voltage );
    float get_adc_pin_voltage() const;

    void set_adc_pin_value( int value );
    int get_adc_pin_value() const;


    // Device handling.
    virtual void _initialize_device() override;

    void open_device();
    void close_device();
    
    int read_value_from_pin();
    
};



}



#endif 