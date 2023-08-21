#include "register_types.h"

// Classes to include in to the gdextension module.
#include "SingleBoardComputer.h"
#include "SBCDevice.h"

// I2C devices.
#include "I2cDevice.h"
#include "I2cAds1115.h"
#include "I2cAk8963.h"
#include "I2cBmp280.h"
#include "I2cMpu9250.h"
#include "I2cPca9685.h"
#include "I2cVl53l0x.h"

// Gpio devices.
#include "GpioDevice.h"
#include "GpioLED.h"
#include "GpioHcSr04.h"

// ADC devices.
#include "AdcDevice.h"
#include "AdcTemperatureSensor.h"

// Helpers and others.
#include "RobotIkJoint.h"

// Standard headers.
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_singleboardcomputer_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    // Add the classes here.
    ClassDB::register_class<SingleBoardComputer>();
    ClassDB::register_class<SBCDevice>(true);

    // I2C devices.
    ClassDB::register_class<I2cDevice>(true);
    ClassDB::register_class<I2cAds1115>();
    ClassDB::register_class<I2cAk8963>();
    ClassDB::register_class<I2cBmp280>();
    ClassDB::register_class<I2cMpu9250>();
    ClassDB::register_class<I2cPca9685>();
    ClassDB::register_class<I2cVl53l0x>();

    // Gpio devices.
    ClassDB::register_class<GpioDevice>(true);
    ClassDB::register_class<GpioLED>();
    ClassDB::register_class<GpioHcSr04>();


    // ADC devices.
    ClassDB::register_class<AdcDevice>(true);
    ClassDB::register_class<AdcTemperatureSensor>();

    // Helpers and others.
    ClassDB::register_class<RobotIkJoint>();
}

void uninitialize_singleboardcomputer_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT singleboardcomputer_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_singleboardcomputer_module);
    init_obj.register_terminator(uninitialize_singleboardcomputer_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}
