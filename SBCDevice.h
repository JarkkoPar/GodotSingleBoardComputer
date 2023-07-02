#ifndef SBC_DEVICE_H_INCLUDED
#define SBC_DEVICE_H_INCLUDED

#include <godot_cpp/classes/node.hpp>

namespace godot {

// This is a "raw" i2c device node, which allows a more low-level
// communication interface with any i2c device.
class SBCDevice : public Node {
    GDCLASS(SBCDevice, Node)

private:
    
protected: 
    static void _bind_methods();
    virtual void _initialize_device();
public:
    SBCDevice();
    ~SBCDevice();

    void initialize_device();

};


}



#endif 