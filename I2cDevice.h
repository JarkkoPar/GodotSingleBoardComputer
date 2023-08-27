#ifndef I2C_DEVICE_H_INCLUDED
#define I2C_DEVICE_H_INCLUDED 

#include "SBCDevice.h"
//#include <godot_cpp/classes/node.hpp>
//#include <godot_cpp/classes/poolbytearray.hpp>


namespace godot {

// This is a "raw" i2c device node, which allows a more low-level
// communication interface with any i2c device.
class I2cDevice : public SBCDevice {
    GDCLASS(I2cDevice, SBCDevice)

private:
    
protected: 
    
    int _i2c_device_fd;             // The file for the i2c device, only used when running the app, not in editor
    int _i2c_device_index;          // Index in the internal array of the SingleBoardComputer class
    int _i2c_device_bus_number;     // Bus number in the SBC spec sheet
    int _i2c_device_address;        // Address of the device, usually a hex like 0x40, etc
    unsigned char _i2c_read_buffer[64];
    unsigned char _i2c_write_buffer[32];
    PackedByteArray _i2c_godot_read_buffer;
    PackedByteArray _i2c_godot_write_buffer;

    static void _bind_methods();

    virtual bool _initialize_device() override;
    virtual void _deinitialize_device() override;

    virtual bool _open_i2c_device();
    virtual bool _configure_i2c_device();

public:

    I2cDevice();
    ~I2cDevice();

     // Godot virtuals.
    void _process(double delta);
    void _physics_process(double delta);

    void _notification(int p_what);

    // Getters and setters.
    void set_i2c_device_bus_number( int number );
    int  get_i2c_device_bus_number() const;

    void set_i2c_device_index( int index );
    int  get_i2c_device_index() const;

    void set_i2c_device_address( int address );
    int  get_i2c_device_address() const;

    // Device handling.
    
    
    //void open_device();
    //virtual void close_device() override;
    bool _read_bytes_from_device( const uint8_t i2c_device_address, const uint8_t i2c_device_register, const uint8_t num_bytes, uint8_t* bytes );
    bool _write_bytes_to_device( const uint8_t i2c_device_address, const uint8_t i2c_device_register, const uint8_t num_bytes_to_write, uint8_t* bytes );

    uint8_t _read_byte_from_device(  const uint8_t i2c_device_address, const uint8_t i2c_device_register );
    bool _write_byte_to_device( const uint8_t i2c_device_address, const uint8_t i2c_device_register, const uint8_t byte );

    //void _read_bytes_from_device(const int length);
    //void _write_bytes_to_device(const char* buffer, int length);

    //PackedByteArray read_bytes_from_device( const int length );
    //void write_bytes_to_device( PackedByteArray bytes );
    
    uint8_t read_byte_from_device_register( uint8_t i2c_device_register );
    void    write_byte_to_device_register( uint8_t i2c_device_register, uint8_t value );

    uint8_t read_byte_from_device_register_at_device_address( uint8_t i2c_device_address, uint8_t i2c_device_register );
    void    write_byte_to_device_register_at_device_address( uint8_t i2c_device_address, uint8_t i2c_device_register, uint8_t value );


    void  write_byte_array_to_device_register( int i2c_device_register, PackedByteArray bytes );
    void _write_byte_array_to_device_register( uint8_t i2c_device_register, const uint8_t* bytes );
};

}

#endif 