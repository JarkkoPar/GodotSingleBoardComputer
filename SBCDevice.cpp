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

    ClassDB::bind_method(D_METHOD("set_update_method"), &SBCDevice::set_update_method);
    ClassDB::bind_method(D_METHOD("get_update_method"), &SBCDevice::get_update_method);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "update_method", PROPERTY_HINT_ENUM, "Process:0,Physics process:1"), "set_update_method", "get_update_method");

}


void SBCDevice::initialize_device() {
    if( !_is_active ) return;
    if( _is_device_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;
    
    _is_device_initialized = _initialize_device();
}

void SBCDevice::deinitialize_device() {
    _deinitialize_device();
    _is_device_initialized = false;
}



bool SBCDevice::_initialize_device() {
    return false;
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


void SBCDevice::set_update_method( int method ) {
    if( method == 0 ) {
        _update_method = SBCDevice::PROCESS;
        return;
    }
    _update_method = SBCDevice::PHYSICS_PROCESS;
}

int  SBCDevice::get_update_method() const {
    if( _update_method == SBCDevice::PROCESS) {
        return 0;
    }
    return 1;
}


