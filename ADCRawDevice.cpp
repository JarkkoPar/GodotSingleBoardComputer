#include "ADCRawDevice.h"

#include "SingleBoardComputer.h"

#include <unistd.h>
#include <fcntl.h>

#include <godot_cpp/classes/engine.hpp>

using namespace godot;




ADCRawDevice::ADCRawDevice() {
    _adc_device_fd = -1; 
    _adc_gpio_pin_index = -1;
    _adc_pin_voltage = 0.0f;

    _adc_voltage_raw_file_index = -1;
}


ADCRawDevice::~ADCRawDevice() {
    close_device();
    _adc_device_fd = -1;
    _adc_gpio_pin_index = -1;
    _adc_pin_voltage = 0.0f;
    _adc_voltage_raw_file_index = -1;
}



void ADCRawDevice::_bind_methods() {

    // The voltage of the pin and its value between 0..1023.
    ClassDB::bind_method(D_METHOD("set_adc_gpio_pin_index", "pin_index"), &ADCRawDevice::set_adc_gpio_pin_index);
	ClassDB::bind_method(D_METHOD("get_adc_gpio_pin_index"), &ADCRawDevice::get_adc_gpio_pin_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "ADC Gpio Pin Index", PROPERTY_HINT_RANGE, "0,39"), "set_adc_gpio_pin_index", "get_adc_gpio_pin_index");

    ClassDB::bind_method(D_METHOD("set_adc_gpio_pin_value", "value"), &ADCRawDevice::set_adc_pin_value);
	ClassDB::bind_method(D_METHOD("get_adc_gpio_pin_value"), &ADCRawDevice::get_adc_pin_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "ADC Gpio Pin Value", PROPERTY_HINT_NONE), "set_adc_adc_pin_value", "get_adc_pin_value");


    // Read and write methods.
    //ClassDB::bind_method(D_METHOD("open_device"), &I2CRawDevice::open_device);
    //ClassDB::bind_method(D_METHOD("close_device"), &I2CRawDevice::close_device);
    //ClassDB::bind_method(D_METHOD("read_bytes_from_device", "num_bytes"), &I2CRawDevice::read_bytes_from_device);
    //ClassDB::bind_method(D_METHOD("write_bytes_to_device", "bytes_to_write"), &I2CRawDevice::write_bytes_to_device);
}



void ADCRawDevice::_process(double delta) {

}


void ADCRawDevice::_physics_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) return;
    read_value_from_pin();
}


void ADCRawDevice::_notification(int p_what) {

}

// Getters and setters.


void ADCRawDevice::set_adc_gpio_pin_index( int pin_index ) {
    // If just loading, simply set the value.
    if( is_inside_tree() == false ){
        _adc_gpio_pin_index = pin_index;
        return; 
    }

    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");

    ERR_FAIL_COND_MSG(pin_index < 0 || pin_index >= sbc->get_num_gpio_pins(), "Pin index must be between 0 and num_pins-1. First valid pin index is 0.");

    _adc_gpio_pin_index = pin_index;
}

int ADCRawDevice::get_adc_gpio_pin_index() const {
    return _adc_gpio_pin_index;
}


void ADCRawDevice::set_adc_pin_voltage( float voltage ) {
    _adc_pin_voltage = voltage;
}

float ADCRawDevice::get_adc_pin_voltage() const {
    return _adc_pin_voltage;
}

void ADCRawDevice::set_adc_pin_value( int value ) {
    _adc_pin_value = value;
}

int ADCRawDevice::get_adc_pin_value() const {
    return _adc_pin_value;
}


// Device handling

void ADCRawDevice::_initialize_device() {
    open_device();
}



void ADCRawDevice::open_device() {

    // Get the parent which should have the gpio pin array.
    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");

    ERR_FAIL_COND_MSG(_adc_gpio_pin_index < 0 || _adc_gpio_pin_index >= sbc->get_num_gpio_pins(), "Invalid index for gpio pin (out of bounds).");

    // Get the raw file index
    _adc_voltage_raw_file_index = sbc->get_gpio_pins()[_adc_gpio_pin_index].get_adc_device_file_index();
    ERR_FAIL_COND_MSG(_adc_voltage_raw_file_index < 0, "ADC raw file identification for the voltage reading failed.");
}


void ADCRawDevice::close_device() {
    _adc_device_fd = -1; // just reset the adc file descriptor, as this will be closed by the SingleBoardComputer class.
}


int ADCRawDevice::read_value_from_pin() {
    //ERR_FAIL_COND_V_MSG(result == nullptr, 0, "Result variable given is a null-pointer.");
    ERR_FAIL_COND_V_MSG(_adc_voltage_raw_file_index < 0, 0, "Read value from adc failed, no raw file index set for voltage reading.");

    char filename[64] = {0}; // radxa rock 4 specific path! todo: fix to be more generic
    sprintf(filename, "/sys/bus/iio/devices/iio:device0/in_voltage%i_raw", _adc_voltage_raw_file_index);
    int voltage_file_fd = open(filename, O_RDONLY);
    ERR_FAIL_COND_V_MSG(voltage_file_fd < 0, 0, "Failed to open the in_voltage file.");

    char read_data[16] = {0};
    ssize_t num_chars_read = read(voltage_file_fd, read_data, 16);
    close(voltage_file_fd);
    ERR_FAIL_COND_V_MSG(num_chars_read <= 0, 0, "Failed to read content from the in_voltage file.");


    float ret_val = 0.0f;
    int tempval = 0;
    int success = sscanf(read_data, "%d", &tempval);
    ERR_FAIL_COND_V_MSG(success != 1, 0, "Failed to convert the voltage value read to an integer.");

    _adc_pin_value = tempval;
    // radxa rock 4 specific temporary value! todo: fix to be more generic
    _adc_pin_voltage = 1.8f * (float)tempval / 1024.0f;

    return _adc_pin_value;
}



//WARN_PRINT("ObjectDB instances leaked at exit (run with --verbose for details).");