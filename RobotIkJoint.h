#ifndef ROBOTIKJOINT_H_INCLUDED
#define ROBOTIKJOINT_H_INCLUDED


#include <godot_cpp/classes/node3d.hpp>

namespace godot {

// This is a base device for any device used
// on the Single Board Computer.
class RobotIkJoint : public Node3D {
    GDCLASS(RobotIkJoint, Node3D)

private:
    Vector3 _joint_axis;
    double  _min_angle_euler;
    double  _max_angle_euler;
    double  _center_angle_euler;
    bool    _is_arm_tip;

    Vector3 _joint_perpendicular_axis;
    double  _min_angle_radian;
    double  _max_angle_radian;
    double  _center_angle_radian;

    double _current_angle_euler;
    double _current_angle_radian;
protected: 
    static void _bind_methods();
public:
    RobotIkJoint();
    ~RobotIkJoint();

    void evaluate(Vector3 arm_tip_global_position, Vector3 target_global_position );

    // Getters and setters.

    void set_current_angle_euler( double angle );
    double get_current_angle_euler() const;

    void set_min_angle_euler( double angle );
    double get_min_angle_euler() const;

    void set_max_angle_euler( double angle );
    double get_max_angle_euler() const;

    void set_center_angle_euler( double angle );
    double get_center_angle_euler() const;

    void set_is_arm_tip( bool is_arm_tip );
    bool get_is_arm_tip() const;

    void set_joint_axis( Vector3 axis );
    Vector3 get_joint_axis() const;

};


}


#endif