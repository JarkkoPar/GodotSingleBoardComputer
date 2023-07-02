#ifndef SPI_BUS_H_INCLUDED 
#define SPI_BUS_H_INCLUDED 


class SPIBus{
public:
    SPIBus();
    ~SPIBus();

    void set_spi_device_file_index( int new_index );
    int  get_spi_device_file_index() const;

private:
    int _spi_device_file_index;
};


#endif 