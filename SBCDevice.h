#ifndef SBC_DEVICE_H_INCLUDED
#define SBC_DEVICE_H_INCLUDED

#include <godot_cpp/classes/node.hpp>

namespace godot {

// This is a base device for any device used
// on the Single Board Computer.
class SBCDevice : public Node {
    GDCLASS(SBCDevice, Node)

private:
    
protected: 
    bool _is_active;
    bool _is_device_initialized;


    static void _bind_methods();
    virtual void _initialize_device(); // This is the method all the inheriting devices should override if needed
    
    
public:
    SBCDevice();
    ~SBCDevice();

    void initialize_device();

    virtual void close_device();

    // Getters and setters.

    void set_is_active( bool is_active );
    bool get_is_active() const;

};


}



#endif 