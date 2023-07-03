#include "I2CPCA9685.h"

#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/engine.hpp>


using namespace godot;

I2CPCA9685::I2CPCA9685() {
    _pwm_frequency_hz = 50; 
    update_servo_min_max_angle_pulse_counts();
    _is_pca9685_initialized = false;
    
    for( int i = 0; i < 16; ++i ) {
        servo_angles[i] = 0.0f;
    }
}


I2CPCA9685::~I2CPCA9685() {

}



void I2CPCA9685::_bind_methods() {
    
    
    // Initialization timeout helper.
    ClassDB::bind_method(D_METHOD("on_timer_finished_finalize_initialize"), &I2CPCA9685::on_timer_finished_finalize_initialize);   

    ClassDB::bind_method(D_METHOD("set_pwm_frequency_hz"), &I2CPCA9685::set_pwm_frequency_hz);
    ClassDB::bind_method(D_METHOD("get_pwm_frequency_hz"), &I2CPCA9685::get_pwm_frequency_hz);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "PWM Frequency Hz", PROPERTY_HINT_RANGE, "1,1000"), "set_pwm_frequency_hz", "get_pwm_frequency_hz");

    // The servo values.
    ClassDB::bind_method(D_METHOD("set_servo_euler_angle", "servo_index", "new_euler_angle"), &I2CPCA9685::set_servo_euler_angle);
    ClassDB::bind_method(D_METHOD("get_servo_euler_angle", "servo_index"), &I2CPCA9685::get_servo_euler_angle);


    ADD_GROUP("Servos", "");
    /**
    ClassDB::bind_method(D_METHOD("set_servo_min_angle_pulses"), &I2CPCA9685::set_servo_min_angle_pulses);
    ClassDB::bind_method(D_METHOD("get_servo_min_angle_pulses"), &I2CPCA9685::get_servo_min_angle_pulses);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Pulses for min angle", PROPERTY_HINT_RANGE, "0,4095"), "set_servo_min_angle_pulses", "get_servo_min_angle_pulses");

    ClassDB::bind_method(D_METHOD("set_servo_max_angle_pulses"), &I2CPCA9685::set_servo_max_angle_pulses);
    ClassDB::bind_method(D_METHOD("get_servo_max_angle_pulses"), &I2CPCA9685::get_servo_max_angle_pulses);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Pulses for max angle", PROPERTY_HINT_RANGE, "0,4095"), "set_servo_max_angle_pulses", "get_servo_max_angle_pulses");
    /**/

    ClassDB::bind_method(D_METHOD("set_servo0_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo0_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo0_euler_angle"), &I2CPCA9685::get_servo0_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 0", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo0_euler_angle", "get_servo0_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo1_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo1_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo1_euler_angle"), &I2CPCA9685::get_servo1_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 1", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo1_euler_angle", "get_servo1_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo2_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo2_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo2_euler_angle"), &I2CPCA9685::get_servo2_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 2", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo2_euler_angle", "get_servo2_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo3_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo3_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo3_euler_angle"), &I2CPCA9685::get_servo3_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 3", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo3_euler_angle", "get_servo3_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo4_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo4_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo4_euler_angle"), &I2CPCA9685::get_servo4_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 4", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo4_euler_angle", "get_servo4_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo5_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo5_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo5_euler_angle"), &I2CPCA9685::get_servo5_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 5", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo5_euler_angle", "get_servo5_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo6_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo6_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo6_euler_angle"), &I2CPCA9685::get_servo6_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 6", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo6_euler_angle", "get_servo6_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo7_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo7_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo7_euler_angle"), &I2CPCA9685::get_servo7_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 7", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo7_euler_angle", "get_servo7_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo8_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo8_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo8_euler_angle"), &I2CPCA9685::get_servo8_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 8", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo8_euler_angle", "get_servo8_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo9_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo9_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo9_euler_angle"), &I2CPCA9685::get_servo9_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 9", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo9_euler_angle", "get_servo9_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo10_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo9_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo10_euler_angle"), &I2CPCA9685::get_servo9_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 10", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo10_euler_angle", "get_servo10_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo11_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo11_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo11_euler_angle"), &I2CPCA9685::get_servo11_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 11", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo11_euler_angle", "get_servo11_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo12_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo12_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo12_euler_angle"), &I2CPCA9685::get_servo12_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 12", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo12_euler_angle", "get_servo12_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo13_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo13_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo13_euler_angle"), &I2CPCA9685::get_servo13_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 13", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo13_euler_angle", "get_servo13_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo14_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo14_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo14_euler_angle"), &I2CPCA9685::get_servo14_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 14", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo14_euler_angle", "get_servo14_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo15_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo15_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo15_euler_angle"), &I2CPCA9685::get_servo15_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "Servo 15", PROPERTY_HINT_RANGE, "-90.0,90.0"), "set_servo15_euler_angle", "get_servo15_euler_angle");



    // The led values.
    ADD_GROUP("LEDs", "");
    ClassDB::bind_method(D_METHOD("set_led0_value", "led_value"), &I2CPCA9685::set_led0_value);
	ClassDB::bind_method(D_METHOD("get_led0_value"), &I2CPCA9685::get_led0_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 0", PROPERTY_HINT_NONE, ""), "set_led0_value", "get_led0_value");

    ClassDB::bind_method(D_METHOD("set_led1_value", "led_value"), &I2CPCA9685::set_led1_value);
	ClassDB::bind_method(D_METHOD("get_led1_value"), &I2CPCA9685::get_led1_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 1", PROPERTY_HINT_NONE, ""), "set_led1_value", "get_led1_value");
    
    ClassDB::bind_method(D_METHOD("set_led2_value", "led_value"), &I2CPCA9685::set_led2_value);
	ClassDB::bind_method(D_METHOD("get_led2_value"), &I2CPCA9685::get_led2_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 2", PROPERTY_HINT_NONE, ""), "set_led2_value", "get_led2_value");
    
    ClassDB::bind_method(D_METHOD("set_led3_value", "led_value"), &I2CPCA9685::set_led3_value);
	ClassDB::bind_method(D_METHOD("get_led3_value"), &I2CPCA9685::get_led3_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 3", PROPERTY_HINT_NONE, ""), "set_led3_value", "get_led3_value");
    
    ClassDB::bind_method(D_METHOD("set_led4_value", "led_value"), &I2CPCA9685::set_led4_value);
	ClassDB::bind_method(D_METHOD("get_led4_value"), &I2CPCA9685::get_led4_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 4", PROPERTY_HINT_NONE, ""), "set_led4_value", "get_led4_value");
    
    ClassDB::bind_method(D_METHOD("set_led5_value", "led_value"), &I2CPCA9685::set_led5_value);
	ClassDB::bind_method(D_METHOD("get_led5_value"), &I2CPCA9685::get_led5_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 5", PROPERTY_HINT_NONE, ""), "set_led5_value", "get_led5_value");
    
    ClassDB::bind_method(D_METHOD("set_led6_value", "led_value"), &I2CPCA9685::set_led6_value);
	ClassDB::bind_method(D_METHOD("get_led6_value"), &I2CPCA9685::get_led6_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 6", PROPERTY_HINT_NONE, ""), "set_led6_value", "get_led6_value");
    
    ClassDB::bind_method(D_METHOD("set_led7_value", "led_value"), &I2CPCA9685::set_led7_value);
	ClassDB::bind_method(D_METHOD("get_led7_value"), &I2CPCA9685::get_led7_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 7", PROPERTY_HINT_NONE, ""), "set_led7_value", "get_led7_value");
    
    ClassDB::bind_method(D_METHOD("set_led8_value", "led_value"), &I2CPCA9685::set_led8_value);
	ClassDB::bind_method(D_METHOD("get_led8_value"), &I2CPCA9685::get_led8_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 8", PROPERTY_HINT_NONE, ""), "set_led8_value", "get_led8_value");
    
    ClassDB::bind_method(D_METHOD("set_led9_value", "led_value"), &I2CPCA9685::set_led9_value);
	ClassDB::bind_method(D_METHOD("get_led9_value"), &I2CPCA9685::get_led9_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 9", PROPERTY_HINT_NONE, ""), "set_led9_value", "get_led9_value");
    
    ClassDB::bind_method(D_METHOD("set_led10_value", "led_value"), &I2CPCA9685::set_led10_value);
	ClassDB::bind_method(D_METHOD("get_led10_value"), &I2CPCA9685::get_led10_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 10", PROPERTY_HINT_NONE, ""), "set_led10_value", "get_led10_value");
    
    ClassDB::bind_method(D_METHOD("set_led11_value", "led_value"), &I2CPCA9685::set_led11_value);
	ClassDB::bind_method(D_METHOD("get_led11_value"), &I2CPCA9685::get_led11_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 11", PROPERTY_HINT_NONE, ""), "set_led11_value", "get_led11_value");
    
    ClassDB::bind_method(D_METHOD("set_led12_value", "led_value"), &I2CPCA9685::set_led12_value);
	ClassDB::bind_method(D_METHOD("get_led12_value"), &I2CPCA9685::get_led12_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 12", PROPERTY_HINT_NONE, ""), "set_led12_value", "get_led12_value");
    
    ClassDB::bind_method(D_METHOD("set_led13_value", "led_value"), &I2CPCA9685::set_led13_value);
	ClassDB::bind_method(D_METHOD("get_led13_value"), &I2CPCA9685::get_led13_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 13", PROPERTY_HINT_NONE, ""), "set_led13_value", "get_led13_value");
    
    ClassDB::bind_method(D_METHOD("set_led14_value", "led_value"), &I2CPCA9685::set_led14_value);
	ClassDB::bind_method(D_METHOD("get_led14_value"), &I2CPCA9685::get_led14_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 14", PROPERTY_HINT_NONE, ""), "set_led14_value", "get_led14_value");
    
    ClassDB::bind_method(D_METHOD("set_led15_value", "led_value"), &I2CPCA9685::set_led15_value);
	ClassDB::bind_method(D_METHOD("get_led15_value"), &I2CPCA9685::get_led15_value);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 15", PROPERTY_HINT_NONE, ""), "set_led15_value", "get_led15_value");
    
    //ClassDB::bind_method(D_METHOD("set_led0_value", "led_value"), &I2CPCA9685::set_led0_value);
	//ClassDB::bind_method(D_METHOD("get_led0_value"), &I2CPCA9685::get_led0_value);
    //ADD_PROPERTY(PropertyInfo(Variant::INT, "LED 0", PROPERTY_HINT_NONE, ""), "set_led0_value", "get_led0_value");

    /*
    BIND_ENUM_CONSTANT(MODE1);
    BIND_ENUM_CONSTANT(MODE2);
    BIND_ENUM_CONSTANT(SUBADDR1);
    BIND_ENUM_CONSTANT(SUBADDR2);
    BIND_ENUM_CONSTANT(SUBADDR3);
    BIND_ENUM_CONSTANT(BROADCAST);
    BIND_ENUM_CONSTANT(SLOT0);
    BIND_ENUM_CONSTANT(SLOT0_ON_L);
    BIND_ENUM_CONSTANT(SLOT0_ON_H);
    BIND_ENUM_CONSTANT(SLOT0_OFF_L);
    BIND_ENUM_CONSTANT(SLOT0_OFF_H);
    BIND_ENUM_CONSTANT(PRESCALE); 
    /**/
}




void I2CPCA9685::_process(double delta) {
    if( !_is_pca9685_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;

    // We update at the same hz as the servo's are running
    _pca9685_update_wait_time -= delta;
    if( _pca9685_update_wait_time > 0.0 ) return;

    // Time to update the servo values.
    _pca9685_update_wait_time = _pca9685_update_frame_delay;
    float angle_multiplier = 1.0f / 180.0f;
    for( int i = 0; i < 16; ++i ) {
        
        // Degrees go between -90.0 and 90.0, so scale
        // to 0..1 first.
        float angle = servo_angles[i];
        if( angle < -90.0f ) { 
            angle = -90.0f; 
        }else if( angle > 90.0f ) {
            angle = 90.0f;
        }
        angle += 90.0f;
        angle *= angle_multiplier;
        int off_index = (int)((float)_servo_pulses_between_min_max_angles * angle);

        set_led_pulse_range( i, 0, _servo_min_angle_pulses + off_index );
    }
}


void I2CPCA9685::_physics_process(double delta) {

}


void I2CPCA9685::_notification(int p_what) {

}


void I2CPCA9685::update_servo_min_max_angle_pulse_counts() {
    float one_over_hz = 1000.0f / (float)_pwm_frequency_hz;
    float ms_to_pulses_multiplier = 4096.0f / one_over_hz; //4096.0f * one_over_hz;
    _servo_min_angle_pulses = (int)(ms_to_pulses_multiplier);
    _servo_max_angle_pulses = (int)(2.0f * ms_to_pulses_multiplier);
    _servo_pulses_between_min_max_angles = _servo_max_angle_pulses - _servo_min_angle_pulses;

    // Also update the update frame delays.
    _pca9685_update_frame_delay = (double)one_over_hz;
    _pca9685_update_wait_time = 0.0;
}

void I2CPCA9685::set_pwm_frequency_hz( int new_frequency_hz ) {
    _pwm_frequency_hz = new_frequency_hz;
    if( _pwm_frequency_hz > 1000 ) {
        _pwm_frequency_hz = 1000;
    }
    if( _pwm_frequency_hz < 1 ) {
        _pwm_frequency_hz = 1;
    }
    if(Engine::get_singleton()->is_editor_hint()) return;
    if( _i2c_device_fd < 0 ) return; // Device not opened so cannot do this yet. Will be set on init in any case.

    // Send to the device if not in editor.
    int prescaling = (int)(25000000.0f / (4096.0f * (float)_pwm_frequency_hz) - 0.5f);
    update_servo_min_max_angle_pulse_counts();

    uint8_t current_state = read_byte_from_device_register(PCA9685Registers::MODE1);
    uint8_t sleep = current_state | PCA9685Mode::SLEEP;
    uint8_t wake = current_state & ~(PCA9685Mode::SLEEP);
    uint8_t restart = wake | 0x80;

    write_byte_to_device_register(PCA9685Registers::MODE1, sleep);
    write_byte_to_device_register(PCA9685Registers::PRESCALE, prescaling);
    write_byte_to_device_register(PCA9685Registers::MODE1, wake);
    // todo: delay at least 500 ms

    write_byte_to_device_register(PCA9685Registers::MODE1, restart );
}

int  I2CPCA9685::get_pwm_frequency_hz() const {
    return _pwm_frequency_hz;
}


void I2CPCA9685::_initialize_device() {
    // Only initialize once.
    if( _is_pca9685_initialized ) return;
    if(Engine::get_singleton()->is_editor_hint()) return;

    open_device();
    ERR_FAIL_COND_MSG(_i2c_device_fd < 0, "PCA9685 initialization failed because the device file is not opened.");
    

    _is_pca9685_initialized = true;

    // Read the current MODE1 register value.
    uint8_t current_state = read_byte_from_device_register(PCA9685Registers::MODE1);


    // First set the PWM Frequency as set on the node.
    uint8_t prescale_value = (int)(25000000.0f / (4096.0f * (float)(_pwm_frequency_hz)) - 0.5f);
    update_servo_min_max_angle_pulse_counts();

    // Device must sleep to change the setting.
    current_state |= PCA9685Mode::SLEEP;
    write_byte_to_device_register( PCA9685Registers::MODE1, PCA9685Mode::SLEEP);
    write_byte_to_device_register( PCA9685Registers::PRESCALE, prescale_value);
    write_byte_to_device_register( PCA9685Registers::MODE1, PCA9685Mode::RESTART);
    
    // Create a timer that will then finalize the 
    // initialization.
    Timer* waittimer = memnew(Timer);//::_new();
    add_child(waittimer);
    waittimer->set_wait_time(0.5);
    waittimer->set_one_shot(true);
    waittimer->connect("timeout", godot::Callable(this, "on_timer_finished_finalize_initialize"));
     // callable_mp(this, &I2CPCA9685::on_timer_finished_finalize_initialize ));
    //"on_timer_finished_finalize_initialize");
    
    waittimer->start();
    //delay(1); 
    
}

void I2CPCA9685::on_timer_finished_finalize_initialize() {
    uint8_t current_state = read_byte_from_device_register(PCA9685Registers::MODE1);
    current_state &= ~(PCA9685Mode::SLEEP); // Clear the sleep-bit.
    write_byte_to_device_register( PCA9685Registers::MODE1, current_state);
    //ERR_FAIL_COND_MSG(true, "Delayed initialization done.");

}


void I2CPCA9685::wake_pca9685() {
    // Wake by getting the current state, then clearing the 
    // sleep-bit, and setting the state back.
    uint8_t current_state = read_byte_from_device_register(PCA9685Registers::MODE1);
    current_state &= ~(PCA9685Mode::SLEEP); 
    write_byte_to_device_register( PCA9685Registers::MODE1, current_state);
}

void I2CPCA9685::sleep_pca9685() {
    // Sleep by getting the current state, then setting the
    // sleep-bit, and setting the state back.
    uint8_t current_state = read_byte_from_device_register(PCA9685Registers::MODE1);
    current_state |= (PCA9685Mode::SLEEP); 
    write_byte_to_device_register( PCA9685Registers::MODE1, current_state);
}

void I2CPCA9685::restart_pca9685() {
    write_byte_to_device_register( PCA9685Registers::MODE1, PCA9685Mode::RESTART);
}


// on-off indices are for pwm singal steps from 0 to 4095.
void I2CPCA9685::set_led_pulse_range( int led_index, int on_index, int off_index ) {
    ERR_FAIL_COND_MSG(led_index < 0 || led_index > 15, "set_led_pulse_range: Led index out of bounds.");
    uint8_t uled_index = (uint8_t)led_index * 4;
    uint8_t uon_index = (uint8_t)on_index;
    uint8_t uoff_index = (uint8_t)off_index;

    write_byte_to_device_register(PCA9685Registers::LED0_ON_L + uled_index, uon_index & 0xFF );
    write_byte_to_device_register(PCA9685Registers::LED0_ON_H + uled_index, (uon_index >> 8) & 0x0F );
    write_byte_to_device_register(PCA9685Registers::LED0_OFF_L + uled_index, uoff_index & 0xFF );
    write_byte_to_device_register(PCA9685Registers::LED0_OFF_H + uled_index, (uoff_index >> 8) & 0x0F );
    
}

// Getters and setters for servos.
/**
void I2CPCA9685::set_servo_min_angle_pulses( const int min_angle_pulses ) {
    _servo_min_angle_pulses = min_angle_pulses;
}

int  I2CPCA9685::get_servo_min_angle_pulses() const {
    return _servo_min_angle_pulses;
}

void I2CPCA9685::set_servo_max_angle_pulses( const int max_angle_pulses ) {
    _servo_max_angle_pulses = max_angle_pulses;
}

int  I2CPCA9685::get_servo_max_angle_pulses() const {
    return _servo_max_angle_pulses;
}
/**/


void I2CPCA9685::set_servo_euler_angle( const int servo_index, const float new_euler_angle ) {
    ERR_FAIL_COND_MSG(servo_index < 0 || servo_index > 15, "set_servo_euler_angle: Servo index out of bounds.");
    servo_angles[servo_index] = new_euler_angle;
    
    
    return;
    if(Engine::get_singleton()->is_editor_hint()) return;

    /**
    // Degrees go between -90.0 and 90.0, so scale
    // to 0..1 first.
    float angle = new_euler_angle;
    if( angle < -90.0f ) { 
        angle = -90.0f; 
    }else if( angle > 90.0f ) {
        angle = 90.0f;
    }
    angle += 90.0f;
    angle /= 180.0f;
    //int servo_pulse_range = _servo_max_angle_pulses - _servo_min_angle_pulses;

    int off_index = (int)((float)_servo_pulses_between_min_max_angles * angle);
    //if( off_index > _servo_max_angle_pulses ) {
    //    off_index = _servo_max_angle_pulses;
    //}

    set_led_pulse_range( servo_index, 0, _servo_min_angle_pulses + off_index );
    /**/
}

void I2CPCA9685::set_servo0_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(0, new_euler_angle);
}

void I2CPCA9685::set_servo1_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(1, new_euler_angle);
}
void I2CPCA9685::set_servo2_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(2, new_euler_angle);
}
void I2CPCA9685::set_servo3_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(3, new_euler_angle);
}
void I2CPCA9685::set_servo4_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(4, new_euler_angle);
}
void I2CPCA9685::set_servo5_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(5, new_euler_angle);
}
void I2CPCA9685::set_servo6_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(6, new_euler_angle);
}
void I2CPCA9685::set_servo7_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(7, new_euler_angle);
}
void I2CPCA9685::set_servo8_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(8, new_euler_angle);
}
void I2CPCA9685::set_servo9_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(9, new_euler_angle);
}
void I2CPCA9685::set_servo10_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(10, new_euler_angle);
}
void I2CPCA9685::set_servo11_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(11, new_euler_angle);
}
void I2CPCA9685::set_servo12_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(12, new_euler_angle);
}
void I2CPCA9685::set_servo13_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(13, new_euler_angle);
}
void I2CPCA9685::set_servo14_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(14, new_euler_angle);
}
void I2CPCA9685::set_servo15_euler_angle( const float new_euler_angle ) {
    set_servo_euler_angle(15, new_euler_angle);
}


