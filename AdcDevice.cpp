#include "AdcDevice.h"

#include "SingleBoardComputer.h"

#include <unistd.h>
#include <fcntl.h>

#include <godot_cpp/classes/engine.hpp>

using namespace godot;


AdcDevice::AdcDevice() {
    _adc_device_fd = -1; 
    _adc_gpio_pin_index = -1;
    _adc_pin_voltage = 0.0f;
    _adc_pin_value = 0;
    _adc_voltage_raw_file_index = -1;
}


AdcDevice::~AdcDevice() {
}



void AdcDevice::_bind_methods() {

    // The voltage of the pin and its value between 0..1023.
    ClassDB::bind_method(D_METHOD("set_adc_gpio_pin_number", "pin_number"), &AdcDevice::set_adc_gpio_pin_index);
	ClassDB::bind_method(D_METHOD("get_adc_gpio_pin_number"), &AdcDevice::get_adc_gpio_pin_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "adc_gpio_pin_number", PROPERTY_HINT_RANGE, "1,40"), "set_adc_gpio_pin_number", "get_adc_gpio_pin_number");

    ClassDB::bind_method(D_METHOD("set_adc_gpio_pin_value", "value"), &AdcDevice::set_adc_pin_value);
	ClassDB::bind_method(D_METHOD("get_adc_gpio_pin_value"), &AdcDevice::get_adc_pin_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "adc_gpio_pin_value", PROPERTY_HINT_NONE), "set_adc_gpio_pin_value", "get_adc_gpio_pin_value");

    ClassDB::bind_method(D_METHOD("set_adc_gpio_pin_voltage", "value"), &AdcDevice::set_adc_pin_voltage);
	ClassDB::bind_method(D_METHOD("get_adc_gpio_pin_voltage"), &AdcDevice::get_adc_pin_voltage);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "adc_gpio_pin_voltage", PROPERTY_HINT_NONE), "set_adc_gpio_pin_voltage", "get_adc_gpio_pin_voltage");

}



void AdcDevice::_process(double delta) {
}


void AdcDevice::_physics_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) return;
    read_value_from_pin();
}


void AdcDevice::_notification(int p_what) {
}


// Getters and setters.

void AdcDevice::set_adc_gpio_pin_index( int pin_number ) {
    // If just loading, simply set the value.
    int pin_index = pin_number - 1;
    if( is_inside_tree() == false ){
        _adc_gpio_pin_index = pin_index;
        return; 
    }

    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");
    ERR_FAIL_COND_MSG(pin_index < 0 || pin_index >= sbc->get_num_gpio_pins(), "Pin number must be between 1 and num_pins. First valid pin number is 1.");
    ERR_FAIL_COND_MSG(!sbc->get_gpio_pins()[pin_index].has_pin_function(GpioPin::GpioPinFunction::GPF_ADC_IN), "The selected pin does not have the ADC function.");

    _adc_gpio_pin_index = pin_index;
}

int AdcDevice::get_adc_gpio_pin_index() const {
    return _adc_gpio_pin_index + 1;
}


void AdcDevice::set_adc_pin_voltage( float voltage ) {
    _adc_pin_voltage = voltage;
}

float AdcDevice::get_adc_pin_voltage() const {
    return _adc_pin_voltage;
}

void AdcDevice::set_adc_pin_value( int value ) {
    _adc_pin_value = value;
}

int AdcDevice::get_adc_pin_value() const {
    return _adc_pin_value;
}


// Device handling

bool AdcDevice::_initialize_device() {
    if( !_open_adc_device() ) return false;
    return _configure_adc_device();
}



bool AdcDevice::_open_adc_device() {

    // Get the parent which should have the gpio pin array.
    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_V_MSG(sbc == nullptr, false, "This node needs to be a child of the SingleBoardComputer node.");

    ERR_FAIL_COND_V_MSG(_adc_gpio_pin_index < 0 || _adc_gpio_pin_index >= sbc->get_num_gpio_pins(), false, "Invalid pin number for gpio pin (out of bounds).");
    ERR_FAIL_COND_V_MSG(!sbc->get_gpio_pins()[_adc_gpio_pin_index].has_pin_function(GpioPin::GpioPinFunction::GPF_ADC_IN), false, "The selected pin does not have the ADC function.");

    // Get the raw file index
    _adc_device_raw_file_index = sbc->get_gpio_pins()[_adc_gpio_pin_index].get_adc_device_file_index();
    _adc_voltage_raw_file_index = sbc->get_gpio_pins()[_adc_gpio_pin_index].get_adc_voltage_file_index();
    ERR_FAIL_COND_V_MSG(_adc_device_raw_file_index < 0 || _adc_voltage_raw_file_index < 0, false, "ADC raw file identification for the voltage reading failed.");

    _adc_pin_max_voltage = sbc->get_gpio_pins()[_adc_gpio_pin_index].get_adc_max_voltage();

    return true;
}


bool AdcDevice::_configure_adc_device() {
    return true;
}


void AdcDevice::_deinitialize_device() {
    _adc_device_fd = -1; // just reset the adc file descriptor, as this will be closed by the SingleBoardComputer class.
}


int AdcDevice::read_value_from_pin() {
    //ERR_FAIL_COND_V_MSG(result == nullptr, 0, "Result variable given is a null-pointer.");
    ERR_FAIL_COND_V_MSG(_adc_device_raw_file_index < 0 || _adc_voltage_raw_file_index < 0, 0, "Read value from adc failed, no raw file index set for voltage reading.");

    char filename[64] = {0}; 
    sprintf(filename, "/sys/bus/iio/devices/iio:device%i/in_voltage%i_raw", _adc_device_raw_file_index, _adc_voltage_raw_file_index);
    int voltage_file_fd = open(filename, O_RDONLY);
    ERR_FAIL_COND_V_MSG(voltage_file_fd < 0, 0, "Failed to open the in_voltage file.");

    char read_data[16] = {0};
    ssize_t num_chars_read = read(voltage_file_fd, read_data, 16);
    close(voltage_file_fd);
    ERR_FAIL_COND_V_MSG(num_chars_read <= 0, 0, "Failed to read content from the in_voltage file.");


    int tempval = 0;
    int success = sscanf(read_data, "%d", &tempval);
    ERR_FAIL_COND_V_MSG(success != 1, 0, "Failed to convert the voltage value read to an integer.");

    // Store the value in the range 0..1023 and then calculate the voltage.
    _adc_pin_value = tempval;
    _adc_pin_voltage = _adc_pin_max_voltage * (float)tempval / 1024.0f;

    return _adc_pin_value;
}



//WARN_PRINT("ObjectDB instances leaked at exit (run with --verbose for details).");
