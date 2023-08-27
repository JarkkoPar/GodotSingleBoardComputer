#include "I2cPca9685.h"

#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/os.hpp>

using namespace godot;

I2cPca9685::I2cPca9685() {
    _i2c_device_address = 0x40;
    for( int i = 0; i < 16; ++i ) {
        servo_angles[i] = 90.0f;
        servo_min_angle_ms[i] = 1.0;
        servo_max_angle_ms[i] = 2.0;
    }

    _pwm_frequency_hz = 50; 
    _pwm_oscillator_frequency = 25000000.0; // 25 MHz
    update_servo_min_max_angle_pulse_counts();    
}


I2cPca9685::~I2cPca9685() {

}



void I2cPca9685::_bind_methods() {
    
    
    // Initialization timeout helper.
    //ClassDB::bind_method(D_METHOD("on_timer_finished_finalize_initialize"), &I2cPca9685::on_timer_finished_finalize_initialize);   

    ClassDB::bind_method(D_METHOD("set_pwm_frequency_hz", "pwm_frequency_hz"), &I2cPca9685::set_pwm_frequency_hz);
    ClassDB::bind_method(D_METHOD("get_pwm_frequency_hz"), &I2cPca9685::get_pwm_frequency_hz);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "pwm_frequency_hz", PROPERTY_HINT_RANGE, "1,1000"), "set_pwm_frequency_hz", "get_pwm_frequency_hz");

    // The servo values.
    ClassDB::bind_method(D_METHOD("set_servo_euler_angle", "servo_index", "new_euler_angle"), &I2cPca9685::set_servo_euler_angle);
    ClassDB::bind_method(D_METHOD("get_servo_euler_angle", "servo_index"), &I2cPca9685::get_servo_euler_angle);

    ADD_GROUP("Servo min angle ms", "");
    
    /**/
    //ClassDB::bind_method(D_METHOD("set_servo0_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo0_min_angle_ms);
	//ClassDB::bind_method(D_METHOD("get_servo0_min_angle_ms"), &I2cPca9685::get_servo0_min_angle_ms);
    //ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 0", PROPERTY_HINT_NONE), "set_servo0_min_angle_ms", "get_servo0_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo0_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo0_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo0_min_angle_ms"), &I2cPca9685::get_servo0_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_0_min_angle_ms", PROPERTY_HINT_NONE), "set_servo0_min_angle_ms", "get_servo0_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo1_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo1_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo1_min_angle_ms"), &I2cPca9685::get_servo1_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_1_min_angle_ms", PROPERTY_HINT_NONE), "set_servo1_min_angle_ms", "get_servo1_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo2_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo2_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo2_min_angle_ms"), &I2cPca9685::get_servo2_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_2_min_angle_ms", PROPERTY_HINT_NONE), "set_servo2_min_angle_ms", "get_servo2_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo3_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo3_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo3_min_angle_ms"), &I2cPca9685::get_servo3_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_3_min_angle_ms", PROPERTY_HINT_NONE), "set_servo3_min_angle_ms", "get_servo3_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo4_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo4_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo4_min_angle_ms"), &I2cPca9685::get_servo4_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_4_min_angle_ms", PROPERTY_HINT_NONE), "set_servo4_min_angle_ms", "get_servo4_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo5_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo5_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo5_min_angle_ms"), &I2cPca9685::get_servo5_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_5_min_angle_ms", PROPERTY_HINT_NONE), "set_servo5_min_angle_ms", "get_servo5_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo6_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo6_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo6_min_angle_ms"), &I2cPca9685::get_servo6_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_6_min_angle_ms", PROPERTY_HINT_NONE), "set_servo6_min_angle_ms", "get_servo6_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo7_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo7_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo7_min_angle_ms"), &I2cPca9685::get_servo7_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_7_min_angle_ms", PROPERTY_HINT_NONE), "set_servo7_min_angle_ms", "get_servo7_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo8_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo8_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo8_min_angle_ms"), &I2cPca9685::get_servo8_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_8_min_angle_ms", PROPERTY_HINT_NONE), "set_servo8_min_angle_ms", "get_servo8_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo9_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo9_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo9_min_angle_ms"), &I2cPca9685::get_servo9_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_9_min_angle_ms", PROPERTY_HINT_NONE), "set_servo9_min_angle_ms", "get_servo9_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo10_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo10_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo10_min_angle_ms"), &I2cPca9685::get_servo10_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_10_min_angle_ms", PROPERTY_HINT_NONE), "set_servo10_min_angle_ms", "get_servo10_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo11_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo11_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo11_min_angle_ms"), &I2cPca9685::get_servo11_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_11_min_angle_ms", PROPERTY_HINT_NONE), "set_servo11_min_angle_ms", "get_servo11_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo12_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo12_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo12_min_angle_ms"), &I2cPca9685::get_servo12_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_12_min_angle_ms", PROPERTY_HINT_NONE), "set_servo12_min_angle_ms", "get_servo12_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo13_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo13_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo13_min_angle_ms"), &I2cPca9685::get_servo13_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_13_min_angle_ms", PROPERTY_HINT_NONE), "set_servo13_min_angle_ms", "get_servo13_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo14_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo14_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo14_min_angle_ms"), &I2cPca9685::get_servo14_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_14_min_angle_ms", PROPERTY_HINT_NONE), "set_servo14_min_angle_ms", "get_servo14_min_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo15_min_angle_ms", "new_min_angle_ms"), &I2cPca9685::set_servo15_min_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo15_min_angle_ms"), &I2cPca9685::get_servo15_min_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_15_min_angle_ms", PROPERTY_HINT_NONE), "set_servo15_min_angle_ms", "get_servo15_min_angle_ms");
    

    ADD_GROUP("Servo max angle ms", "");
    
    ClassDB::bind_method(D_METHOD("set_servo0_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo0_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo0_max_angle_ms"), &I2cPca9685::get_servo0_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_0_max_angle_ms", PROPERTY_HINT_NONE), "set_servo0_max_angle_ms", "get_servo0_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo1_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo1_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo1_max_angle_ms"), &I2cPca9685::get_servo1_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_1_max_angle_ms", PROPERTY_HINT_NONE), "set_servo1_max_angle_ms", "get_servo1_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo2_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo2_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo2_max_angle_ms"), &I2cPca9685::get_servo2_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_2_max_angle_ms", PROPERTY_HINT_NONE), "set_servo2_max_angle_ms", "get_servo2_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo3_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo3_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo3_max_angle_ms"), &I2cPca9685::get_servo3_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_3_max_angle_ms", PROPERTY_HINT_NONE), "set_servo3_max_angle_ms", "get_servo3_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo4_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo4_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo4_max_angle_ms"), &I2cPca9685::get_servo4_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_4_max_angle_ms", PROPERTY_HINT_NONE), "set_servo4_max_angle_ms", "get_servo4_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo5_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo5_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo5_max_angle_ms"), &I2cPca9685::get_servo5_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_5_max_angle_ms", PROPERTY_HINT_NONE), "set_servo5_max_angle_ms", "get_servo5_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo6_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo6_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo6_max_angle_ms"), &I2cPca9685::get_servo6_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_6_max_angle_ms", PROPERTY_HINT_NONE), "set_servo6_max_angle_ms", "get_servo6_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo7_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo7_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo7_max_angle_ms"), &I2cPca9685::get_servo7_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_7_max_angle_ms", PROPERTY_HINT_NONE), "set_servo7_max_angle_ms", "get_servo7_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo8_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo8_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo8_max_angle_ms"), &I2cPca9685::get_servo8_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_8_max_angle_ms", PROPERTY_HINT_NONE), "set_servo8_max_angle_ms", "get_servo8_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo9_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo9_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo9_max_angle_ms"), &I2cPca9685::get_servo9_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_9_max_angle_ms", PROPERTY_HINT_NONE), "set_servo9_max_angle_ms", "get_servo9_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo10_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo10_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo10_max_angle_ms"), &I2cPca9685::get_servo10_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_10_max_angle_ms", PROPERTY_HINT_NONE), "set_servo10_max_angle_ms", "get_servo10_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo11_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo11_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo11_max_angle_ms"), &I2cPca9685::get_servo11_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_11_max_angle_ms", PROPERTY_HINT_NONE), "set_servo11_max_angle_ms", "get_servo11_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo12_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo12_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo12_max_angle_ms"), &I2cPca9685::get_servo12_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_12_max_angle_ms", PROPERTY_HINT_NONE), "set_servo12_max_angle_ms", "get_servo12_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo13_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo13_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo13_max_angle_ms"), &I2cPca9685::get_servo13_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_13_max_angle_ms", PROPERTY_HINT_NONE), "set_servo13_max_angle_ms", "get_servo13_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo14_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo14_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo14_max_angle_ms"), &I2cPca9685::get_servo14_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_14_max_angle_ms", PROPERTY_HINT_NONE), "set_servo14_max_angle_ms", "get_servo14_max_angle_ms");
    
    ClassDB::bind_method(D_METHOD("set_servo15_max_angle_ms", "new_max_angle_ms"), &I2cPca9685::set_servo15_max_angle_ms);
	ClassDB::bind_method(D_METHOD("get_servo15_max_angle_ms"), &I2cPca9685::get_servo15_max_angle_ms);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_15_max_angle_ms", PROPERTY_HINT_NONE), "set_servo15_max_angle_ms", "get_servo15_max_angle_ms");
    

    /**/

    ADD_GROUP("Servo angles", "");

    ClassDB::bind_method(D_METHOD("set_servo0_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo0_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo0_euler_angle"), &I2cPca9685::get_servo0_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_0_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo0_euler_angle", "get_servo0_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo1_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo1_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo1_euler_angle"), &I2cPca9685::get_servo1_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_1_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo1_euler_angle", "get_servo1_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo2_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo2_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo2_euler_angle"), &I2cPca9685::get_servo2_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_2_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo2_euler_angle", "get_servo2_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo3_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo3_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo3_euler_angle"), &I2cPca9685::get_servo3_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_3_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo3_euler_angle", "get_servo3_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo4_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo4_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo4_euler_angle"), &I2cPca9685::get_servo4_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_4_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo4_euler_angle", "get_servo4_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo5_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo5_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo5_euler_angle"), &I2cPca9685::get_servo5_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_5_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo5_euler_angle", "get_servo5_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo6_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo6_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo6_euler_angle"), &I2cPca9685::get_servo6_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_6_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo6_euler_angle", "get_servo6_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo7_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo7_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo7_euler_angle"), &I2cPca9685::get_servo7_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_7_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo7_euler_angle", "get_servo7_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo8_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo8_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo8_euler_angle"), &I2cPca9685::get_servo8_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_8_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo8_euler_angle", "get_servo8_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo9_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo9_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo9_euler_angle"), &I2cPca9685::get_servo9_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_9_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo9_euler_angle", "get_servo9_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo10_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo9_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo10_euler_angle"), &I2cPca9685::get_servo9_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_10_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo10_euler_angle", "get_servo10_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo11_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo11_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo11_euler_angle"), &I2cPca9685::get_servo11_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_11_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo11_euler_angle", "get_servo11_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo12_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo12_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo12_euler_angle"), &I2cPca9685::get_servo12_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_12_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo12_euler_angle", "get_servo12_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo13_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo13_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo13_euler_angle"), &I2cPca9685::get_servo13_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_13_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo13_euler_angle", "get_servo13_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo14_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo14_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo14_euler_angle"), &I2cPca9685::get_servo14_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_14_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo14_euler_angle", "get_servo14_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo15_euler_angle", "new_euler_angle"), &I2cPca9685::set_servo15_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo15_euler_angle"), &I2cPca9685::get_servo15_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "servo_15_angle", PROPERTY_HINT_RANGE, "0.0,180.0"), "set_servo15_euler_angle", "get_servo15_euler_angle");

    /**
    // The led values.
    ADD_GROUP("LEDs", "");
    ClassDB::bind_method(D_METHOD("set_led0_value", "led_value"), &I2cPca9685::set_led0_value);
	ClassDB::bind_method(D_METHOD("get_led0_value"), &I2cPca9685::get_led0_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 0", PROPERTY_HINT_NONE, ""), "set_led0_value", "get_led0_value");

    ClassDB::bind_method(D_METHOD("set_led1_value", "led_value"), &I2cPca9685::set_led1_value);
	ClassDB::bind_method(D_METHOD("get_led1_value"), &I2cPca9685::get_led1_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 1", PROPERTY_HINT_NONE, ""), "set_led1_value", "get_led1_value");
    
    ClassDB::bind_method(D_METHOD("set_led2_value", "led_value"), &I2cPca9685::set_led2_value);
	ClassDB::bind_method(D_METHOD("get_led2_value"), &I2cPca9685::get_led2_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 2", PROPERTY_HINT_NONE, ""), "set_led2_value", "get_led2_value");
    
    ClassDB::bind_method(D_METHOD("set_led3_value", "led_value"), &I2cPca9685::set_led3_value);
	ClassDB::bind_method(D_METHOD("get_led3_value"), &I2cPca9685::get_led3_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 3", PROPERTY_HINT_NONE, ""), "set_led3_value", "get_led3_value");
    
    ClassDB::bind_method(D_METHOD("set_led4_value", "led_value"), &I2cPca9685::set_led4_value);
	ClassDB::bind_method(D_METHOD("get_led4_value"), &I2cPca9685::get_led4_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 4", PROPERTY_HINT_NONE, ""), "set_led4_value", "get_led4_value");
    
    ClassDB::bind_method(D_METHOD("set_led5_value", "led_value"), &I2cPca9685::set_led5_value);
	ClassDB::bind_method(D_METHOD("get_led5_value"), &I2cPca9685::get_led5_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 5", PROPERTY_HINT_NONE, ""), "set_led5_value", "get_led5_value");
    
    ClassDB::bind_method(D_METHOD("set_led6_value", "led_value"), &I2cPca9685::set_led6_value);
	ClassDB::bind_method(D_METHOD("get_led6_value"), &I2cPca9685::get_led6_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 6", PROPERTY_HINT_NONE, ""), "set_led6_value", "get_led6_value");
    
    ClassDB::bind_method(D_METHOD("set_led7_value", "led_value"), &I2cPca9685::set_led7_value);
	ClassDB::bind_method(D_METHOD("get_led7_value"), &I2cPca9685::get_led7_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 7", PROPERTY_HINT_NONE, ""), "set_led7_value", "get_led7_value");
    
    ClassDB::bind_method(D_METHOD("set_led8_value", "led_value"), &I2cPca9685::set_led8_value);
	ClassDB::bind_method(D_METHOD("get_led8_value"), &I2cPca9685::get_led8_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 8", PROPERTY_HINT_NONE, ""), "set_led8_value", "get_led8_value");
    
    ClassDB::bind_method(D_METHOD("set_led9_value", "led_value"), &I2cPca9685::set_led9_value);
	ClassDB::bind_method(D_METHOD("get_led9_value"), &I2cPca9685::get_led9_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 9", PROPERTY_HINT_NONE, ""), "set_led9_value", "get_led9_value");
    
    ClassDB::bind_method(D_METHOD("set_led10_value", "led_value"), &I2cPca9685::set_led10_value);
	ClassDB::bind_method(D_METHOD("get_led10_value"), &I2cPca9685::get_led10_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 10", PROPERTY_HINT_NONE, ""), "set_led10_value", "get_led10_value");
    
    ClassDB::bind_method(D_METHOD("set_led11_value", "led_value"), &I2cPca9685::set_led11_value);
	ClassDB::bind_method(D_METHOD("get_led11_value"), &I2cPca9685::get_led11_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 11", PROPERTY_HINT_NONE, ""), "set_led11_value", "get_led11_value");
    
    ClassDB::bind_method(D_METHOD("set_led12_value", "led_value"), &I2cPca9685::set_led12_value);
	ClassDB::bind_method(D_METHOD("get_led12_value"), &I2cPca9685::get_led12_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 12", PROPERTY_HINT_NONE, ""), "set_led12_value", "get_led12_value");
    
    ClassDB::bind_method(D_METHOD("set_led13_value", "led_value"), &I2cPca9685::set_led13_value);
	ClassDB::bind_method(D_METHOD("get_led13_value"), &I2cPca9685::get_led13_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 13", PROPERTY_HINT_NONE, ""), "set_led13_value", "get_led13_value");
    
    ClassDB::bind_method(D_METHOD("set_led14_value", "led_value"), &I2cPca9685::set_led14_value);
	ClassDB::bind_method(D_METHOD("get_led14_value"), &I2cPca9685::get_led14_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 14", PROPERTY_HINT_NONE, ""), "set_led14_value", "get_led14_value");
    
    ClassDB::bind_method(D_METHOD("set_led15_value", "led_value"), &I2cPca9685::set_led15_value);
	ClassDB::bind_method(D_METHOD("get_led15_value"), &I2cPca9685::get_led15_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 15", PROPERTY_HINT_NONE, ""), "set_led15_value", "get_led15_value");
    
    //ClassDB::bind_method(D_METHOD("set_led0_value", "led_value"), &I2cPca9685::set_led0_value);
	//ClassDB::bind_method(D_METHOD("get_led0_value"), &I2cPca9685::get_led0_value);
    //ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 0", PROPERTY_HINT_NONE, ""), "set_led0_value", "get_led0_value");
    /**/
}




