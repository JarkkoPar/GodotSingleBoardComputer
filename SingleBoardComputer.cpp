#include "SingleBoardComputer.h"

#include <unistd.h>
#include <fcntl.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include "SBCDevice.h"


using namespace godot;

// Method binds.

void SingleBoardComputer::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_board_id", "new_board_id"), &SingleBoardComputer::set_board_id);
	ClassDB::bind_method(D_METHOD("get_board_id"), &SingleBoardComputer::get_board_id);

    ClassDB::bind_method(D_METHOD("_setup_board"), &SingleBoardComputer::_setup_board);
    ClassDB::bind_method(D_METHOD("_initialize_child_devices"), &SingleBoardComputer::_initialize_child_devices);



    // Add the general group.
    ADD_GROUP("Board", "board_");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "board", PROPERTY_HINT_ENUM,"Undefined:0,Radxa Rock 4,StarFive VisionFive 2"), "set_board_id","get_board_id");


}


// Constructor and destructor.

SingleBoardComputer::SingleBoardComputer() {
    _board_id = 0; // Default to 0 = Undefined

    // Simply clear all the properties to 0's and nulls.
    _num_gpio_pins = 0;
    _num_pwm_pins = 0;
    _num_i2c_buses = 0;
    _num_spi_buses = 0;

    _gpio_pins = nullptr; 
    _i2c_buses = nullptr;
    _spi_buses = nullptr;

    _opened_gpio_device_files = nullptr;
    _opened_gpio_device_file_gpio_numbers = nullptr;
    _num_opened_gpio_device_files = 0; 

    _opened_i2c_bus_device_files = nullptr;
    _opened_i2c_bus_device_file_i2c_bus_numbers = nullptr;
    _num_opened_i2c_bus_device_files = 0;

    /**
    _opened_adc_device_files = nullptr;
    _opened_adc_device_file_adc_numbers = nullptr;
    _num_opened_adc_device_files = 0;
    /**/

}


SingleBoardComputer::~SingleBoardComputer() {
    // Close any opened device files.
    if( _opened_gpio_device_files != nullptr ) {
        for( int i = 0; i < _num_opened_gpio_device_files; ++i ) {
            if( _opened_gpio_device_files[i] > -1 ) {
                close(_opened_gpio_device_files[i]);
                _opened_gpio_device_files[i] = 0;
            }
        }
        delete[] _opened_gpio_device_files;
        delete[] _opened_gpio_device_file_gpio_numbers;
        _opened_gpio_device_files = nullptr;
        _opened_gpio_device_file_gpio_numbers = nullptr;
        _num_opened_gpio_device_files = 0;
    }

    if( _opened_i2c_bus_device_files != nullptr ) {
        for( int i = 0; i < _num_opened_i2c_bus_device_files; ++i ) {
            if( _opened_i2c_bus_device_files[i] > -1 ) {
                close(_opened_i2c_bus_device_files[i]);
                _opened_i2c_bus_device_files[i] = 0;
            }
        }
        delete[] _opened_i2c_bus_device_files;
        delete[] _opened_i2c_bus_device_file_i2c_bus_numbers;
        _opened_i2c_bus_device_files = nullptr;
        _opened_i2c_bus_device_file_i2c_bus_numbers = nullptr;
        _num_opened_i2c_bus_device_files = 0;
    }

    /*
    if( _opened_adc_device_files != nullptr ) {
        for( int i = 0; i < _num_opened_adc_device_files; ++i ) {
            if( _opened_adc_device_files[i] > -1 ) {
                close(_opened_adc_device_files[i]);
                _opened_adc_device_files[i] = 0;
            }
        }
        delete[] _opened_adc_device_files;
        delete[] _opened_adc_device_file_adc_numbers;
        _opened_adc_device_files = nullptr;
        _opened_adc_device_file_adc_numbers = nullptr;
        _num_opened_adc_device_files = 0;
    }
    /**/

    // Free the config arrays.
    if( _gpio_pins != nullptr ) {
        delete[] _gpio_pins;
        _gpio_pins = nullptr;
    }
    _num_gpio_pins = 0;
    _num_pwm_pins = 0; 

    if( _i2c_buses != nullptr ) {
        delete[] _i2c_buses;
        _i2c_buses = nullptr;
    }
    _num_i2c_buses = 0;

    if( _spi_buses != nullptr ) {
        delete[] _spi_buses;
        _spi_buses = nullptr;
    }
    _num_spi_buses = 0;
}



