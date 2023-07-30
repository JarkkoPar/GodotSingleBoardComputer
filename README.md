# GodotSingleBoardComputer
A set of nodes to interface with IO pins of a SingleBoardComputer (SBC) using the Godot Engine. This extension is intended to be used with Linux.

Currently supported SBCs:
 - Radxa Rock 4 b
 - Radxa Rock 4 SE 


## Usage 

Copy the extension to your godot project bin-folder and you should be set. 

Add the SingleBoardComputer node to your scene, select the board you are using from the supported ones, and under the SingleBoardComputer node add any of the device nodes you need. Then configure each device based on your specs and assign them the correct device or pin index, addresses, etc.


## The top level nodes 

### SingleBoardComputer 

The main node that is needed to interface with any device. You must choose which SBC you are using in the Board-dropdown in the inspector. This will allow the initialization of relevant interfaces when your project is run.

### SBCDevice 

The base device node, not meant to be used in scenes. Unifies the initialization and shutdown of the various device types.

### I2cDevice

The base node for I2C devices with generic interfaces to interact with an I2C device. Under this node you will find specific device nodes. All I2C devices need the index of the i2c device file and the address of the device. The derived nodes will have additional properties depending on the device in question.

### AdcDevice 

The base node for Analog-to-digital converter devices. If there are ADC pins on the SBC, you can use the device nodes derived from this to interface with them. 

### SpiDevice 

The base node for SPI devices. Not implemented yet.


## I2cDevice nodes 

### I2cPca9685

A PCA9685 based device for controlling leds or servo motors. Currently the servo controls are implemented for a 16 channel PCA9685 device. You can set the minimum and maximum ms for each of the 16 channels (which each correspond with a servo), and the angle for each servo. 


## AdcDevice nodes 

### AdcTemperatureSensor

A temperature sensor that interfaces with an ADC. While this just now returns values, they may be wrong.


## Helper nodes 

### RoboticIkJoint

A helper node to create robotic appendixes that are rotated using servos. Each joint is constrained to a single axis that is defined in the node inspector and can be set a minimum and maximum angle for rotation around the assigned axis. The center angle is set to 90 by default, and minimum and maximum to 0 and 180 respectively which seem to be defaults for many hobby servos.

Chains of joints are created by adding RoboticIkJoints as childs of each other. A final joint needs to be added as the tip by setting the value in the inspector. The tip-joint needs to be assigned with a target node which it will try and reach when running your godot project. The target node can be a Node3D or any other node derived from it.

As an example, to create a robotic arm with a rotating base and three joints do the following: 
 * Add a RoboticIkJoint as a base and name it BaseY, configure its axis as Vector3(0,1,0) which is rotation around the Y-axis
 * Add a RoboticIkJoint as a child of BaseY and name it BaseX, configure its axis as Vector3(1,0,0) which is rotation around the X-axis, set position to Vector3(0,0.05,0) which means 5 cm higher than the rotating base
 * Add a RoboticIkJoint as a child of BaseX and name it ElbowX, configure its axis as Vector3(1,0,0), set position to Vector3(0,0.1,0) which means 10 cm higher than the BaseX node
 * Add a RoboticIkJoint as a child of ElbowX and name it WristX, configure its axis as Vector3(1,0,0), set position to Vector3(0,0.05,0)
 * Add a RoboticIkJoint as a child of WristX and name it as TipJoint, check the Tip-checkbox, and set position to Vector3(0,0.05,0)

 By doing this you have configured an arm pointing straight up with multiple joints. To visualize your arm, you can add MeshInstances with a box mesh.

 To get the angles from the joints and to assign them with servos, you can use the joint angle property and assign it to the angles of the I2cPca9685 node, for example.