void I2cPca9685::_process(double delta) {
    if( !_is_device_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;

    // We update at the same hz as the servo's are running
    _pca9685_update_wait_time -= delta;
    if( _pca9685_update_wait_time > 0.0 ) return;

    // Time to update the servo values.
    _pca9685_update_wait_time = _pca9685_update_frame_delay;
    float angle_multiplier = 1.0f / 180.0f;
    for( int i = 0; i < 16; ++i ) {
        
        // Degrees go between 0.0 and 180.0, so scale
        // to 0..1 first.
        float angle = servo_angles[i];
        float tween = angle * angle_multiplier;
        int off_index = (int)((float)_servo_pulses_between_min_max_angles[i] * tween);
        if( off_index < 0 ) {
            off_index = 0;
        }

        set_led_pulse_range( i, 0, _servo_min_angle_pulses[i] + off_index );
    }
}


void I2cPca9685::_physics_process(double delta) {

}


void I2cPca9685::_notification(int p_what) {

}


void I2cPca9685::update_servo_min_max_angle_pulse_counts() {
    double duty_cycle_ms = 1000.0 / (double)_pwm_frequency_hz;
    double time_per_pulse = duty_cycle_ms / 4096.0;
    //double pulses_per_ms = 4096.0 / duty_cycle_ms;

    for( int i = 0; i < 16; ++i ) {
        _servo_min_angle_pulses[i] = (int)(servo_min_angle_ms[i] / time_per_pulse);
        _servo_max_angle_pulses[i] = (int)(servo_max_angle_ms[i] / time_per_pulse);
        
        //_servo_min_angle_pulses[i] = (int)(servo_min_angle_ms[i] * pulses_per_ms);
        //_servo_max_angle_pulses[i] = (int)(servo_max_angle_ms[i] * pulses_per_ms);
        _servo_pulses_between_min_max_angles[i] = _servo_max_angle_pulses[i] - _servo_min_angle_pulses[i];
    }
    
    // Also update the update frame delays.
    _pca9685_update_frame_delay = 1.0 / (double)_pwm_frequency_hz;
    _pca9685_update_wait_time = 0.0;

    // And the prescale-value.
    // With 50Hz frequency there will be 4096 * 50 pulses per second, so the 
    // pre-scaling value comes to 25 000 000 / (4096 * 50) = 25 000 000 / 204 800 = 122.07 ~ 122 = 0x7A
    _pwm_prescale_value = (int)(_pwm_oscillator_frequency / (4096.0 * (double)_pwm_frequency_hz));
}


void I2cPca9685::set_pwm_frequency_hz( int new_frequency_hz ) {
    _pwm_frequency_hz = new_frequency_hz;
    if( _pwm_frequency_hz > 1000 ) {
        _pwm_frequency_hz = 1000;
    }
    if( _pwm_frequency_hz < 1 ) {
        _pwm_frequency_hz = 1;
    }
    if(Engine::get_singleton()->is_editor_hint()) return;
    if( _i2c_device_fd < 0 ) return; // Device not opened so cannot do this yet. Will be set on init in any case.
    if( !_is_device_initialized ) return;
    if( !_is_active ) return;

    // Send to the device if not in editor.
    //int prescaling = (int)(25000000.0f / (4096.0f * (float)_pwm_frequency_hz) - 0.5f);
    update_servo_min_max_angle_pulse_counts();

    uint8_t current_state = read_byte_from_device_register(PCA9685Registers::MODE1);
    uint8_t sleep = current_state | PCA9685Mode::SLEEP;
    uint8_t wake = current_state & ~(PCA9685Mode::SLEEP);
    uint8_t restart = wake | 0x80;

    write_byte_to_device_register(PCA9685Registers::MODE1, sleep);
    OS::get_singleton()->delay_msec(50);
    write_byte_to_device_register(PCA9685Registers::PRESCALE, (uint8_t)_pwm_prescale_value ); //prescaling);
    OS::get_singleton()->delay_msec(50);
    write_byte_to_device_register(PCA9685Registers::MODE1, restart );
    OS::get_singleton()->delay_msec(50);
    write_byte_to_device_register(PCA9685Registers::MODE1, wake);
    OS::get_singleton()->delay_msec(50);
    // Use a timer to delay 500 ms before waking.
    /**
    Timer* waittimer = memnew(Timer);//::_new();
    waittimer->set_name("wake_wait_timer");
    add_child(waittimer);
    waittimer->set_wait_time(0.5);
    waittimer->set_one_shot(true);
    waittimer->connect("timeout", godot::Callable(this, "on_timer_finished_finalize_initialize"));
     // callable_mp(this, &I2cPca9685::on_timer_finished_finalize_initialize ));
    //"on_timer_finished_finalize_initialize");
    /**/
    
}

int  I2cPca9685::get_pwm_frequency_hz() const {
    return _pwm_frequency_hz;
}


bool I2cPca9685::_configure_i2c_device() {
    ERR_FAIL_COND_V_MSG(_i2c_device_fd < 0, false, "PCA9685 configuration failed because the device file is not opened.");
    
    // Read the current MODE1 register value.
    //uint8_t current_state = read_byte_from_device_register(PCA9685Registers::MODE1);


    // First set the PWM Frequency as set on the node.
    //uint8_t prescale_value = (int)(25000000.0f / (4096.0f * (float)(_pwm_frequency_hz)) - 0.5f);
    //update_servo_min_max_angle_pulse_counts();
    
    set_pwm_frequency_hz(_pwm_frequency_hz);

    return true;
}


void I2cPca9685::wake_pca9685() {
    // Wake by getting the current state, then clearing the 
    // sleep-bit, and setting the state back.
    uint8_t current_state = read_byte_from_device_register(PCA9685Registers::MODE1);
    current_state &= ~(PCA9685Mode::SLEEP); 
    write_byte_to_device_register( PCA9685Registers::MODE1, current_state);
}

void I2cPca9685::sleep_pca9685() {
    // Sleep by getting the current state, then setting the
    // sleep-bit, and setting the state back.
    uint8_t current_state = read_byte_from_device_register(PCA9685Registers::MODE1);
    current_state |= (PCA9685Mode::SLEEP); 
    write_byte_to_device_register( PCA9685Registers::MODE1, current_state);
}

void I2cPca9685::restart_pca9685() {
    write_byte_to_device_register( PCA9685Registers::MODE1, PCA9685Mode::RESTART);
}


// on-off indices are for pwm singal steps from 0 to 4095.
void I2cPca9685::set_led_pulse_range( int led_index, int on_index, int off_index ) {
    ERR_FAIL_COND_MSG(led_index < 0 || led_index > 15, "set_led_pulse_range: Led index out of bounds.");
    int iled_index = led_index * 4;
    uint16_t uon_index = (uint16_t)on_index;
    uint16_t uoff_index = (uint16_t)off_index;

    uint8_t uon_low = (uint8_t)(uon_index & 0x00FF);
    uint8_t uon_high = (uint8_t)((uon_index >> 8) & 0x000F);
    uint8_t uoff_low = (uint8_t)(uoff_index & 0x00FF) ;
    uint8_t uoff_high = (uint8_t)((uoff_index >> 8) & 0x000F);

    write_byte_to_device_register(PCA9685Registers::LED0_ON_L + iled_index,  uon_low  );
    write_byte_to_device_register(PCA9685Registers::LED0_ON_H + iled_index,  uon_high );
    write_byte_to_device_register(PCA9685Registers::LED0_OFF_L + iled_index, uoff_low );
    write_byte_to_device_register(PCA9685Registers::LED0_OFF_H + iled_index, uoff_high );
    
}

// Getters and setters for servos.

void I2cPca9685::set_servo0_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[0] = new_ms;
}