float I2CPCA9685::get_servo_euler_angle( const int servo_index ) const {
    if( servo_index < 0 ) return 0.0f;
    if( servo_index > 15 ) return 0.0f;
    return servo_angles[servo_index];    
}

float I2CPCA9685::get_servo0_euler_angle() const {
    return servo_angles[0];
}

float I2CPCA9685::get_servo1_euler_angle() const {
    return servo_angles[1];
}

float I2CPCA9685::get_servo2_euler_angle() const {
    return servo_angles[2];
}

float I2CPCA9685::get_servo3_euler_angle() const {
    return servo_angles[3];
}

float I2CPCA9685::get_servo4_euler_angle() const {
    return servo_angles[4];
}

float I2CPCA9685::get_servo5_euler_angle() const {
    return servo_angles[5];
}

float I2CPCA9685::get_servo6_euler_angle() const {
    return servo_angles[6];
}

float I2CPCA9685::get_servo7_euler_angle() const {
    return servo_angles[7];
}

float I2CPCA9685::get_servo8_euler_angle() const {
    return servo_angles[8];
}

float I2CPCA9685::get_servo9_euler_angle() const {
    return servo_angles[9];
}

float I2CPCA9685::get_servo10_euler_angle() const {
    return servo_angles[10];
}

float I2CPCA9685::get_servo11_euler_angle() const {
    return servo_angles[11];
}

