#include "RobotIkJoint.h"

using namespace godot;



void RobotIkJoint::_bind_methods() {

}


RobotIkJoint::RobotIkJoint() {
    _min_angle_euler = 0.0;
    _max_angle_euler = 180.0;
    _center_angle_euler = 90.0;
    _min_angle_radian = Math::deg_to_rad(_min_angle_euler);
    _max_angle_radian = Math::deg_to_rad(_max_angle_euler);
    _center_angle_radian = Math::deg_to_rad(_center_angle_euler);

    _joint_axis = Vector3( 1.0, 0.0, 0.0 );
    _joint_perpendicular_axis = Vector3(0.0, 1.0, 0.0 );

    _current_angle_euler = _center_angle_euler;
    _current_angle_radian = _center_angle_radian;
}


RobotIkJoint::~RobotIkJoint() {
    
}



void RobotIkJoint::evaluate(Vector3 arm_tip_global_position, Vector3 target_global_position ) {
    // Skip the evaluation if this is a the arm tip.
    if( !_is_arm_tip ) {
        // Point towards the target.
        Vector3 arm_tip_local = to_local(arm_tip_global_position);
        Vector3 target_local = to_local(target_global_position);

        Vector3 direction_to_arm_tip_local = (arm_tip_local - get_transform().origin).normalized();
        Vector3 direction_target_local = (target_local - get_transform().origin).normalized();

        Quaternion current_rotation = get_transform().basis.get_rotation_quaternion();
        Quaternion point_to_target_rotation = Quaternion(direction_to_arm_tip_local, direction_to_arm_tip_local) * current_rotation;
        
        // Constraint to hinge axis.
        Vector3 hinge_axis_local = point_to_target_rotation.xform(_joint_axis);
        Quaternion hinge_rotation_back = Quaternion( hinge_axis_local, _joint_axis );

        Quaternion hinge_corrected_rotation = hinge_rotation_back * point_to_target_rotation;

        // Constraint to the min & max angles.
        Vector3 rotation_axis = hinge_corrected_rotation.get_axis();
        double  rotation_angle_radian = hinge_corrected_rotation.get_angle();

        double constraint_corrected_angle_radian = Math::clamp( rotation_angle_radian + _center_angle_radian, _min_angle_radian, _max_angle_radian ) - _center_angle_radian;

        _current_angle_radian = constraint_corrected_angle_radian + _center_angle_radian;
        _current_angle_euler = Math::rad_to_deg( _current_angle_radian );

        Quaternion constrained_rotation = Quaternion( rotation_axis, constraint_corrected_angle_radian );

        // Set the basis.
        get_transform().basis = Transform3D(constrained_rotation).basis;
    }

    // Check the parent.
    RobotIkJoint* parent_joint = static_cast<RobotIkJoint*>(get_parent());
    if( parent_joint == nullptr ) {
        return;
    }
    parent_joint->evaluate( arm_tip_global_position, target_global_position );

}


// Getters and setters.


void RobotIkJoint::set_current_angle_euler( double angle ) {
    _current_angle_euler = angle;
    _current_angle_radian = Math::deg_to_rad(_current_angle_euler);

}

double RobotIkJoint::get_current_angle_euler() const {
    return _current_angle_euler;
}


void RobotIkJoint::set_min_angle_euler( double angle ) {
    _min_angle_euler = angle;
    _min_angle_radian = Math::deg_to_rad(_min_angle_euler);

}

double RobotIkJoint::get_min_angle_euler() const {
    return _min_angle_euler;
}

void RobotIkJoint::set_max_angle_euler( double angle ) {
    _max_angle_euler = angle;
    _max_angle_radian = Math::deg_to_rad(_max_angle_euler);
}

double RobotIkJoint::get_max_angle_euler() const {
    return _max_angle_euler;
}

void RobotIkJoint::set_center_angle_euler( double angle ) {
    _center_angle_euler = angle;
    _center_angle_radian = Math::deg_to_rad(_center_angle_euler);
}

double RobotIkJoint::get_center_angle_euler() const {
    return _center_angle_euler;
}

void RobotIkJoint::set_is_arm_tip( bool is_arm_tip ) {
    _is_arm_tip = is_arm_tip;
}

bool RobotIkJoint::get_is_arm_tip() const {
    return _is_arm_tip;
}

void RobotIkJoint::set_joint_axis( Vector3 axis ) {
    _joint_axis = axis;
    if( _joint_axis != Vector3(0.0, 1.0, 0.0 )) {
        _joint_perpendicular_axis = _joint_axis.cross(Vector3(0.0,1.0,0.0));
    } else {
        _joint_perpendicular_axis = _joint_axis.cross(Vector3(0.0,0.0,-1.0));
    }
}

Vector3 RobotIkJoint::get_joint_axis() const {
    return _joint_axis;
}