void I2cPca9685::set_servo1_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[1] = new_ms;
}

void I2cPca9685::set_servo2_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[2] = new_ms;
}

void I2cPca9685::set_servo3_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[3] = new_ms;
}

void I2cPca9685::set_servo4_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[4] = new_ms;
}

void I2cPca9685::set_servo5_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[5] = new_ms;
}

void I2cPca9685::set_servo6_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[6] = new_ms;
}

void I2cPca9685::set_servo7_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[7] = new_ms;
}

void I2cPca9685::set_servo8_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[8] = new_ms;
}

void I2cPca9685::set_servo9_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[9] = new_ms;
}

void I2cPca9685::set_servo10_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[10] = new_ms;
}

void I2cPca9685::set_servo11_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[11] = new_ms;
}

void I2cPca9685::set_servo12_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[12] = new_ms;
}

void I2cPca9685::set_servo13_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[13] = new_ms;
}

void I2cPca9685::set_servo14_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[14] = new_ms;
}

void I2cPca9685::set_servo15_min_angle_ms( const double new_ms ) {
    servo_min_angle_ms[15] = new_ms;
}


double I2cPca9685::get_servo0_min_angle_ms() const {
    return servo_min_angle_ms[0];
}

double I2cPca9685::get_servo1_min_angle_ms() const {
    return servo_min_angle_ms[1];
}

