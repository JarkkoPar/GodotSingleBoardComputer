#include "GpioHcSr04.h"
#include "SingleBoardComputer.h"
#include <godot_cpp/classes/engine.hpp>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>


#include <chrono>

using namespace godot;


void trigger_echo_loop( GpioHcSr04* sensor ) { 
    int return_value = 0;
    int echo_value = 0;
    int gpio_trig_pin_fd = sensor->_gpio_trig_pin_fd;
    int gpio_echo_pin_fd = sensor->_gpio_echo_pin_fd;
    struct gpio_v2_line_values data_to_send;
    struct gpio_v2_line_values data_read;
    while( sensor->_end_processing == false ) {
        echo_value = 0;
        // Activate trigger.
        auto start = std::chrono::high_resolution_clock::now();
        memset(&data_to_send, 0, sizeof(data_to_send));
        data_to_send.bits = 1;
        data_to_send.mask = 1; 
        return_value = ioctl(gpio_trig_pin_fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &data_to_send);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        // Deactivate trigger.
        memset(&data_to_send, 0, sizeof(data_to_send));
	    data_to_send.bits = 0;
        data_to_send.mask = 1; 
        return_value = ioctl(gpio_trig_pin_fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &data_to_send);
        // Listen to the echo back going high.
        while( echo_value == 0 ) {
            if( sensor->_end_processing ) return;
            start = std::chrono::high_resolution_clock::now();
            memset(&data_read, 0, sizeof(data_read));
            data_read.mask = 1;
            return_value = ioctl( gpio_echo_pin_fd, GPIO_V2_LINE_GET_VALUES_IOCTL, &data_read);
            echo_value = (uint8_t)data_read.bits;
        }
        // Listen to the echo back going low.
        auto end = std::chrono::high_resolution_clock::now();
        while( echo_value == 1 ) {
            if( sensor->_end_processing ) return;
            end = std::chrono::high_resolution_clock::now();
            memset(&data_read, 0, sizeof(data_read));
            data_read.mask = 1;
            return_value = ioctl( gpio_echo_pin_fd, GPIO_V2_LINE_GET_VALUES_IOCTL, &data_read);
            echo_value = (uint8_t)data_read.bits;
        }
        auto duration_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        {
            std::lock_guard<std::mutex> lock(sensor->_distance_polling_mutex);
            sensor->_distance_mm = 10.0f * (float)(duration_microseconds.count() * 0.034f * 0.5f);
            sensor->_distance_inch = sensor->_distance_mm * 0.03937007874f;
        }
        // Seems to need this pause to work properly.
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}


GpioHcSr04::GpioHcSr04() {
    _gpio_trig_pin_index = 5;
    _gpio_echo_pin_index = 6;
    _gpio_trig_pin_device_fd = -1;
    _gpio_echo_pin_device_fd = -1;
    _gpio_trig_pin_fd = -1;
    _gpio_echo_pin_fd = -1;

    _distance_mm = 0.0f;
    _distance_inch = 0.0f;

    _run_trigger_echo_loop = true;
}

GpioHcSr04::~GpioHcSr04() { 
}


void GpioHcSr04::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_gpio_trig_pin_index", "echo_pin_number"), &GpioHcSr04::set_gpio_trig_pin_index);
	ClassDB::bind_method(D_METHOD("get_gpio_trig_pin_index"), &GpioHcSr04::get_gpio_trig_pin_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "gpio_triggering_pin_number", PROPERTY_HINT_RANGE, "1,40"), "set_gpio_trig_pin_index", "get_gpio_trig_pin_index");

    ClassDB::bind_method(D_METHOD("set_gpio_echo_pin_index", "echo_pin_number"), &GpioHcSr04::set_gpio_echo_pin_index);
	ClassDB::bind_method(D_METHOD("get_gpio_echo_pin_index"), &GpioHcSr04::get_gpio_echo_pin_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "gpio_echoing_pin_number", PROPERTY_HINT_RANGE, "1,40"), "set_gpio_echo_pin_index", "get_gpio_echo_pin_index");

    ClassDB::bind_method(D_METHOD("set_distance_mm", "distance_mm"), &GpioHcSr04::set_distance_mm);
	ClassDB::bind_method(D_METHOD("get_distance_mm"), &GpioHcSr04::get_distance_mm);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance_mm", PROPERTY_HINT_RANGE, "0.0,2000.0,allow_greater,suffix:mm", PROPERTY_USAGE_READ_ONLY), "set_distance_mm", "get_distance_mm");

    ClassDB::bind_method(D_METHOD("set_distance_inch", "distance_inch"), &GpioHcSr04::set_distance_inch);
	ClassDB::bind_method(D_METHOD("get_distance_inch"), &GpioHcSr04::get_distance_inch);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance_inch", PROPERTY_HINT_RANGE, "0.0,2000.0,allow_greater,suffix:inch", PROPERTY_USAGE_READ_ONLY), "set_distance_inch", "get_distance_inch");

}

