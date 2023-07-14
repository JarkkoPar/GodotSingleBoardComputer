#ifndef ADC_TEMPERATURE_SENSOR_H_INCLUDED
#define ADC_TEMPERATURE_SENSOR_H_INCLUDED

#include "AdcDevice.h"

namespace godot {

// This is a "raw" Analog-Digital-Converter (ADC) device node, which allows a more low-level
// interface to SBC pins that allow analog input.
class AdcTemperatureSensor : public AdcDevice {
    GDCLASS(AdcTemperatureSensor, AdcDevice)

private:
    
protected: 
    float _min_temperature_celsius;
    float _max_temperature_celsius;
    float _internal_resistance; // The internal resistance of the thermistor sensor (default 10 kOhms)
    float _A, _B, _C; // Coefficients for the thermistor calculations
    bool  _is_averaged;

    float _temperature_kelvin;
    float _temperature_celsius;
    float _temperature_fahrenheit;

    
    static void _bind_methods();

public:

    AdcTemperatureSensor();
    ~AdcTemperatureSensor();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.

    void set_internal_resistance( int ohms );
    int get_internal_resistance() const;

    void set_a( float a );
    float get_a() const;

    void set_b( float b );
    float get_b() const;

    void set_c( float c );
    float get_c() const;

    void set_is_averaged( bool is_averaged );
    bool get_is_averaged() const;


    void set_min_temperature_celsius( float celsius );
    float get_min_temperature_celsius() const;

    void set_max_temperature_celsius( float celsius );
    float get_max_temperature_celsius() const;

    void set_temperature_kelvin( float kelvin );
    float get_temperature_kelvin() const;

    void set_temperature_celsius( float celsius );
    float get_temperature_celsius() const;

    void set_temperature_fahrenheit( float fahrenheit );
    float get_temperature_fahrenheit() const;

    
};



}



#endif 