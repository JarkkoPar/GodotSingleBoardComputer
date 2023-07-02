#ifndef I2C_BUS_H_INCLUDED
#define I2C_BUS_H_INCLUDED 


class I2CBus {
public:

    I2CBus();
    ~I2CBus();

    void set_i2c_device_file_index( int new_index );
    int  get_i2c_device_file_index() const;

private:
    int _i2c_device_file_index; 
};


#endif 