// Godot virtuals.

void GpioHcSr04::_process(double delta) {

}

void GpioHcSr04::_physics_process(double delta) {

}

void GpioHcSr04::_notification(int p_what) {

}


// Getters and setters.

void GpioHcSr04::set_gpio_trig_pin_index( int trig_pin_number ) {
    _gpio_trig_pin_index = trig_pin_number - 1;
}

int  GpioHcSr04::get_gpio_trig_pin_index() const {
    return _gpio_trig_pin_index + 1;
}
    
void GpioHcSr04::set_gpio_echo_pin_index( int echo_pin_number ) {
    _gpio_echo_pin_index = echo_pin_number - 1;
}

int  GpioHcSr04::get_gpio_echo_pin_index() const {
    return _gpio_echo_pin_index + 1;
}   

void GpioHcSr04::set_distance_mm( float distance_mm ) {
    std::lock_guard<std::mutex> lock(_distance_polling_mutex);
    _distance_mm = distance_mm;
}

float GpioHcSr04::get_distance_mm() {
    std::lock_guard<std::mutex> lock(_distance_polling_mutex);
    return _distance_mm;
}

void GpioHcSr04::set_distance_inch( float distance_inch ) {
    std::lock_guard<std::mutex> lock(_distance_polling_mutex);
    _distance_inch = distance_inch;
}

float GpioHcSr04::get_distance_inch() {
    std::lock_guard<std::mutex> lock(_distance_polling_mutex);
    return _distance_inch;
}


// Device handling.

void GpioHcSr04::_configure_gpio_device() {

    // Get the parent which should have the gpio pin array.
    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");

    ERR_FAIL_INDEX_EDMSG(_gpio_trig_pin_index, 40, "Invalid index for gpio trigger pin (out of bounds).");
    ERR_FAIL_INDEX_EDMSG(_gpio_echo_pin_index, 40, "Invalid index for gpio echo pin (out of bounds).");
    
    // Open the selected gpio file.
    _gpio_trig_pin_device_fd = sbc->request_gpio_device_file(_gpio_trig_pin_index);
    ERR_FAIL_COND_MSG(_gpio_trig_pin_device_fd < 0, "Request for gpio trigger pin device file failed.");

    _gpio_echo_pin_device_fd = sbc->request_gpio_device_file(_gpio_echo_pin_index);
    ERR_FAIL_COND_MSG(_gpio_echo_pin_device_fd < 0, "Request for gpio echo pin device file failed.");    

    // Request the triggering pin.
    _gpio_trig_pin_fd = request_pin_number(_gpio_trig_pin_index + 1, GPIO_TYPE_OUTPUT, (char *)this->get_name().to_ascii_buffer().ptr() );
    ERR_FAIL_COND_MSG(_gpio_trig_pin_fd < 0, "Failed to get line handle from gpio device for the HC-SR04 triggering pin.");
    
    // Request the echoing pin.
    _gpio_echo_pin_fd = request_pin_number(_gpio_echo_pin_index + 1, GPIO_TYPE_INPUT, (char *)this->get_name().to_ascii_buffer().ptr() );
    ERR_FAIL_COND_MSG(_gpio_echo_pin_fd < 0, "Failed to get line handle from gpio device for the HC-SR04 echoing pin.");
    
    // Start th distance polling thread.
    _end_processing = false;
    _distance_polling_thread = std::thread(trigger_echo_loop, this);
}


void GpioHcSr04::_deinitialize_device() {
        
    {
        std::lock_guard<std::mutex> lock(_distance_polling_mutex);
        _end_processing = true;
    }
    if( _distance_polling_thread.joinable() ) {
        _distance_polling_thread.join();
    }

    // Close the pin file descriptors if they are still open.
    if( _gpio_trig_pin_fd > -1 ) {
        close( _gpio_trig_pin_fd );
        _gpio_trig_pin_fd = -1;
    }
    _gpio_trig_pin_device_fd = -1; // just reset the main gpio file descriptor, as this will be closed by the SingleBoardComputer class.

    if( _gpio_echo_pin_fd > -1 ) {
        close( _gpio_echo_pin_fd );
        _gpio_echo_pin_fd = -1;
    }
    _gpio_echo_pin_device_fd = -1; // just reset the main gpio file descriptor, as this will be closed by the SingleBoardComputer class.
}

