#include "SBCDevice.h"

using namespace godot;

SBCDevice::SBCDevice() {

}


SBCDevice::~SBCDevice() {
    close_device();
}


void SBCDevice::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initialize_device"), &SBCDevice::initialize_device);
}


void SBCDevice::initialize_device() {
    _initialize_device();
}


void SBCDevice::_initialize_device() {
    
}

void SBCDevice::close_device() {
    
}