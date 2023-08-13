#include "GpioDevice.h"
#include "GpioPin.h"
#include "SingleBoardComputer.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>

using namespace godot;




GpioDevice::GpioDevice() {
    _gpio_device_fd = -1; 
    _gpio_pin_fd = -1;
    //_gpio_pin_offset = -1;
}


GpioDevice::~GpioDevice() {
    close_device();
    _gpio_device_fd = -1;
    //_gpio_pin_offset = -1;
}



void GpioDevice::_bind_methods() {

    // Gpio Pin Number on the SCB.
    ClassDB::bind_method(D_METHOD("set_gpio_pin_number", "pin_number"), &GpioDevice::set_gpio_pin_index);
	ClassDB::bind_method(D_METHOD("get_gpio_pin_number"), &GpioDevice::get_gpio_pin_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "gpio_pin_number", PROPERTY_HINT_RANGE, "1,40"), "set_gpio_pin_number", "get_gpio_pin_number");

    // Read and write methods.
    //ClassDB::bind_method(D_METHOD("open_device"), &I2CRawDevice::open_device);
    //ClassDB::bind_method(D_METHOD("close_device"), &I2CRawDevice::close_device);
    //ClassDB::bind_method(D_METHOD("read_bytes_from_device", "num_bytes"), &I2CRawDevice::read_bytes_from_device);
    //ClassDB::bind_method(D_METHOD("write_bytes_to_device", "bytes_to_write"), &I2CRawDevice::write_bytes_to_device);
}



void GpioDevice::_process(double delta) {

}


void GpioDevice::_physics_process(double delta) {

}


void GpioDevice::_notification(int p_what) {

}

// Getters and setters.


void GpioDevice::set_gpio_pin_index( int pin_number ) {
    // If just loading, simply set the value.
    int pin_index = pin_number - 1;
    if( is_inside_tree() == false ){
        _gpio_pin_index = pin_index;
        return; 
    }

    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");

    ERR_FAIL_COND_MSG(pin_index < 0 || pin_index >= sbc->get_num_gpio_pins(), "Pin index must be between 0 and num_pins-1. First valid pin index is 0.");

    _gpio_pin_index = pin_index;
}

int GpioDevice::get_gpio_pin_index() const {
    return _gpio_pin_index + 1;
}


void GpioDevice::set_gpio_pin_type( int pin_type ) {
    _gpio_pin_type = pin_type;
}

int  GpioDevice::get_gpio_pin_type() const {
    return _gpio_pin_type;
}

// Device handling

void GpioDevice::_initialize_device() {
    open_device();
}



void GpioDevice::open_device() {

    // Get the parent which should have the gpio pin array.
    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");

    //ERR_FAIL_COND_MSG(_gpio_pin_index < 0 || _gpio_pin_index >= sbc->get_num_gpio_pins(), "Invalid index for gpio pin (out of bounds).");
    ERR_FAIL_INDEX_EDMSG(_gpio_pin_index, 40, "Invalid index for gpio pin (out of bounds).");

    // Open the selected gpio file.
    _gpio_device_fd = sbc->request_gpio_device_file(_gpio_pin_index);
    ERR_FAIL_COND_MSG(_gpio_device_fd < 0, "Request gpio device file failed.");

    // Get the pin offset.
    int pin_offset = sbc->get_gpio_pin_offset(_gpio_pin_index);
    /**
    // Get the pin based on type.
    struct gpiohandle_request request; 

    request.lineoffsets[0] = pin_offset;

    if( _gpio_pin_type == GPIO_TYPE_OUTPUT ) {
        request.flags = GPIOHANDLE_REQUEST_OUTPUT;
    }
    else if( _gpio_pin_type == GPIO_TYPE_INPUT ) {
        request.flags = GPIOHANDLE_REQUEST_INPUT;
    }
    
    request.lines = 1;

    int return_value = ioctl(_gpio_device_fd, GPIO_GET_LINEHANDLE_IOCTL, &request);
    ERR_FAIL_COND_MSG(return_value < 0, "Failed to get line handle from gpio device.");

    // All OK so store the file descriptor.
    _gpio_pin_fd = request.fd; 
    */
    struct gpio_v2_line_request line_request;
    memset(&line_request, 0, sizeof(line_request));
    
    if( _gpio_pin_type == GPIO_TYPE_OUTPUT ) {
        line_request.config.flags = GPIO_V2_LINE_FLAG_OUTPUT;
    }
    else if( _gpio_pin_type == GPIO_TYPE_INPUT ) {
        line_request.config.flags = GPIO_V2_LINE_FLAG_INPUT;
    }
    
    line_request.config.num_attrs = 0;
    strcpy(line_request.consumer, "GodotEngine"); // TODO: Add user definable name
    line_request.num_lines = 1;
    line_request.offsets[0] = pin_offset;
    
    int return_value = ioctl(_gpio_device_fd, GPIO_V2_GET_LINE_IOCTL, &line_request);
    ERR_FAIL_COND_MSG(return_value < 0, "Failed to get line handle from gpio device.");

    // All OK so store the file descriptor.
    _gpio_pin_fd = line_request.fd; 
}


void GpioDevice::close_device() {
    // Close the pin file descriptor if it is still open.
    if( _gpio_pin_fd > -1 ) {
        close( _gpio_pin_fd );
        _gpio_pin_fd = -1;
    }
    _gpio_device_fd = -1; // just reset the main gpio file descriptor, as this will be closed by the SingleBoardComputer class.
}


void GpioDevice::write_byte_to_device( uint8_t data ) {
    ERR_FAIL_COND_MSG(_gpio_pin_fd < 0, "Write byte to device failed, no file descriptor.");
    //struct gpiohandle_data data_to_send;
    //data_to_send.values[0] = data;
    //int return_value = ioctl(_gpio_pin_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data_to_send);
    struct gpio_v2_line_values data_to_send;
    memset(&data_to_send, 0, sizeof(data_to_send));
	uint64_t mask = 0, bits = 0;
    data_to_send.bits = data;
    data_to_send.mask = 1; 

    int return_value = ioctl(_gpio_pin_fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &data_to_send);
    ERR_FAIL_COND_MSG(return_value < 0, "Failed write data to gpio device.");
}

// Returns number of bytes read, which is 0 in case of error, 1 otherwise.
int  GpioDevice::read_byte_from_device( uint8_t* result ) {
    ERR_FAIL_COND_V_MSG(result == nullptr, 0, "Result variable given is a null-pointer.");
    ERR_FAIL_COND_V_MSG(_gpio_pin_fd < 0, 0, "Read byte from device failed, no file descriptor.");
    
    //struct gpiohandle_data data_read;
    struct gpio_v2_line_values data_read;
    memset(&data_read, 0, sizeof(data_read));
    //data_read.bits = 1; // Read the last bit.
    data_read.mask = 1;
    int ret_val = ioctl( _gpio_pin_fd, GPIO_V2_LINE_GET_VALUES_IOCTL, &data_read);
    ERR_FAIL_COND_V_MSG(ret_val < 0, 0, "Failed write data to gpio device.");
    *result = (uint8_t)data_read.bits;
    return 1;
}



//WARN_PRINT("ObjectDB instances leaked at exit (run with --verbose for details).");