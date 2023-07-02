#ifndef I2C_RAWDEVICE_H_INCLUDED
#define I2C_RAWDEVICE_H_INCLUDED 

#include "SBCDevice.h"
//#include <godot_cpp/classes/node.hpp>
//#include <godot_cpp/classes/poolbytearray.hpp>


namespace godot {

// This is a "raw" i2c device node, which allows a more low-level
// communication interface with any i2c device.
class I2CRawDevice : public SBCDevice {
    GDCLASS(I2CRawDevice, SBCDevice)

private:
    
protected: 
    
    int _i2c_device_fd;
    int _i2c_device_id;
    int _i2c_device_address; 
    unsigned char _i2c_read_buffer[64];
    unsigned char _i2c_write_buffer[32];
    PackedByteArray _i2c_godot_read_buffer;
    PackedByteArray _i2c_godot_write_buffer;

    static void _bind_methods();

public:

    I2CRawDevice();
    ~I2CRawDevice();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.
    void set_i2c_device_index( int index );
    int  get_i2c_device_index() const;

    void set_i2c_device_address( int address );
    int  get_i2c_device_address() const;

    // Device handling.
    virtual void _initialize_device() override;

    void open_device();
    void close_device();

    void _read_bytes_from_device(const int length);
    void _write_bytes_to_device(const char* buffer, int length);

    PackedByteArray read_bytes_from_device( const int length );
    void write_bytes_to_device( PackedByteArray bytes );
    
    int read_byte_from_device_register( int i2c_device_register );
    void    write_byte_to_device_register( int i2c_device_register, int value );
    void    write_byte_array_to_device_register( int i2c_device_register, PackedByteArray bytes );
    void    _write_byte_array_to_device_register( uint8_t i2c_device_register, const uint8_t* bytes );
};

}

#endif 