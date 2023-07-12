#ifndef ADC_NTCMF52_3950_H_INCLUDED
#define ADC_NTCMF52_3950_H_INCLUDED

#include "ADCRawDevice.h"

namespace godot {

// This is a "raw" Analog-Digital-Converter (ADC) device node, which allows a more low-level
// interface to SBC pins that allow analog input.
class ADCNTCMF52_3950 : public ADCRawDevice {
    GDCLASS(ADCNTCMF52_3950, ADCRawDevice)

private:
    
protected: 
    
    float _temperature_kelvin;
    float _temperature_celsius;
    float _temperature_fahrenheit;

    //float A, B, C, lnR;

    static void _bind_methods();

public:

    ADCNTCMF52_3950();
    ~ADCNTCMF52_3950();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.

    void set_temperature_kelvin( float kelvin );
    float get_temperature_kelvin() const;

    void set_temperature_celsius( float celsius );
    float get_temperature_celsius() const;

    void set_temperature_fahrenheit( float fahrenheit );
    float get_temperature_fahrenheit() const;

    
};



}



#endif 