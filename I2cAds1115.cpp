#include "I2cAds1115.h"


using namespace godot;


I2cAds1115::I2cAds1115() {

    _i2c_device_address = 0x48;
    _ain0_value = 0;
    _ain1_value = 0;
    _ain2_value = 0;
    _ain3_value = 0;

    // The default config
    _config =  ADS1115InputMultiplexerConfiguration::P_AIN0_N_AIN1   |
               ADS1115ProgrammableGainAmplifierConfiguration::GAIN_2 |
               ADS1115DeviceOperatingMode::SINGLE_SHOT |
               ADS1115DataRate::SPS_128 |
               ADS1115ComparatorMode::TRADITIONAL |
               ADS1115ComparatorPolarity::ACTIVE_LOW ||
               ADS1115LatchingComparator::NON_LATCHING |
               ADS1115ComparatorQueueAndDisable::DISABLE
    ;


}

I2cAds1115::~I2cAds1115() {
    
}


void I2cAds1115::_bind_methods() {
    
}


// Godot virtuals.

void I2cAds1115::_process(double delta) {

}

void I2cAds1115::_physics_process(double delta) {

}

void I2cAds1115::_notification(int p_what) {

}
    
// Device handling.

bool I2cAds1115::_configure_i2c_device() {

    
    return true;
} 

