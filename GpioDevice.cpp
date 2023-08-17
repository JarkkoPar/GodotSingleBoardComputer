#include "GpioDevice.h"
#include "GpioPin.h"
#include "SingleBoardComputer.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>

using namespace godot;


GpioDevice::GpioDevice() {
    }


GpioDevice::~GpioDevice() {
}



void GpioDevice::_bind_methods() {

    // Gpio Pin Number on the SCB.
    //ClassDB::bind_method(D_METHOD("set_gpio_pin_number", "pin_number"), &GpioDevice::set_gpio_pin_index);
	//ClassDB::bind_method(D_METHOD("get_gpio_pin_number"), &GpioDevice::get_gpio_pin_index);
    //ADD_PROPERTY(PropertyInfo(Variant::INT, "gpio_pin_number", PROPERTY_HINT_RANGE, "1,40"), "set_gpio_pin_number", "get_gpio_pin_number");

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


// Device handling

int GpioDevice::request_pin_number( int pin_number, int pin_type, const char* consumer_name ) {

    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_V_MSG(sbc == nullptr, -1, "This node needs to be a child of the SingleBoardComputer node.");

    // Get the gpio file for the pin and the offset.
    int pin_index = pin_number - 1;
    ERR_FAIL_INDEX_V_EDMSG( pin_index, 40, -1, "Invalid index for gpio pin (out of bounds).");

    int gpio_device_fd = sbc->request_gpio_device_file(pin_index);
    ERR_FAIL_COND_V_MSG(gpio_device_fd < 0, -1, "Request for gpio device file failed.");

    int pin_offset = sbc->get_gpio_pin_offset(pin_index);
    
    // Request for the line to the selected pin.
    struct gpio_v2_line_request line_request;
    memset(&line_request, 0, sizeof(line_request));
    
    if( pin_type == GPIO_TYPE_OUTPUT ) {
        line_request.config.flags = GPIO_V2_LINE_FLAG_OUTPUT;
    }
    else if( pin_type == GPIO_TYPE_INPUT ) {
        line_request.config.flags = GPIO_V2_LINE_FLAG_INPUT;
    }
    
    line_request.config.num_attrs = 0;
    if( consumer_name == nullptr ) {
        strcpy(line_request.consumer, "GodotEngine");
    } else {
        strcpy(line_request.consumer, consumer_name);
    
    } 
    line_request.num_lines = 1;
    line_request.offsets[0] = pin_offset;
    
    int return_value = ioctl(gpio_device_fd, GPIO_V2_GET_LINE_IOCTL, &line_request);
    ERR_FAIL_COND_V_MSG(return_value < 0, -1, "Failed to get line handle from gpio device.");

    // All OK so return the file descriptor.
    int gpio_pin_fd = line_request.fd; 
    return gpio_pin_fd;
}





void GpioDevice::_initialize_device() {
    _open_gpio_device();
    _configure_gpio_device();
}



void GpioDevice::_open_gpio_device() {

}

void GpioDevice::_configure_gpio_device() {

}


void GpioDevice::_deinitialize_device() {
}


void GpioDevice::write_byte_to_device( int gpio_pin_fd, uint8_t data ) {
    ERR_FAIL_COND_MSG( gpio_pin_fd < 0, "Write byte to device failed, no file descriptor.");
    //struct gpiohandle_data data_to_send;
    //data_to_send.values[0] = data;
    //int return_value = ioctl(_gpio_pin_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data_to_send);
    struct gpio_v2_line_values data_to_send;
    memset(&data_to_send, 0, sizeof(data_to_send));
	uint64_t mask = 0, bits = 0;
    data_to_send.bits = data;
    data_to_send.mask = 1; 

    int return_value = ioctl(gpio_pin_fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &data_to_send);
    ERR_FAIL_COND_MSG(return_value < 0, "Failed write data to gpio device.");
}

// Returns number of bytes read, which is 0 in case of error, 1 otherwise.
int  GpioDevice::read_byte_from_device( int gpio_pin_fd, uint8_t* result ) {
    ERR_FAIL_COND_V_MSG(result == nullptr, 0, "Result variable given is a null-pointer.");
    ERR_FAIL_COND_V_MSG(gpio_pin_fd < 0, 0, "Read byte from device failed, no file descriptor.");
    
    //struct gpiohandle_data data_read;
    struct gpio_v2_line_values data_read;
    memset(&data_read, 0, sizeof(data_read));
    //data_read.bits = 1; // Read the last bit.
    data_read.mask = 1;
    int ret_val = ioctl( gpio_pin_fd, GPIO_V2_LINE_GET_VALUES_IOCTL, &data_read);
    ERR_FAIL_COND_V_MSG(ret_val < 0, 0, "Failed write data to gpio device.");
    *result = (uint8_t)data_read.bits;
    return 1;
}



//WARN_PRINT("ObjectDB instances leaked at exit (run with --verbose for details).");