double I2cPca9685::get_servo2_min_angle_ms() const {
    return servo_min_angle_ms[2];
}

double I2cPca9685::get_servo3_min_angle_ms() const {
    return servo_min_angle_ms[3];
}

double I2cPca9685::get_servo4_min_angle_ms() const {
    return servo_min_angle_ms[4];
}

double I2cPca9685::get_servo5_min_angle_ms() const {
    return servo_min_angle_ms[5];
}

double I2cPca9685::get_servo6_min_angle_ms() const {
    return servo_min_angle_ms[6];
}

double I2cPca9685::get_servo7_min_angle_ms() const {
    return servo_min_angle_ms[7];
}

double I2cPca9685::get_servo8_min_angle_ms() const {
    return servo_min_angle_ms[8];
}

double I2cPca9685::get_servo9_min_angle_ms() const {
    return servo_min_angle_ms[9];
}

double I2cPca9685::get_servo10_min_angle_ms() const {
    return servo_min_angle_ms[10];
}

double I2cPca9685::get_servo11_min_angle_ms() const {
    return servo_min_angle_ms[11];
}

double I2cPca9685::get_servo12_min_angle_ms() const {
    return servo_min_angle_ms[12];
}

double I2cPca9685::get_servo13_min_angle_ms() const {
    return servo_min_angle_ms[13];
}

