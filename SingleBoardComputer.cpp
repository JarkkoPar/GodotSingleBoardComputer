#include "SingleBoardComputer.h"



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
}


SingleBoardComputer::~SingleBoardComputer() {
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
            _gpio_pins[24].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);        _gpio_pins[25].set_primary_function(GpioPin::GpioPinFunction::GPF_ADC_IN0);
            _gpio_pins[26].set_primary_function(GpioPin::GpioPinFunction::GPF_I2C_SDA);    _gpio_pins[27].set_primary_function(GpioPin::GpioPinFunction::GPF_I2C_CLK);
            _gpio_pins[28].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_TXD);    _gpio_pins[29].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);
            _gpio_pins[30].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_RXD);    _gpio_pins[31].set_primary_function(GpioPin::GpioPinFunction::GPF_SPDIF_TX);
            _gpio_pins[32].set_primary_function(GpioPin::GpioPinFunction::GPF_SPI_CSn);    _gpio_pins[33].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);
            _gpio_pins[34].set_primary_function(GpioPin::GpioPinFunction::GPF_I2S_LRCK_TX);_gpio_pins[35].set_primary_function(GpioPin::GpioPinFunction::GPF_I2S_LRCK_RX);
            _gpio_pins[36].set_primary_function(GpioPin::GpioPinFunction::GPF_PIN);        _gpio_pins[37].set_primary_function(GpioPin::GpioPinFunction::GPF_I2S_SDI);
            _gpio_pins[38].set_primary_function(GpioPin::GpioPinFunction::GPF_GND);        _gpio_pins[39].set_primary_function(GpioPin::GpioPinFunction::GPF_I2S_SDO);
            
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


            // Set the I2C buses.
            // https://github.com/besp9510/pi_i2c
            _i2c_buses[0].set_i2c_device_file_index(2);
            _i2c_buses[1].set_i2c_device_file_index(7);
            
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