#include "GpioHcSr04.h"
#include "SingleBoardComputer.h"
#include <godot_cpp/classes/engine.hpp>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>


#include <chrono>

using namespace godot;

/**
void thread_function_poll_distance( int trig_pin_fd, int echo_pin_fd, float* distance_mm, float* distance_inch, bool* end_processing ) {
    int return_value = 0;
    int echo_value = 0;
    struct gpio_v2_line_values data_to_send;
    struct gpio_v2_line_values data_read;
    std::mutex m;
    while(*end_processing == false ) {
        echo_value = 0;
        memset(&data_to_send, 0, sizeof(data_to_send));
	    //uint64_t mask = 0, bits = 0;
        data_to_send.bits = 1;
        data_to_send.mask = 1; 
        return_value = ioctl(trig_pin_fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &data_to_send);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        
        memset(&data_to_send, 0, sizeof(data_to_send));
	    //uint64_t mask = 0, bits = 0;
        data_to_send.bits = 0;
        data_to_send.mask = 1; 
        return_value = ioctl(trig_pin_fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &data_to_send);
        auto start = std::chrono::high_resolution_clock::now();
        while( *end_processing == false && echo_value == 0 ) {
            start = std::chrono::high_resolution_clock::now();
            memset(&data_read, 0, sizeof(data_read));
            data_read.mask = 1;
            return_value = ioctl( echo_pin_fd, GPIO_V2_LINE_GET_VALUES_IOCTL, &data_read);
            echo_value = (uint8_t)data_read.bits;
        }
        auto end = std::chrono::high_resolution_clock::now();
        while( *end_processing == false && echo_value == 1 ) {
            end = std::chrono::high_resolution_clock::now();
            memset(&data_read, 0, sizeof(data_read));
            data_read.mask = 1;
            return_value = ioctl( echo_pin_fd, GPIO_V2_LINE_GET_VALUES_IOCTL, &data_read);
            echo_value = (uint8_t)data_read.bits;
        }
        auto duration_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        {
            std::lock_guard<std::mutex> lock(m);
            *distance_mm = (float)(duration_microseconds.count() * 0.0017150);
            *distance_inch = *distance_mm * 0.03937007874f;
        }
    }
}
/**/

GpioHcSr04::GpioHcSr04() {
    _is_hcsr04_initialized = false;
    _is_active = true;

    _gpio_trig_pin_index = 5;
    _gpio_echo_pin_index = 6;
    _gpio_trig_pin_device_fd = -1;
    _gpio_echo_pin_device_fd = -1;

    _distance_mm = 0.0f;
    _distance_inch = 0.0f;

    _run_trigger_echo_loop = true;
}

GpioHcSr04::~GpioHcSr04() {
    close_device();    
}


void GpioHcSr04::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_is_active", "is_active"), &GpioHcSr04::set_is_active);
	ClassDB::bind_method(D_METHOD("get_is_active"), &GpioHcSr04::get_is_active);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_active", PROPERTY_HINT_NONE ), "set_is_active", "get_is_active");

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

void GpioHcSr04::set_is_active( bool is_active ) {
    _is_active = is_active;
}

bool GpioHcSr04::get_is_active() const {
    return _is_active;
}


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
    _distance_mm = distance_mm;
}

float GpioHcSr04::get_distance_mm() {
    return _distance_mm;
}

void GpioHcSr04::set_distance_inch( float distance_inch ) {
    _distance_inch = distance_inch;
}

float GpioHcSr04::get_distance_inch() {
    return _distance_inch;
}



// Device handling.

void GpioHcSr04::_initialize_device() {
    // Only initialize once.
    if( _is_hcsr04_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    if( !_is_active ) return;

    open_device();
}


void GpioHcSr04::open_device() {

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


    // Get the pin offsets.
    int trig_pin_offset = sbc->get_gpio_pin_offset(_gpio_trig_pin_index);
    int echo_pin_offset = sbc->get_gpio_pin_offset(_gpio_echo_pin_index);
    

    // Request the triggering pin.
    struct gpio_v2_line_request trig_line_request;
    memset(&trig_line_request, 0, sizeof(trig_line_request));
    trig_line_request.config.flags = GPIO_V2_LINE_FLAG_OUTPUT;
    trig_line_request.config.num_attrs = 0;
    strcpy(trig_line_request.consumer, (char *)this->get_name().to_ascii_buffer().ptr() );//"GpioHcSr04"); // TODO: Add user definable name
    trig_line_request.num_lines = 1;
    trig_line_request.offsets[0] = trig_pin_offset;
    
    int return_value = ioctl(_gpio_trig_pin_device_fd, GPIO_V2_GET_LINE_IOCTL, &trig_line_request);
    ERR_FAIL_COND_MSG(return_value < 0, "Failed to get line handle from gpio device for the HC-SR04 triggering pin.");

    // All OK so store the file descriptor.
    _gpio_trig_pin_fd = trig_line_request.fd; 


    // Request the echoing pin.
    struct gpio_v2_line_request echo_line_request;
    memset(&echo_line_request, 0, sizeof(echo_line_request));
    echo_line_request.config.flags = GPIO_V2_LINE_FLAG_INPUT;    
    echo_line_request.config.num_attrs = 0;
    strcpy(echo_line_request.consumer, (char *)this->get_name().to_ascii_buffer().ptr());//"GpioHcSr04"); // TODO: Add user definable name
    echo_line_request.num_lines = 1;
    echo_line_request.offsets[0] = echo_pin_offset;
    
    return_value = ioctl(_gpio_echo_pin_device_fd, GPIO_V2_GET_LINE_IOCTL, &echo_line_request);
    ERR_FAIL_COND_MSG(return_value < 0, "Failed to get line handle from gpio device for the HC-SR04 echoing pin.");

    // All OK so store the file descriptor.
    _gpio_echo_pin_fd = echo_line_request.fd; 

    // Start th distance polling thread.
    //_end_processing = false;
    //_distance_polling_thread = std::thread(thread_function_poll_distance, _gpio_trig_pin_fd, _gpio_echo_pin_fd, &_distance_mm, &_distance_inch, &_end_processing );
}


void GpioHcSr04::close_device() {
    //_distance_polling_mutex.lock();
    /*{
        std::lock_guard<std::mutex> lock(_distance_polling_mutex);
        _end_processing = true;
    }
    if( _distance_polling_thread.joinable() ) {
        _distance_polling_thread.join();
    }*/

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

