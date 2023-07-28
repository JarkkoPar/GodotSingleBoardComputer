#ifndef ROBOTIKJOINT_H_INCLUDED
#define ROBOTIKJOINT_H_INCLUDED


#include <godot_cpp/classes/node3d.hpp>

namespace godot {

// This is a base device for any device used
// on the Single Board Computer.
class RobotIkJoint : public Node3D {
    GDCLASS(RobotIkJoint, Node3D)

protected: 
    static void _bind_methods();
public:
    RobotIkJoint();
    ~RobotIkJoint();

    // Enums.
    enum RobotIkJointUpdateMethod{
        PROCESS = 0,
        PHYSICS_PROCESS
    };

    // Functions

    void evaluate(Vector3 arm_tip_global_position, Vector3 target_global_position );

    // Godot virtuals. 
    void _process(double delta);
    void _physics_process(double delta);    

    // Getters and setters.

    void set_current_angle_euler( float angle );
    float get_current_angle_euler() const;

    void set_min_angle_euler( float angle );
    float get_min_angle_euler() const;

    void set_max_angle_euler( float angle );
    float get_max_angle_euler() const;

    void set_center_angle_euler( float angle );
    float get_center_angle_euler() const;

    void set_is_arm_tip( bool is_arm_tip );
    bool get_is_arm_tip() const;

    void set_is_updated_in_editor( bool is_updated_in_editor );
    bool get_is_updated_in_editor() const;

    void set_is_no_target_node_error_suppressed( bool is_no_target_node_error_suppressed );
    bool get_is_no_target_node_error_suppressed() const;


    void set_joint_axis( Vector3 axis );
    Vector3 get_joint_axis() const;

    void set_target_node_path( NodePath path );
    NodePath get_target_node_path() const;

    void set_update_method( int method );
    int  get_update_method() const;

private:
    Vector3 _joint_axis;
    float  _min_angle_euler;
    float  _max_angle_euler;
    float  _center_angle_euler;
    bool    _is_arm_tip;

    Vector3 _joint_perpendicular_axis;
    float  _min_angle_radian;
    float  _max_angle_radian;
    float  _center_angle_radian;

    float _current_angle_euler;
    float _current_angle_radian;

    bool  _is_no_target_node_error_suppressed;
    NodePath _target_node_path;
    Node3D* _target_node;

    RobotIkJointUpdateMethod _update_method;

    bool    _is_updated_in_editor;

    void update_evaluation( double delta );
};


}


#endif