double I2cPca9685::get_servo14_min_angle_ms() const {
    return servo_min_angle_ms[14];
}

double I2cPca9685::get_servo15_min_angle_ms() const {
    return servo_min_angle_ms[15];
}


void I2cPca9685::set_servo0_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[0] = new_ms;
}

void I2cPca9685::set_servo1_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[1] = new_ms;
}

void I2cPca9685::set_servo2_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[2] = new_ms;
}

void I2cPca9685::set_servo3_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[3] = new_ms;
}

void I2cPca9685::set_servo4_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[4] = new_ms;
}

void I2cPca9685::set_servo5_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[5] = new_ms;
}

void I2cPca9685::set_servo6_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[6] = new_ms;
}

void I2cPca9685::set_servo7_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[7] = new_ms;
}

void I2cPca9685::set_servo8_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[8] = new_ms;
}

void I2cPca9685::set_servo9_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[9] = new_ms;
}

void I2cPca9685::set_servo10_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[10] = new_ms;
}

void I2cPca9685::set_servo11_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[11] = new_ms;
}

void I2cPca9685::set_servo12_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[12] = new_ms;
}

void I2cPca9685::set_servo13_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[13] = new_ms;
}

void I2cPca9685::set_servo14_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[14] = new_ms;
}

void I2cPca9685::set_servo15_max_angle_ms( const double new_ms ) {
    servo_max_angle_ms[15] = new_ms;
}