// Getters and Setters.

void SingleBoardComputer::set_board_id( int input ) {
    _board_id = input;
}

int SingleBoardComputer::get_board_id() const {
    return _board_id;
}


// GPIO related handlers.
int SingleBoardComputer::request_gpio_device_file( int pin_index ) {
    ERR_FAIL_COND_V_MSG(pin_index < 0 || pin_index > 39, -1, "Pin index must be between 0 and 39.");
    ERR_FAIL_COND_V_MSG(_gpio_pins == nullptr, -1, "Gpio pins not yet initialized.");
    

    // Check if the gpio file related to the pin is already open.
    int gpio_index = _gpio_pins[pin_index].get_gpio_device_file_index();
    for( int i = 0; i < _num_opened_gpio_device_files; ++i ) {
        if( _opened_gpio_device_file_gpio_numbers[i] == gpio_index ) {
            return _opened_gpio_device_files[i];
        }
    }
    // The file wasn't already open so add a new file to the list.
    int* new_gpio_files_array = new int[_num_opened_gpio_device_files + 1];
    ERR_FAIL_COND_V_MSG(new_gpio_files_array == nullptr, -1, "Could not create a new gpio file descriptor array.");
    int* new_gpio_numbers_array = new int[_num_opened_gpio_device_files + 1];
    if( new_gpio_numbers_array == nullptr ) {
        delete[] new_gpio_files_array;
        ERR_FAIL_COND_V_MSG(new_gpio_numbers_array == nullptr, -1, "Could not create a new gpio file descriptor numbers array.");    
    }

    if( _opened_gpio_device_files != nullptr ) {
        memcpy( new_gpio_files_array, _opened_gpio_device_files, sizeof(_opened_gpio_device_files) );
        memcpy( new_gpio_numbers_array, _opened_gpio_device_file_gpio_numbers, sizeof(_opened_gpio_device_file_gpio_numbers) );

        delete[] _opened_gpio_device_files;
        delete[] _opened_gpio_device_file_gpio_numbers;
    }
    _opened_gpio_device_files = new_gpio_files_array;
    _opened_gpio_device_file_gpio_numbers = new_gpio_numbers_array;
    
    // Open the file and return the result back.
    char filename[32] = {0};
    sprintf(filename, "/dev/gpiochip%i", gpio_index);
    _opened_gpio_device_files[_num_opened_gpio_device_files] = open(filename, O_RDONLY);
    _opened_gpio_device_file_gpio_numbers[_num_opened_gpio_device_files] = gpio_index;
    _num_opened_gpio_device_files += 1; 

    return _opened_gpio_device_files[_num_opened_gpio_device_files-1];
}


int SingleBoardComputer::get_num_gpio_pins() const {
    return _num_gpio_pins;
}

GpioPin* SingleBoardComputer::get_gpio_pins() const {
    return _gpio_pins;
}


int SingleBoardComputer::get_gpio_pin_offset(int pin_index) const {
    ERR_FAIL_COND_V_MSG(pin_index < 0 || pin_index > get_num_gpio_pins(), -1, "Gpio pin index must be between 0 and max count.");
    return _gpio_pins[pin_index].get_pin_offset();
}


