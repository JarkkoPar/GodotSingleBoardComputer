#include "GpioRawDevice.h"
#include "GpioPin.h"
#include "SingleBoardComputer.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>

using namespace godot;




GpioRawDevice::GpioRawDevice() {
    _gpio_device_fd = -1; 
    _gpio_pin_fd = -1;
}


GpioRawDevice::~GpioRawDevice() {
    close_device();
    _gpio_device_fd = -1;
}



void GpioRawDevice::_bind_methods() {

    // Gpio Pin Number on the SCB.
    ClassDB::bind_method(D_METHOD("set_gpio_pin_index", "pin_index"), &GpioRawDevice::set_gpio_pin_index);
	ClassDB::bind_method(D_METHOD("get_gpio_pin_index"), &GpioRawDevice::get_gpio_pin_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Gpio Pin Index", PROPERTY_HINT_RANGE, "0,39"), "set_gpio_pin_index", "get_gpio_pin_index");

    // Read and write methods.
    //ClassDB::bind_method(D_METHOD("open_device"), &I2CRawDevice::open_device);
    //ClassDB::bind_method(D_METHOD("close_device"), &I2CRawDevice::close_device);
    //ClassDB::bind_method(D_METHOD("read_bytes_from_device", "num_bytes"), &I2CRawDevice::read_bytes_from_device);
    //ClassDB::bind_method(D_METHOD("write_bytes_to_device", "bytes_to_write"), &I2CRawDevice::write_bytes_to_device);
}



void GpioRawDevice::_process(double delta) {

}


void GpioRawDevice::_physics_process(double delta) {

}


void GpioRawDevice::_notification(int p_what) {

}

// Getters and setters.


void GpioRawDevice::set_gpio_pin_index( int pin_index ) {
    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");

    ERR_FAIL_COND_MSG(pin_index < 0 || pin_index >= sbc->get_num_gpio_pins(), "Pin index must be between 0 and num_pins-1. First valid pin index is 0.");

    _gpio_pin_index = pin_index;


    /**
    int num_pins = sbc->get_num_gpio_pins();
    for( int i = 0; i < num_pins; ++i ) {
        GpioPin* gpio_pin = sbc->get_gpio_pin(i); 
        if( gpio_pin == nullptr ) {
            continue;
        }

        if( gpio_pin->get_gpio_device_file_index() == bus_number ) {
            _i2c_device_id = i;
            _i2c_device_bus_number = bus_number;
            return;
        }
    }
    
    // No such device bus number.
    _i2c_device_id = -1;
    // todo: add a list of valid numbers here!
    ERR_FAIL_MSG("Invalid I2C bus number selected, consult the board spec sheet for valid I2C bus numbers.");
    /**/
}

int GpioRawDevice::get_gpio_pin_index() const {
    return _gpio_pin_index;
}

/**
void GpioRawDevice::set_gpio_pin_type( int pin_type ) {

}

int  GpioRawDevice::get_gpio_pin_type() const {

}
/**/

// Device handling

void GpioRawDevice::_initialize_device() {
    open_device();
}



void GpioRawDevice::open_device() {

    // Get the parent which should have the gpio pin array.
    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");

    ERR_FAIL_COND_MSG(_gpio_pin_index < 0 || _gpio_pin_index >= sbc->get_num_gpio_pins(), "Invalid index for gpio pin (out of bounds).");


    //I2CBus* selectedBus = sbc->get_i2c_bus(_i2c_device_id);
    //ERR_FAIL_COND_MSG(selectedBus == nullptr, "The selected bus returned nullptr.");


    // Open the selected bus file.
    //int dfi = selectedBus->get_i2c_device_file_index();
    //char device_filename_buffer[64] = {0};
    //sprintf(device_filename_buffer, "/dev/i2c-%i\0", dfi);   
    //_i2c_device_fd = open(device_filename_buffer, O_RDWR);
    _gpio_device_fd = sbc->request_gpio_device_file(_gpio_pin_index);
    ERR_FAIL_COND_MSG(_gpio_device_fd < 0, "Failed to open the gpio device file.");


    // Try to set the address for communications.  
    //int ioctl_retval = ioctl(_i2c_device_fd, I2C_SLAVE, _i2c_device_address);
    //ERR_FAIL_COND_MSG(ioctl_retval < 0, "Failed to set the slave device address.");
}


void GpioRawDevice::close_device() {
    // Close the pin file descriptor if it is still open.
    if( _gpio_pin_fd > -1 ) {
        close( _gpio_pin_fd );
        _gpio_pin_fd = -1;
    }
    _gpio_device_fd = -1; // just reset the main gpio file descriptor, as this will be closed by the SingleBoardComputer class.
}



//WARN_PRINT("ObjectDB instances leaked at exit (run with --verbose for details).");