double I2cPca9685::get_servo0_max_angle_ms() const {
    return servo_max_angle_ms[0];
}

double I2cPca9685::get_servo1_max_angle_ms() const {
    return servo_max_angle_ms[1];
}

double I2cPca9685::get_servo2_max_angle_ms() const {
    return servo_max_angle_ms[2];
}

double I2cPca9685::get_servo3_max_angle_ms() const {
    return servo_max_angle_ms[3];
}

double I2cPca9685::get_servo4_max_angle_ms() const {
    return servo_max_angle_ms[4];
}

double I2cPca9685::get_servo5_max_angle_ms() const {
    return servo_max_angle_ms[5];
}

double I2cPca9685::get_servo6_max_angle_ms() const {
    return servo_max_angle_ms[6];
}

double I2cPca9685::get_servo7_max_angle_ms() const {
    return servo_max_angle_ms[7];
}

double I2cPca9685::get_servo8_max_angle_ms() const {
    return servo_max_angle_ms[8];
}

double I2cPca9685::get_servo9_max_angle_ms() const {
    return servo_max_angle_ms[9];
}

double I2cPca9685::get_servo10_max_angle_ms() const {
    return servo_max_angle_ms[10];
}

double I2cPca9685::get_servo11_max_angle_ms() const {
    return servo_max_angle_ms[11];
}