// ADC related handlers.
/*
int SingleBoardComputer::request_adc_device_file( int pin_index ) {
    ERR_FAIL_COND_V_MSG(pin_index < 0 || pin_index > 39, -1, "Pin index must be between 0 and 39.");
    ERR_FAIL_COND_V_MSG(_gpio_pins == nullptr, -1, "Gpio pins not yet initialized.");
    ERR_FAIL_COND_V_MSG(!(_gpio_pins[pin_index].get_primary_function() == GpioPin::GpioPinFunction::GPF_ADC_IN || _gpio_pins[pin_index].get_secondary_function() != GpioPin::GpioPinFunction::GPF_ADC_IN), -1, "Gpio pins does not have an ADC function.");
    

    // Check if the adc file related to the pin is already open.
    int adc_index = _gpio_pins[pin_index].get_adc_device_file_index();
    for( int i = 0; i < _num_opened_adc_device_files; ++i ) {
        if( _opened_adc_device_file_adc_numbers[i] == adc_index ) {
            return _opened_adc_device_files[i];
        }
    }
    
    // The file wasn't already open so add a new file to the list.
    int* new_adc_files_array = new int[_num_opened_adc_device_files + 1];
    ERR_FAIL_COND_V_MSG(new_adc_files_array == nullptr, -1, "Could not create a new adc file descriptor array.");
    int* new_adc_numbers_array = new int[_num_opened_adc_device_files + 1];
    if( new_adc_numbers_array == nullptr ) {
        delete[] new_adc_files_array;
        ERR_FAIL_COND_V_MSG(new_adc_numbers_array == nullptr, -1, "Could not create a new adc file descriptor numbers array.");    
    }

    if( _opened_adc_device_files != nullptr ) {
        memcpy( new_adc_files_array, _opened_adc_device_files, sizeof(_opened_adc_device_files) );
        memcpy( new_adc_numbers_array, _opened_adc_device_file_adc_numbers, sizeof(_opened_adc_device_file_adc_numbers) );

        delete[] _opened_adc_device_files;
        delete[] _opened_adc_device_file_adc_numbers;
    }
    _opened_adc_device_files = new_adc_files_array;
    _opened_adc_device_file_adc_numbers = new_adc_numbers_array;
    
    // Open the file and return the result back.
    char filename[64] = {0};
    sprintf(filename, "/sys/bus/iio/devices/iio:device0/in_voltage%i_raw", adc_index);
    _opened_adc_device_files[_num_opened_adc_device_files] = open(filename, O_RDONLY);
    _opened_adc_device_file_adc_numbers[_num_opened_adc_device_files] = adc_index;
    _num_opened_adc_device_files += 1; 

    return _opened_adc_device_files[_num_opened_adc_device_files-1];
}
/**/


// I2C related handlers.
int SingleBoardComputer::get_num_i2c_buses() const {
    return _num_i2c_buses;
}


I2CBus* SingleBoardComputer::get_i2c_bus( int bus_index ) const {
    if( bus_index < 0 || bus_index >= _num_i2c_buses ) {
        return nullptr;
    }
    return &_i2c_buses[bus_index];
}    


int SingleBoardComputer::request_i2c_device_file( int bus_index ) {
    ERR_FAIL_COND_V_MSG(bus_index < 0 || bus_index > get_num_i2c_buses(), -1, "I2C bus index must be between 0 and max count.");

    // Check if the i2c file related to the bus is already open.
    int i2c_index = _i2c_buses[bus_index].get_i2c_device_file_index();
    for( int i = 0; i < _num_opened_i2c_bus_device_files; ++i ) {
        if( _opened_i2c_bus_device_file_i2c_bus_numbers[i] == i2c_index ) {
            return _opened_i2c_bus_device_files[i];
        }
    }
    // The file wasn't already open so add a new file to the list.
    int* new_i2c_files_array = new int[_num_opened_i2c_bus_device_files + 1];
    ERR_FAIL_COND_V_MSG(new_i2c_files_array == nullptr, -1, "Could not create a new I2C file descriptor array.");
    int* new_i2c_numbers_array = new int[_num_opened_i2c_bus_device_files + 1];
    if( new_i2c_numbers_array == nullptr ) {
        delete[] new_i2c_files_array;
        ERR_FAIL_COND_V_MSG(new_i2c_numbers_array == nullptr, -1, "Could not create a new I2C file descriptor numbers array.");    
    }

    if( _opened_i2c_bus_device_files != nullptr ) {
        memcpy( new_i2c_files_array, _opened_i2c_bus_device_files, sizeof(_opened_i2c_bus_device_files) );
        memcpy( new_i2c_numbers_array, _opened_i2c_bus_device_file_i2c_bus_numbers, sizeof(_opened_i2c_bus_device_file_i2c_bus_numbers) );

        delete[] _opened_i2c_bus_device_files;
        delete[] _opened_i2c_bus_device_file_i2c_bus_numbers;
    }
    _opened_i2c_bus_device_files = new_i2c_files_array;
    _opened_i2c_bus_device_file_i2c_bus_numbers = new_i2c_numbers_array;
    
    // Open the file and return the result back.
    char filename[32] = {0};
    sprintf(filename, "/dev/i2c-%i", i2c_index);
    _opened_i2c_bus_device_files[_num_opened_i2c_bus_device_files] = open(filename, O_RDWR); //O_RDONLY);
    _opened_i2c_bus_device_file_i2c_bus_numbers[_num_opened_i2c_bus_device_files] = i2c_index;
    _num_opened_i2c_bus_device_files += 1; 

    return _opened_i2c_bus_device_files[_num_opened_i2c_bus_device_files-1];
}
    



