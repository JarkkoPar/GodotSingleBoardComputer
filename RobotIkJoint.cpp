#include "RobotIkJoint.h"
#include <godot_cpp/classes/engine.hpp>

using namespace godot;



void RobotIkJoint::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_joint_axis"), &RobotIkJoint::set_joint_axis);
    ClassDB::bind_method(D_METHOD("get_joint_axis"), &RobotIkJoint::get_joint_axis);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "joint_axis", PROPERTY_HINT_NONE), "set_joint_axis", "get_joint_axis");

    ClassDB::bind_method(D_METHOD("set_is_arm_tip"), &RobotIkJoint::set_is_arm_tip);
    ClassDB::bind_method(D_METHOD("get_is_arm_tip"), &RobotIkJoint::get_is_arm_tip);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_arm_tip", PROPERTY_HINT_NONE), "set_is_arm_tip", "get_is_arm_tip");

    ClassDB::bind_method(D_METHOD("set_target_node_path"), &RobotIkJoint::set_target_node_path);
    ClassDB::bind_method(D_METHOD("get_target_node_path"), &RobotIkJoint::get_target_node_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "target_node_path", PROPERTY_HINT_NONE), "set_target_node_path", "get_target_node_path");


    ClassDB::bind_method(D_METHOD("set_center_angle_euler"), &RobotIkJoint::set_center_angle_euler);
    ClassDB::bind_method(D_METHOD("get_center_angle_euler"), &RobotIkJoint::get_center_angle_euler);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "center_angle_Euler", PROPERTY_HINT_RANGE, "-360,360"), "set_center_angle_euler", "get_center_angle_euler");

    ClassDB::bind_method(D_METHOD("set_min_angle_euler"), &RobotIkJoint::set_min_angle_euler);
    ClassDB::bind_method(D_METHOD("get_min_angle_euler"), &RobotIkJoint::get_min_angle_euler);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_angle_euler", PROPERTY_HINT_RANGE, "-360,360"), "set_min_angle_euler", "get_min_angle_euler");

    ClassDB::bind_method(D_METHOD("set_max_angle_euler"), &RobotIkJoint::set_max_angle_euler);
    ClassDB::bind_method(D_METHOD("get_max_angle_euler"), &RobotIkJoint::get_max_angle_euler);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_angle_euler", PROPERTY_HINT_RANGE, "-360,360"), "set_max_angle_euler", "get_max_angle_euler");

    ClassDB::bind_method(D_METHOD("set_current_angle_euler"), &RobotIkJoint::set_current_angle_euler);
    ClassDB::bind_method(D_METHOD("get_current_angle_euler"), &RobotIkJoint::get_current_angle_euler);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "current_angle_euler", PROPERTY_HINT_RANGE, "-360,360"), "set_current_angle_euler", "get_current_angle_euler");


    ClassDB::bind_method(D_METHOD("set_update_method"), &RobotIkJoint::set_update_method);
    ClassDB::bind_method(D_METHOD("get_update_method"), &RobotIkJoint::get_update_method);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "update_method", PROPERTY_HINT_ENUM, "Process:0,Physics process:1"), "set_update_method", "get_update_method");


    ADD_GROUP("In-editor properties", "");
    
    ClassDB::bind_method(D_METHOD("set_is_updated_in_editor"), &RobotIkJoint::set_is_updated_in_editor);
    ClassDB::bind_method(D_METHOD("get_is_updated_in_editor"), &RobotIkJoint::get_is_updated_in_editor);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_updated_in_editor", PROPERTY_HINT_NONE), "set_is_updated_in_editor", "get_is_updated_in_editor");

}


RobotIkJoint::RobotIkJoint() {
    _min_angle_euler = 0.0f;
    _max_angle_euler = 180.0f;
    _center_angle_euler = 90.0f;
    _min_angle_radian = Math::deg_to_rad(_min_angle_euler);
    _max_angle_radian = Math::deg_to_rad(_max_angle_euler);
    _center_angle_radian = Math::deg_to_rad(_center_angle_euler);

    _joint_axis = Vector3( 1.0, 0.0, 0.0 );
    _joint_perpendicular_axis = Vector3(0.0, 0.0, -1.0 );

    _current_angle_euler = _center_angle_euler;
    _current_angle_radian = _center_angle_radian;

    _update_method = RobotIkJointUpdateMethod::PROCESS;

    _is_arm_tip = false;
    _is_updated_in_editor = false;

    _target_node = nullptr;
}


RobotIkJoint::~RobotIkJoint() {
    _target_node = nullptr;
}



