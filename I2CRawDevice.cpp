#include "I2CRawDevice.h"
#include "I2CBus.h"
#include "SingleBoardComputer.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#ifndef I2C_M_RD
    #include <linux/i2c.h>
#endif 

using namespace godot;




I2CRawDevice::I2CRawDevice() {
    _i2c_device_index = 0;
    _i2c_device_bus_number = 0;
    _i2c_device_fd = -1; 
}


I2CRawDevice::~I2CRawDevice() {
    if( _i2c_device_fd >= 0 ) {
        //close(_i2c_device_fd ); // Closed by the SBC.
        _i2c_device_fd = -1;
    }
}



void I2CRawDevice::_bind_methods() {

    // Device Number on the SCB.
    ClassDB::bind_method(D_METHOD("set_i2c_device_bus_number", "bus_number"), &I2CRawDevice::set_i2c_device_bus_number);
	ClassDB::bind_method(D_METHOD("get_i2c_device_bus_number"), &I2CRawDevice::get_i2c_device_bus_number);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "I2C Bus Number", PROPERTY_HINT_NONE, ""), "set_i2c_device_bus_number", "get_i2c_device_bus_number");


    // Device ID on the SCB.
    //ClassDB::bind_method(D_METHOD("set_i2c_device_index", "device_index"), &I2CRawDevice::set_i2c_device_index);
	//ClassDB::bind_method(D_METHOD("get_i2c_device_index"), &I2CRawDevice::get_i2c_device_index);
    //ADD_PROPERTY(PropertyInfo(Variant::INT, "I2C Device", PROPERTY_HINT_NONE, ""), "set_i2c_device_index", "get_i2c_device_index");

    // Address.
    ClassDB::bind_method(D_METHOD("set_i2c_device_address", "device_address"), &I2CRawDevice::set_i2c_device_address);
	ClassDB::bind_method(D_METHOD("get_i2c_device_address"), &I2CRawDevice::get_i2c_device_address);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "I2C Device Address", PROPERTY_HINT_NONE, ""), "set_i2c_device_address", "get_i2c_device_address");

    // Read and write methods.
    ClassDB::bind_method(D_METHOD("open_device"), &I2CRawDevice::open_device);
    ClassDB::bind_method(D_METHOD("close_device"), &I2CRawDevice::close_device);
    ClassDB::bind_method(D_METHOD("read_bytes_from_device", "num_bytes"), &I2CRawDevice::read_bytes_from_device);
    ClassDB::bind_method(D_METHOD("write_bytes_to_device", "bytes_to_write"), &I2CRawDevice::write_bytes_to_device);
}



void I2CRawDevice::_process(double delta) {

}


void I2CRawDevice::_physics_process(double delta) {

}


void I2CRawDevice::_notification(int p_what) {

}

// Getters and setters.


void I2CRawDevice::set_i2c_device_bus_number( int bus_number ) {
    // Try and find the device.
    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");

    int num_i2c_buses = sbc->get_num_i2c_buses();
    for( int i = 0; i < num_i2c_buses; ++i ) {
        I2CBus* i2c_device = sbc->get_i2c_bus(i);
        if( i2c_device == nullptr ) {
            continue;
        }

        if( i2c_device->get_i2c_device_file_index() == bus_number ) {
            _i2c_device_index = i;
            _i2c_device_bus_number = bus_number;
            return;
        }
    }
    
    // No such device bus number.
    _i2c_device_index = -1;
    // todo: add a list of valid numbers here!
    ERR_FAIL_MSG("Invalid I2C bus number selected, consult the board spec sheet for valid I2C bus numbers.");

}

int  I2CRawDevice::get_i2c_device_bus_number() const {
    return _i2c_device_bus_number;
}


void I2CRawDevice::set_i2c_device_index( int index ) {
    // If the device is open, cannot change the index.
    ERR_FAIL_COND_MSG(_i2c_device_fd > -1, "Cannot change device index when the device file is open.");

    _i2c_device_index = index;
}


int  I2CRawDevice::get_i2c_device_index() const {
    return _i2c_device_index;
}

void I2CRawDevice::set_i2c_device_address( int address ) {
    // If the device is open, cannot change the address.
    ERR_FAIL_COND_MSG(_i2c_device_fd > -1, "Cannot change device address when the device file is open.");

    _i2c_device_address = address;
}

int  I2CRawDevice::get_i2c_device_address() const {
    return _i2c_device_address;
}



// Device handling

void I2CRawDevice::_initialize_device() {
    open_device();
}



void I2CRawDevice::open_device() {
    // If a device file descriptor exists, close the device
    // before opening a new one. 
    //if( _i2c_device_fd > -1 ) {
    //    close_device();
    //}

    // Get the parent which should have the i2c bus array.
    SingleBoardComputer* sbc = Object::cast_to<SingleBoardComputer>(get_parent());
    ERR_FAIL_COND_MSG(sbc == nullptr, "This node needs to be a child of the SingleBoardComputer node.");

    ERR_FAIL_COND_MSG(_i2c_device_index < 0 || _i2c_device_index >= sbc->get_num_i2c_buses(), "Invalid index for I2C bus. Have you set the I2C device number?");


    //I2CBus* selectedBus = sbc->get_i2c_bus(_i2c_device_id);
    //ERR_FAIL_COND_MSG(selectedBus == nullptr, "The selected bus returned nullptr.");


    // Open the selected bus file.
    //int dfi = selectedBus->get_i2c_device_file_index();
    //char device_filename_buffer[64] = {0};
    //sprintf(device_filename_buffer, "/dev/i2c-%i\0", dfi);   
    //_i2c_device_fd = open(device_filename_buffer, O_RDWR);
    _i2c_device_fd = sbc->request_i2c_device_file(_i2c_device_index);
    ERR_FAIL_COND_MSG(_i2c_device_fd < 0, "Failed to open the device file.");


    // Try to set the address for communications.  
    int ioctl_retval = ioctl(_i2c_device_fd, I2C_SLAVE, _i2c_device_address);
    ERR_FAIL_COND_MSG(ioctl_retval < 0, "Failed to set the slave device address.");

}