double I2cPca9685::get_servo12_max_angle_ms() const {
    return servo_max_angle_ms[12];
}

double I2cPca9685::get_servo13_max_angle_ms() const {
    return servo_max_angle_ms[13];
}

double I2cPca9685::get_servo14_max_angle_ms() const {
    return servo_max_angle_ms[14];
}

double I2cPca9685::get_servo15_max_angle_ms() const {
    return servo_max_angle_ms[15];
}


/**
void I2cPca9685::set_servo_min_angle_pulses( const int min_angle_pulses ) {
    _servo_min_angle_pulses = min_angle_pulses;
}

int  I2cPca9685::get_servo_min_angle_pulses() const {
    return _servo_min_angle_pulses;
}

void I2cPca9685::set_servo_max_angle_pulses( const int max_angle_pulses ) {
    _servo_max_angle_pulses = max_angle_pulses;
}

int  I2cPca9685::get_servo_max_angle_pulses() const {
    return _servo_max_angle_pulses;
}
/**/


void I2cPca9685::set_servo_euler_angle( const int servo_index, const float new_euler_angle ) {
    ERR_FAIL_COND_MSG(servo_index < 0 || servo_index > 15, "set_servo_euler_angle: Servo index out of bounds.");
    servo_angles[servo_index] = new_euler_angle;
}

void I2cPca9685::set_servo0_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(0, new_euler_angle);
}