float I2CPCA9685::get_servo12_euler_angle() const {
    return servo_angles[12];
}

float I2CPCA9685::get_servo13_euler_angle() const {
    return servo_angles[13];
}

float I2CPCA9685::get_servo14_euler_angle() const {
    return servo_angles[14];
}

float I2CPCA9685::get_servo15_euler_angle() const {
    return servo_angles[15];
}



// Getters and setters for leds.

int I2CPCA9685::get_led0_value(){
    return 0;
}

int I2CPCA9685::get_led1_value(){
    return 0;
}

int I2CPCA9685::get_led2_value(){
    return 0;
}

int I2CPCA9685::get_led3_value(){
    return 0;
}

int I2CPCA9685::get_led4_value(){
    return 0;
}

int I2CPCA9685::get_led5_value(){
    return 0;
}

int I2CPCA9685::get_led6_value(){
    return 0;
}

int I2CPCA9685::get_led7_value(){
    return 0;
}

int I2CPCA9685::get_led8_value(){
    return 0;
}

int I2CPCA9685::get_led9_value(){
    return 0;
}

int I2CPCA9685::get_led10_value(){
    return 0;
}

int I2CPCA9685::get_led11_value(){
    return 0;
}

int I2CPCA9685::get_led12_value(){
    return 0;
}

