#ifndef SBC_DEVICE_H_INCLUDED
#define SBC_DEVICE_H_INCLUDED

#include <godot_cpp/classes/node.hpp>

namespace godot {

// This is a base device for any device used
// on the Single Board Computer.
class SBCDevice : public Node {
    GDCLASS(SBCDevice, Node)

public:

    // Enums.

    enum SBCDeviceUpdateMethod{
        PROCESS = 0,
        PHYSICS_PROCESS
    };

    SBCDevice();
    ~SBCDevice();

    void initialize_device();
    void deinitialize_device(); 

    // Getters and setters.

    void set_is_active( bool is_active );
    bool get_is_active() const;

    void set_update_method( int method );
    int  get_update_method() const;

    
protected: 
    bool _is_active;
    bool _is_device_initialized;

    SBCDeviceUpdateMethod _update_method;


    static void _bind_methods();
    virtual bool _initialize_device(); // This is the method all the inheriting devices should override if needed
    virtual void _deinitialize_device(); // This is the method all the inheriting devices should override if needed

private:


};


}



#endif 