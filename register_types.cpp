#include "register_types.h"

// Classes to include in to the gdextension module.
#include "SingleBoardComputer.h"
#include "SBCDevice.h"

// I2C devices.
#include "I2CRawDevice.h"
#include "I2CPCA9685.h"

// Gpio devices.
#include "GpioRawDevice.h"
#include "GpioLED.h"

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
    ClassDB::register_class<SBCDevice>();

    // I2C devices.
    ClassDB::register_class<I2CRawDevice>();
    ClassDB::register_class<I2CPCA9685>();

    // Gpio devices.
    ClassDB::register_class<GpioRawDevice>();
    ClassDB::register_class<GpioLED>();
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
