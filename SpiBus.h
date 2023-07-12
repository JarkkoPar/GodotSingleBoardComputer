#ifndef SPI_BUS_H_INCLUDED 
#define SPI_BUS_H_INCLUDED 


class SpiBus{
public:
    SpiBus();
    ~SpiBus();

    void set_spi_device_file_index( int new_index );
    int  get_spi_device_file_index() const;

private:
    int _spi_device_file_index;
};


#endif 