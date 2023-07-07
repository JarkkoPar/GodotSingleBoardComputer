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
    static void _bind_methods();
    virtual void _initialize_device(); // This is the method all the inheriting devices should override if needed
public:
    SBCDevice();
    ~SBCDevice();

    void initialize_device();

};


}



#endif 