// Godot virtuals.

void SingleBoardComputer::_notification(int p_what) {
	switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
            // Entered the tree. 
        } break;
		case NOTIFICATION_EXIT_TREE: {
			//_clear_monitoring();
		} break;
	}
}

void SingleBoardComputer::_ready() {
    _setup_board();
    if(Engine::get_singleton()->is_editor_hint()) return; // Don't initialize the childs in the editor.

    // Initialize the childs.
    _initialize_child_devices();
}

void SingleBoardComputer::_process(double delta ) {

}

void SingleBoardComputer::_physics_process(double delta ) {
    
}

/**
PackedStringArray SingleBoardComputer::get_configuration_warnings() const {
	PackedStringArray warnings = Node::get_configuration_warnings();

	if (get_child_count() < 1) {
		warnings.push_back(RTR("This node has no devices as childs.\nConsider adding some devices to enable some functionality."));
	}

	return warnings;
}
/**/


// Board setup

void SingleBoardComputer::_setup_board(){
    // Fail if the board_id is 0 - undefined
    ERR_FAIL_COND_MSG(_board_id == 0, "The board ID has not been set.");

    // Create based on the board id.
    switch(_board_id) {
        // Radxa Rock 4
        case 1: {
            // Set the counts and create the arrays.
            _num_gpio_pins = 40;
            _num_i2c_buses = 2;
            _num_spi_buses = 2;
            _num_pwm_pins = 2; 
            _num_adc_pins = 1;

            _gpio_pins = new GpioPin[_num_gpio_pins];
            _i2c_buses = new I2CBus[_num_i2c_buses];
            _spi_buses = new SPIBus[_num_spi_buses];
            
            // Set the functions etc.
            _gpio_pins[ 0].set_primary_function(GpioPin::GpioPinFunction::GPF_3_3V);       _gpio_pins[ 1].set_primary_function(GpioPin::GpioPinFunction::GPF_5_0V);
            _gpio_pins[ 2].set_primary_function(GpioPin::GpioPinFunction::GPF_I2C_SDA);    _gpio_pins[ 3].set_primary_function(GpioPin::GpioPinFunction::GPF_5_0V);
            _gpio_pins[ 4].set_primary_function(GpioPin::GpioPinFunction::GPF_I2C_SCL);    _gpio_pins[ 5].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);
            _gpio_pins[ 6].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_CLK);    _gpio_pins[ 7].set_primary_function(GpioPin::GpioPinFunction::GPF_UART_TXD);
            _gpio_pins[ 8].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);        _gpio_pins[ 9].set_primary_function(GpioPin::GpioPinFunction::GPF_UART_RXD);
            _gpio_pins[10].set_primary_function(GpioPin::GpioPinFunction::GPF_PWM);        _gpio_pins[11].set_primary_function(GpioPin::GpioPinFunction::GPF_I2S_SCLK);
            _gpio_pins[12].set_primary_function(GpioPin::GpioPinFunction::GPF_PWM);        _gpio_pins[13].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);
            _gpio_pins[14].set_primary_function(GpioPin::GpioPinFunction::GPF_SPDIF_TX);   _gpio_pins[15].set_primary_function(GpioPin::GpioPinFunction::GPF_PIN);
            _gpio_pins[16].set_primary_function(GpioPin::GpioPinFunction::GPF_3_3V);       _gpio_pins[17].set_primary_function(GpioPin::GpioPinFunction::GPF_PIN);
            _gpio_pins[18].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_TXD);    _gpio_pins[19].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);
            _gpio_pins[20].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_RXD);    _gpio_pins[21].set_primary_function(GpioPin::GpioPinFunction::GPF_PIN);
            _gpio_pins[22].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_CLK);    _gpio_pins[23].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_CSn);
            _gpio_pins[24].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);        _gpio_pins[25].set_primary_function(GpioPin::GpioPinFunction::GPF_ADC_IN);
            _gpio_pins[26].set_primary_function(GpioPin::GpioPinFunction::GPF_I2C_SDA);    _gpio_pins[27].set_primary_function(GpioPin::GpioPinFunction::GPF_I2C_CLK);
            _gpio_pins[28].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_TXD);    _gpio_pins[29].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);
            _gpio_pins[30].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_RXD);    _gpio_pins[31].set_primary_function(GpioPin::GpioPinFunction::GPF_SPDIF_TX);
            _gpio_pins[32].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_CSn);    _gpio_pins[33].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);
            _gpio_pins[34].set_primary_function(GpioPin::GpioPinFunction::GPF_I2S_LRCK_TX);_gpio_pins[35].set_primary_function(GpioPin::GpioPinFunction::GPF_I2S_LRCK_RX);
            _gpio_pins[36].set_primary_function(GpioPin::GpioPinFunction::GPF_PIN);        _gpio_pins[37].set_primary_function(GpioPin::GpioPinFunction::GPF_I2S_SDI);
            _gpio_pins[38].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);        _gpio_pins[39].set_primary_function(GpioPin::GpioPinFunction::GPF_I2S_SDO);
            
            // Todo: set secondary functions.

            // Set the device file indices for the gpio pins.
            _gpio_pins[ 2].set_gpio_device_file_index(2);
            _gpio_pins[ 4].set_gpio_device_file_index(2);
            _gpio_pins[ 6].set_gpio_device_file_index(2); _gpio_pins[ 7].set_gpio_device_file_index(4);
                                                          _gpio_pins[ 9].set_gpio_device_file_index(4);
            _gpio_pins[10].set_gpio_device_file_index(4); _gpio_pins[11].set_gpio_device_file_index(4);
            _gpio_pins[12].set_gpio_device_file_index(4);
            _gpio_pins[14].set_gpio_device_file_index(4); _gpio_pins[15].set_gpio_device_file_index(4);
                                                          _gpio_pins[17].set_gpio_device_file_index(4);
            _gpio_pins[18].set_gpio_device_file_index(1); 
            _gpio_pins[20].set_gpio_device_file_index(1); _gpio_pins[21].set_gpio_device_file_index(4);
            _gpio_pins[22].set_gpio_device_file_index(1); _gpio_pins[23].set_gpio_device_file_index(1);
                                                         // _gpio_pins[25].set_gpio_device_file_index(-1); // ADC_IN0
            _gpio_pins[26].set_gpio_device_file_index(2); _gpio_pins[27].set_gpio_device_file_index(2);
            _gpio_pins[28].set_gpio_device_file_index(2);
            _gpio_pins[30].set_gpio_device_file_index(2); _gpio_pins[31].set_gpio_device_file_index(3);
            _gpio_pins[32].set_gpio_device_file_index(2);
            _gpio_pins[34].set_gpio_device_file_index(4); _gpio_pins[35].set_gpio_device_file_index(4);
            _gpio_pins[36].set_gpio_device_file_index(4); _gpio_pins[37].set_gpio_device_file_index(4);
                                                          _gpio_pins[39].set_gpio_device_file_index(4);


            // Set the offsets for the pins.
            // Radxa uses the formula GPIOx_[A-D]y in their pins, for instance GPIO4_D5
            // is 32 * 4 + 8 * 3 + 5. You can see the values for a-d below.
            // As the file is set above, we only need the letter and the last number.
            int a = 0, b = 1, c = 2, d = 3;
            a *= 8; b *= 8; c *= 8; d *= 8;

            _gpio_pins[ 2].set_pin_offset(a+7);
            _gpio_pins[ 4].set_pin_offset(b+0);
            _gpio_pins[ 6].set_pin_offset(b+3); _gpio_pins[ 7].set_pin_offset(c+4);
                                                _gpio_pins[ 9].set_pin_offset(c+3);
            _gpio_pins[10].set_pin_offset(c+2); _gpio_pins[11].set_pin_offset(a+3);
            _gpio_pins[12].set_pin_offset(c+6);
            _gpio_pins[14].set_pin_offset(c+5); _gpio_pins[15].set_pin_offset(d+2);
                                                _gpio_pins[17].set_pin_offset(d+4);
            _gpio_pins[18].set_pin_offset(b+0); 
            _gpio_pins[20].set_pin_offset(a+7); _gpio_pins[21].set_pin_offset(d+5); 
            _gpio_pins[22].set_pin_offset(b+1); _gpio_pins[23].set_pin_offset(b+2);
                                                _gpio_pins[25].set_pin_offset( -1); // ADC_IN0
            _gpio_pins[26].set_pin_offset(a+0); _gpio_pins[27].set_pin_offset(a+1);
            _gpio_pins[28].set_pin_offset(b+2);
            _gpio_pins[30].set_pin_offset(b+1); _gpio_pins[31].set_pin_offset(c+0);
            _gpio_pins[32].set_pin_offset(b+4);
            _gpio_pins[34].set_pin_offset(a+5); _gpio_pins[35].set_pin_offset(a+4);
            _gpio_pins[36].set_pin_offset(d+6); _gpio_pins[37].set_pin_offset(a+6);
                                                _gpio_pins[39].set_pin_offset(a+7);
            

            /* rock 5b*
            _gpio_pins[ 2].set_pin_offset(b+3);
            _gpio_pins[ 4].set_pin_offset(b+2);
            _gpio_pins[ 6].set_pin_offset(c+3); _gpio_pins[ 7].set_pin_offset(b+5);
                                                _gpio_pins[ 9].set_pin_offset(b+6);
            _gpio_pins[10].set_pin_offset(c+1); _gpio_pins[11].set_pin_offset(b+5);
            _gpio_pins[12].set_pin_offset(b+7);
            _gpio_pins[14].set_pin_offset(c+0); _gpio_pins[15].set_pin_offset(a+4);
                                                _gpio_pins[17].set_pin_offset(c+4);
            _gpio_pins[18].set_pin_offset(b+2); 
            _gpio_pins[20].set_pin_offset(b+1); _gpio_pins[21].set_pin_offset( -1); //SARADC_IN4
            _gpio_pins[22].set_pin_offset(b+3); _gpio_pins[23].set_pin_offset(b+4);
                                                _gpio_pins[25].set_pin_offset(b+5); // ADC_IN0
            _gpio_pins[26].set_pin_offset(c+6); _gpio_pins[27].set_pin_offset(c+5);
            _gpio_pins[28].set_pin_offset(d+7);
            _gpio_pins[30].set_pin_offset(b+7); _gpio_pins[31].set_pin_offset(c+2);
            _gpio_pins[32].set_pin_offset(a+7);
            _gpio_pins[34].set_pin_offset(b+6); _gpio_pins[35].set_pin_offset(b+1);
            _gpio_pins[36].set_pin_offset( -1); _gpio_pins[37].set_pin_offset(4);
                                                          _gpio_pins[39].set_pin_offset(4);
            /**/

            // Set the ADC pins.
            _gpio_pins[25].set_adc_device_file_index(0);
            _gpio_pins[25].set_adc_voltage_file_index(0);
            _gpio_pins[25].set_adc_max_voltage(1.8f);

            // Set the I2C buses.
            _i2c_buses[0].set_i2c_device_file_index(2);
            _i2c_buses[1].set_i2c_device_file_index(7);

            // Set the pins that the i2c devices will reserve.
            _i2c_buses[0].add_gpio_reserved_pin(26);
            _i2c_buses[0].add_gpio_reserved_pin(27);

            _i2c_buses[1].add_gpio_reserved_pin(2);
            _i2c_buses[1].add_gpio_reserved_pin(4);


            // And the SPI buses.
            // todo: code when I have an SPI device
        } break;
    }
}



void SingleBoardComputer::_initialize_child_devices() {
    
    for( int i = 0; i < get_child_count(); ++i ) {
        // If the child is of base initializable sbc device, then initialize 
        // the device.
        SBCDevice* child_device = static_cast<SBCDevice*>(get_child(i));
        if( child_device == nullptr ) continue;
        child_device->initialize_device();
    }
}