int I2CPCA9685::get_led13_value(){
    return 0;
}

int I2CPCA9685::get_led14_value(){
    return 0;
}

int I2CPCA9685::get_led15_value(){
    return 0;
}


void I2CPCA9685::set_led_value(int led_index, int new_led_value ) {
    write_byte_to_device_register(PCA9685Registers::LED0 + led_index * 4, new_led_value );

}

void I2CPCA9685::set_led0_value(int new_led_value){

}

void I2CPCA9685::set_led1_value(int new_led_value){

}

void I2CPCA9685::set_led2_value(int new_led_value){

}

void I2CPCA9685::set_led3_value(int new_led_value){

}

void I2CPCA9685::set_led4_value(int new_led_value){

}

void I2CPCA9685::set_led5_value(int new_led_value){

}

void I2CPCA9685::set_led6_value(int new_led_value){

}

void I2CPCA9685::set_led7_value(int new_led_value){

}

void I2CPCA9685::set_led8_value(int new_led_value){

}

void I2CPCA9685::set_led9_value(int new_led_value){

}

void I2CPCA9685::set_led10_value(int new_led_value){

}

void I2CPCA9685::set_led11_value(int new_led_value){

}

void I2CPCA9685::set_led12_value(int new_led_value){

}

void I2CPCA9685::set_led13_value(int new_led_value){

}

void I2CPCA9685::set_led14_value(int new_led_value){

}

void I2CPCA9685::set_led15_value(int new_led_value){

}