void I2CRawDevice::close_device() {
    //ERR_FAIL_COND_MSG(_i2c_device_fd < 0, "Failed to close device, it is already closed.");
    
    //close(_i2c_device_fd);
    _i2c_device_fd = -1; // just reset the file descriptor.
}


void I2CRawDevice::_read_bytes_from_device(const int length) {
    ERR_FAIL_COND_MSG(_i2c_device_fd < 0, "Read failed because the device file is not opened.");
    ERR_FAIL_COND_MSG( read( _i2c_device_fd, _i2c_read_buffer, length ) != length, "Read failed.");
}

void I2CRawDevice::_write_bytes_to_device( const char* buffer, const int length) {
    ERR_FAIL_COND_MSG(_i2c_device_fd < 0, "Write failed because the device file is not opened.");
    ERR_FAIL_COND_MSG( write( _i2c_device_fd, buffer, length ) != length, "Write failed.");
}

PackedByteArray I2CRawDevice::read_bytes_from_device( const int length ) {
    memset(_i2c_read_buffer, 0, sizeof(_i2c_read_buffer) );
    _read_bytes_from_device(length);
    PackedByteArray retVal = PackedByteArray();
    retVal.resize(64);
    memcpy((uint8_t *)retVal.ptr(), _i2c_read_buffer, 64 ); 

    return retVal;
}

void I2CRawDevice::write_bytes_to_device( PackedByteArray bytes ) {
    int data_length = bytes.size();
    _write_bytes_to_device((char *)bytes.ptr(), data_length);
}


uint8_t I2CRawDevice::read_byte_from_device_register( uint8_t i2c_device_register ) {
    ERR_FAIL_COND_V_MSG(_i2c_device_fd < 0, 0, "Read device register failed because the device file is not opened. Did you forget to call open_device()?");

    uint8_t output_buffer[1] = {i2c_device_register}, input_buffer[1] = {0};
    struct i2c_msg messages[2];
    struct i2c_rdwr_ioctl_data message_set[1];

    messages[0].addr = _i2c_device_address;
    messages[0].flags = 0;
    messages[0].len = 1; 
    messages[0].buf = output_buffer;

    messages[1].addr = _i2c_device_address;
    messages[1].flags = I2C_M_RD | I2C_M_NOSTART;
    messages[1].len = 1;
    messages[1].buf = input_buffer;

    message_set[0].msgs = messages;
    message_set[0].nmsgs = 2; 

    ERR_FAIL_COND_V_MSG(ioctl(_i2c_device_fd, I2C_RDWR, &message_set) < 0, 0,"Read byte from i2c device register failed.");
    return input_buffer[0];
}


void I2CRawDevice::write_byte_to_device_register( uint8_t i2c_device_register, uint8_t value ) {
    ERR_FAIL_COND_MSG(_i2c_device_fd < 0, "Write device register failed because the device file is not opened. Did you forget to call open_device()?");

    uint8_t output_buffer[2] = {i2c_device_register, value};

    struct i2c_msg messages[1];
    struct i2c_rdwr_ioctl_data message_set[1];

    messages[0].addr = _i2c_device_address;
    messages[0].flags = 0;
    messages[0].len = 2;
    messages[0].buf = output_buffer;

    message_set[0].msgs = messages;
    message_set[0].nmsgs = 1;
    
    ERR_FAIL_COND_MSG(ioctl(_i2c_device_fd, I2C_RDWR, &message_set) < 0, "Write byte to i2c device register failed.");

}

void I2CRawDevice::write_byte_array_to_device_register( int i2c_device_register, PackedByteArray bytes ) {
    // todo: code
    ERR_FAIL_MSG("write_byte_array_to_device_register is not implemented yet.");
}

void I2CRawDevice::_write_byte_array_to_device_register( uint8_t i2c_device_register, const uint8_t* bytes ) {
    ERR_FAIL_COND_MSG(_i2c_device_fd < 0, "Write device register failed because the device file is not opened. Did you forget to call open_device()?");

    //uint8_t output_buffer[2] = {(uint8_t)i2c_device_register, (uint8_t)value};
    int num_bytes = *(&bytes + 1) - bytes;
    uint8_t* output_buffer = new uint8_t[num_bytes + 1];
    output_buffer[0] = i2c_device_register; // The first element is the register.
    memcpy(&output_buffer[1], bytes, num_bytes); // Then the rest are the bytes.

    struct i2c_msg messages[1];
    struct i2c_rdwr_ioctl_data message_set[1];

    messages[0].addr = _i2c_device_address;
    messages[0].flags = 0;
    messages[0].len = num_bytes + 1;
    messages[0].buf = output_buffer;

    message_set[0].msgs = messages;
    message_set[0].nmsgs = 1;
    
    ERR_FAIL_COND_MSG(ioctl(_i2c_device_fd, I2C_RDWR, &message_set) < 0, "Write byte to i2c device register failed.");

}


//WARN_PRINT("ObjectDB instances leaked at exit (run with --verbose for details).");