void I2cPca9685::set_servo1_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(1, new_euler_angle);
}
void I2cPca9685::set_servo2_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(2, new_euler_angle);
}
void I2cPca9685::set_servo3_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(3, new_euler_angle);
}
void I2cPca9685::set_servo4_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(4, new_euler_angle);
}
void I2cPca9685::set_servo5_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(5, new_euler_angle);
}
void I2cPca9685::set_servo6_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(6, new_euler_angle);
}
void I2cPca9685::set_servo7_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(7, new_euler_angle);
}
void I2cPca9685::set_servo8_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(8, new_euler_angle);
}
void I2cPca9685::set_servo9_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(9, new_euler_angle);
}
void I2cPca9685::set_servo10_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(10, new_euler_angle);
}
void I2cPca9685::set_servo11_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(11, new_euler_angle);
}
void I2cPca9685::set_servo12_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(12, new_euler_angle);
}
void I2cPca9685::set_servo13_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(13, new_euler_angle);
}
void I2cPca9685::set_servo14_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(14, new_euler_angle);
}
void I2cPca9685::set_servo15_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(15, new_euler_angle);
}


float I2cPca9685::get_servo_euler_angle( const int servo_index ) const {
    if( servo_index < 0 ) return 0.0f;
    if( servo_index > 15 ) return 0.0f;
    return servo_angles[servo_index];    
}

float I2cPca9685::get_servo0_euler_angle() const {
    return servo_angles[0];
}

float I2cPca9685::get_servo1_euler_angle() const {
    return servo_angles[1];
}

float I2cPca9685::get_servo2_euler_angle() const {
    return servo_angles[2];
}

float I2cPca9685::get_servo3_euler_angle() const {
    return servo_angles[3];
}

float I2cPca9685::get_servo4_euler_angle() const {
    return servo_angles[4];
}

float I2cPca9685::get_servo5_euler_angle() const {
    return servo_angles[5];
}

float I2cPca9685::get_servo6_euler_angle() const {
    return servo_angles[6];
}

float I2cPca9685::get_servo7_euler_angle() const {
    return servo_angles[7];
}

float I2cPca9685::get_servo8_euler_angle() const {
    return servo_angles[8];
}

float I2cPca9685::get_servo9_euler_angle() const {
    return servo_angles[9];
}

float I2cPca9685::get_servo10_euler_angle() const {
    return servo_angles[10];
}

float I2cPca9685::get_servo11_euler_angle() const {
    return servo_angles[11];
}

float I2cPca9685::get_servo12_euler_angle() const {
    return servo_angles[12];
}

float I2cPca9685::get_servo13_euler_angle() const {
    return servo_angles[13];
}

float I2cPca9685::get_servo14_euler_angle() const {
    return servo_angles[14];
}

float I2cPca9685::get_servo15_euler_angle() const {
    return servo_angles[15];
}



// Getters and setters for leds.

int I2cPca9685::get_led0_value(){
    return 0;
}

int I2cPca9685::get_led1_value(){
    return 0;
}

int I2cPca9685::get_led2_value(){
    return 0;
}

int I2cPca9685::get_led3_value(){
    return 0;
}

int I2cPca9685::get_led4_value(){
    return 0;
}

int I2cPca9685::get_led5_value(){
    return 0;
}

int I2cPca9685::get_led6_value(){
    return 0;
}

int I2cPca9685::get_led7_value(){
    return 0;
}

int I2cPca9685::get_led8_value(){
    return 0;
}

int I2cPca9685::get_led9_value(){
    return 0;
}

int I2cPca9685::get_led10_value(){
    return 0;
}

int I2cPca9685::get_led11_value(){
    return 0;
}

int I2cPca9685::get_led12_value(){
    return 0;
}

int I2cPca9685::get_led13_value(){
    return 0;
}

int I2cPca9685::get_led14_value(){
    return 0;
}

int I2cPca9685::get_led15_value(){
    return 0;
}


void I2cPca9685::set_led_value(int led_index, int new_led_value ) {
    write_byte_to_device_register(PCA9685Registers::LED0 + led_index * 4, new_led_value );

}

void I2cPca9685::set_led0_value(int new_led_value){

}

void I2cPca9685::set_led1_value(int new_led_value){

}

void I2cPca9685::set_led2_value(int new_led_value){

}

void I2cPca9685::set_led3_value(int new_led_value){

}

void I2cPca9685::set_led4_value(int new_led_value){

}

void I2cPca9685::set_led5_value(int new_led_value){

}

void I2cPca9685::set_led6_value(int new_led_value){

}

void I2cPca9685::set_led7_value(int new_led_value){

}

void I2cPca9685::set_led8_value(int new_led_value){

}

void I2cPca9685::set_led9_value(int new_led_value){

}

void I2cPca9685::set_led10_value(int new_led_value){

}

void I2cPca9685::set_led11_value(int new_led_value){

}

void I2cPca9685::set_led12_value(int new_led_value){

}

void I2cPca9685::set_led13_value(int new_led_value){

}

void I2cPca9685::set_led14_value(int new_led_value){

}

void I2cPca9685::set_led15_value(int new_led_value){

}

