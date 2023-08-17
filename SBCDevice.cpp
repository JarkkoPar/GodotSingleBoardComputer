#include "SBCDevice.h"

#include <godot_cpp/classes/engine.hpp>


using namespace godot;

SBCDevice::SBCDevice() {
    _is_active = true;
    _is_device_initialized = false;
}


SBCDevice::~SBCDevice() {
    deinitialize_device();
}


void SBCDevice::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initialize_device"), &SBCDevice::initialize_device);
    ClassDB::bind_method(D_METHOD("deinitialize_device"), &SBCDevice::deinitialize_device);

    ClassDB::bind_method(D_METHOD("set_is_active", "is_active"), &SBCDevice::set_is_active);
	ClassDB::bind_method(D_METHOD("get_is_active"), &SBCDevice::get_is_active);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_active", PROPERTY_HINT_NONE ), "set_is_active", "get_is_active");

}


void SBCDevice::initialize_device() {
    if( _is_device_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    if( !_is_active ) return;

    _initialize_device();
    _is_device_initialized = true;
}

void SBCDevice::deinitialize_device() {
    _deinitialize_device();
    _is_device_initialized = false;
}



void SBCDevice::_initialize_device() {
    
}

void SBCDevice::_deinitialize_device() {
    
}

// Getters and setters.

void SBCDevice::set_is_active( bool is_active ) {
    _is_active = is_active;
}

bool SBCDevice::get_is_active() const {
    return _is_active;
}