void RobotIkJoint::evaluate(Vector3 arm_tip_global_position, Vector3 target_global_position ) {
    // Skip the evaluation if this is a the arm tip.
    if( !_is_arm_tip ) {
        // Check that to_local can be done.
        if( get_transform().basis.determinant() == 0 ) return;

        // Point towards the target.
        Vector3 arm_tip_local = to_local(arm_tip_global_position);
        Vector3 target_local = to_local(target_global_position);

        Vector3 joint_position = get_transform().get_origin();
        Vector3 direction_to_arm_tip_local = arm_tip_local.normalized();//(arm_tip_local - joint_position).normalized();
        Vector3 direction_to_target_local = target_local.normalized();// (target_local - joint_position).normalized();

        Quaternion current_rotation = get_transform().basis.get_rotation_quaternion();
        Quaternion point_to_target_rotation = Quaternion(direction_to_arm_tip_local, direction_to_target_local) * current_rotation;
        // Constraint to hinge axis.
        Vector3 hinge_axis_local = point_to_target_rotation.xform(_joint_axis);
        Quaternion hinge_rotation_back = Quaternion( hinge_axis_local, _joint_axis );

        Quaternion hinge_corrected_rotation = hinge_rotation_back * point_to_target_rotation;
        hinge_corrected_rotation.normalize();
        set_quaternion(hinge_corrected_rotation);
        // Constraint to the min & max angles.
        Vector3 rotation_axis = Vector3(1.0f, 0.0f, 0.0f); 
        float rotation_angle_radian = 0.0f;
        get_transform().get_basis().get_rotation_axis_angle_local(rotation_axis, rotation_angle_radian);

        float constraint_corrected_angle_radian = -rotation_angle_radian + _center_angle_radian;
        
        if( constraint_corrected_angle_radian < _min_angle_radian ) {
            constraint_corrected_angle_radian = _min_angle_radian;
        } else if( constraint_corrected_angle_radian > _max_angle_radian ) {
            constraint_corrected_angle_radian = _max_angle_radian;
        }
        _current_angle_radian = constraint_corrected_angle_radian;
        _current_angle_euler = Math::rad_to_deg( _current_angle_radian );
        
        constraint_corrected_angle_radian -= _center_angle_radian;
        constraint_corrected_angle_radian = -constraint_corrected_angle_radian; 
        Quaternion constrained_rotation = Quaternion( rotation_axis, constraint_corrected_angle_radian );
        set_quaternion(constrained_rotation);
        
    }

    // Check the parent.
    if( get_parent() == nullptr ) return;
    RobotIkJoint* parent_joint = Object::cast_to<RobotIkJoint>(get_parent());
    if( parent_joint == nullptr ) return;
    parent_joint->evaluate( arm_tip_global_position, target_global_position );

}

void RobotIkJoint::update_evaluation(double delta) {
    if( !is_inside_tree() ) return;
    if( !_is_arm_tip ) return;
    if(Engine::get_singleton()->is_editor_hint() ) {
        if( _is_updated_in_editor == false) return;
    } 
    if( _target_node == nullptr ) {
        ERR_FAIL_COND_MSG( _target_node_path.is_empty(), "No target node path set for the robot ik joint." );
        Node* target_node = get_node_or_null( _target_node_path );
        ERR_FAIL_COND_MSG( target_node == nullptr, "Could not find the target node for the robot ik joint with the given node path: " + _target_node_path + "." );
        _target_node = Object::cast_to<Node3D>(target_node);
        ERR_FAIL_COND_MSG( _target_node == nullptr, "The target node must be of type Node3D or a node type derived from it." );
    } else {
        evaluate( get_global_position(), _target_node->get_global_position());
    }
    
}

// Godot virtuals.

void RobotIkJoint::_process(double delta) {
    if( _update_method == RobotIkJointUpdateMethod::PROCESS ) {
        update_evaluation(delta);
    }
}    


void RobotIkJoint::_physics_process(double delta) {
    if( _update_method == RobotIkJointUpdateMethod::PHYSICS_PROCESS ) {
        update_evaluation(delta);
    }
}    


// Getters and setters.


void RobotIkJoint::set_current_angle_euler( float angle ) {
    _current_angle_euler = angle;
    _current_angle_radian = Math::deg_to_rad(_current_angle_euler);

}

float RobotIkJoint::get_current_angle_euler() const {
    return _current_angle_euler;
}


void RobotIkJoint::set_min_angle_euler( float angle ) {
    _min_angle_euler = angle;
    _min_angle_radian = Math::deg_to_rad(_min_angle_euler);

}

float RobotIkJoint::get_min_angle_euler() const {
    return _min_angle_euler;
}

void RobotIkJoint::set_max_angle_euler( float angle ) {
    _max_angle_euler = angle;
    _max_angle_radian = Math::deg_to_rad(_max_angle_euler);
}

float RobotIkJoint::get_max_angle_euler() const {
    return _max_angle_euler;
}

void RobotIkJoint::set_center_angle_euler( float angle ) {
    _center_angle_euler = angle;
    _center_angle_radian = Math::deg_to_rad(_center_angle_euler);
}

float RobotIkJoint::get_center_angle_euler() const {
    return _center_angle_euler;
}

void RobotIkJoint::set_is_arm_tip( bool is_arm_tip ) {
    _is_arm_tip = is_arm_tip;
}

bool RobotIkJoint::get_is_arm_tip() const {
    return _is_arm_tip;
}

void RobotIkJoint::set_is_updated_in_editor( bool is_updated_in_editor ) {
    _is_updated_in_editor = is_updated_in_editor;
}

bool RobotIkJoint::get_is_updated_in_editor() const {
    return _is_updated_in_editor;
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


void RobotIkJoint::set_target_node_path( NodePath path ) {
    _target_node_path = path;
}

NodePath RobotIkJoint::get_target_node_path() const {
    return _target_node_path;
}


void RobotIkJoint::set_update_method( int method ) {
    if( method == 0 ) {
        _update_method = RobotIkJointUpdateMethod::PROCESS;
        return;
    }
    _update_method = RobotIkJointUpdateMethod::PHYSICS_PROCESS;
}

int  RobotIkJoint::get_update_method() const {
    if( _update_method == RobotIkJointUpdateMethod::PROCESS) {
        return 0;
    }
    return 1;
}


