#include "I2CPCA9685.h"

#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/engine.hpp>


using namespace godot;

I2CPCA9685::I2CPCA9685() {
    _pwm_frequency_hz = 50; 
    _is_pca9685_initialized = false;

    for( int i = 0; i < 16; ++i ) {
        servo_angles[i] = 0.0f;
    }
}


I2CPCA9685::~I2CPCA9685() {

}



void I2CPCA9685::_bind_methods() {
    /**
    // Device ID on the SCB.
    ClassDB::bind_method(D_METHOD("set_i2c_device_index", "device_index"), &I2CRawDevice::set_i2c_device_index);
	ClassDB::bind_method(D_METHOD("get_i2c_device_index"), &I2CRawDevice::get_i2c_device_index);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "I2C Device", PROPERTY_HINT_NONE, ""), "set_i2c_device_index", "get_i2c_device_index");

    // Address.
    ClassDB::bind_method(D_METHOD("set_i2c_device_address", "device_address"), &I2CRawDevice::set_i2c_device_address);
	ClassDB::bind_method(D_METHOD("get_i2c_device_address"), &I2CRawDevice::get_i2c_device_address);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "I2C Device Address", PROPERTY_HINT_NONE, ""), "set_i2c_device_address", "get_i2c_device_address");

    // Read and write methods.
    ClassDB::bind_method(D_METHOD("open_device"), &I2CRawDevice::open_device);
    ClassDB::bind_method(D_METHOD("close_device"), &I2CRawDevice::close_device);
    ClassDB::bind_method(D_METHOD("read_bytes_from_device", "num_bytes"), &I2CRawDevice::read_bytes_from_device);
    ClassDB::bind_method(D_METHOD("write_bytes_to_device", "bytes_to_write"), &I2CRawDevice::write_bytes_to_device);
    /**/

    // Initialization function.
    //ClassDB::bind_method(D_METHOD("initialize_device"), &I2CPCA9685::initialize_device);

    // Initialization timeout helper.
    ClassDB::bind_method(D_METHOD("on_timer_finished_finalize_initialize"), &I2CPCA9685::on_timer_finished_finalize_initialize);   

    ClassDB::bind_method(D_METHOD("set_pwm_frequency_hz"), &I2CPCA9685::set_pwm_frequency_hz);
    ClassDB::bind_method(D_METHOD("get_pwm_frequency_hz"), &I2CPCA9685::get_pwm_frequency_hz);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "PWM Frequency Hz", PROPERTY_HINT_RANGE, "40,1000"), "set_pwm_frequency_hz", "get_pwm_frequency_hz");


    // The servo values.
    ClassDB::bind_method(D_METHOD("set_servo_euler_angle", "servo_index", "new_euler_angle"), &I2CPCA9685::set_servo_euler_angle);
    ClassDB::bind_method(D_METHOD("get_servo_euler_angle", "servo_index"), &I2CPCA9685::get_servo_euler_angle);


    ADD_GROUP("Servos", "");
    ClassDB::bind_method(D_METHOD("set_servo0_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo0_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo0_euler_angle"), &I2CPCA9685::get_servo0_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Servo 0", PROPERTY_HINT_NONE, ""), "set_servo0_euler_angle", "get_servo0_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo1_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo1_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo1_euler_angle"), &I2CPCA9685::get_servo1_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Servo 1", PROPERTY_HINT_NONE, ""), "set_servo1_euler_angle", "get_servo1_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo2_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo2_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo2_euler_angle"), &I2CPCA9685::get_servo2_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Servo 2", PROPERTY_HINT_NONE, ""), "set_servo2_euler_angle", "get_servo2_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo3_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo3_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo3_euler_angle"), &I2CPCA9685::get_servo3_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Servo 3", PROPERTY_HINT_NONE, ""), "set_servo3_euler_angle", "get_servo3_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo4_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo4_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo4_euler_angle"), &I2CPCA9685::get_servo4_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Servo 4", PROPERTY_HINT_NONE, ""), "set_servo4_euler_angle", "get_servo4_euler_angle");

    ClassDB::bind_method(D_METHOD("set_servo5_euler_angle", "new_euler_angle"), &I2CPCA9685::set_servo5_euler_angle);
	ClassDB::bind_method(D_METHOD("get_servo5_euler_angle"), &I2CPCA9685::get_servo5_euler_angle);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "Servo 5", PROPERTY_HINT_NONE, ""), "set_servo5_euler_angle", "get_servo5_euler_angle");


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

}


void I2CPCA9685::_physics_process(double delta) {

}


void I2CPCA9685::_notification(int p_what) {

}



void I2CPCA9685::set_pwm_frequency_hz( int new_frequency_hz ) {
    _pwm_frequency_hz = new_frequency_hz;
    if( _pwm_frequency_hz > 1000 ) {
        _pwm_frequency_hz = 1000;
    }
    if( _pwm_frequency_hz < 40 ) {
        _pwm_frequency_hz = 40;
    }
    if(Engine::get_singleton()->is_editor_hint()) return;
    if( _i2c_device_fd < 0 ) return; // Device not opened so cannot do this yet. Will be set on init in any case.

    // Send to the device if not in editor.
    int prescaling = (int)(25000000.0f / (4096 * _pwm_frequency_hz) - 0.5f);

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
    // Device must sleep to change the setting.
    current_state |= PCA9685Mode::SLEEP;
    write_byte_to_device_register( PCA9685Registers::MODE1, PCA9685Mode::SLEEP);
    write_byte_to_device_register( PCA9685Registers::PRESCALE, prescale_value);
    write_byte_to_device_register( PCA9685Registers::MODE1, PCA9685Mode::RESTART);
    
    // Create a timer that will then finalize the 
    // initialization.
    Timer* waittimer = memnew(Timer);//::_new();
    add_child(waittimer);
    waittimer->set_wait_time(1);
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
    ERR_FAIL_COND_MSG(true, "Delayed initialization done.");

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


void I2CPCA9685::set_servo_euler_angle( const int servo_index, const float new_euler_angle ) {
    ERR_FAIL_COND_MSG(servo_index < 0 || servo_index > 15, "set_servo_euler_angle: Servo index out of bounds.");
    
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

    int off_index = (int)(500.0f * angle);
    if( off_index > 500 ) {
        off_index = 500;
    }

    set_led_pulse_range( servo_index, 150, 150 + off_index );
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

