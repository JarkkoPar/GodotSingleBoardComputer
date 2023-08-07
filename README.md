# GodotSingleBoardComputer
A set of nodes to interface with IO pins of a SingleBoardComputer (SBC) using the Godot Engine. This extension is intended to be used with Linux.

Currently supported SBCs:
 - Radxa Rock 4 b
 - Radxa Rock 4 SE 


## Usage 

Copy the extension to your godot project bin-folder and you should be set. 

Add the SingleBoardComputer node to your scene, select the board you are using from the supported ones, and under the SingleBoardComputer node add any of the device nodes you need. Then configure each device based on your specs and assign them the correct device or pin index, addresses, etc.

As SBC's have limited cpu and memory resources, it is recommended that you do the Godot development on your Linux laptop/desktop and then copy over the files to your SBC. This will greatly speed up your work in the Godot engine editor. 

To run your godot app on your SBC you can start it directly from the command line (assuming main.tscn is your main scene):

`sudo ./your-godot-executable main.tscn`

If you do not need the 2D/3D views from godot, you get more performance by running the project headless:

`sudo ./your-godot-executable main.tscn --headless`

If your SBC doesn't support Vulkan and you have developed on your Linux machine using the Forward+ renderer, you may need to specify your rendering driver as well:

`sudo ./your-godot-executable main.tscn --rendering-driver opengl3`

You need to use `sudo` for the nodes to be able to access the device files in Linux.

## Compiling

Set up your project as described in the godot documentation for the gdexample extension example. Copy all the code from this repository in to the src folder, then copy the `_SConstruct` file found in this repository to the gdextension folder and rename it to `SConstruct`. Finally copy the libsingleboardcomputer.gdextension file to the demo/bin folder. Compile with `scons platform=linux` command as usual.


## The top level nodes 

### SingleBoardComputer 

The main node that is needed to interface with any device. You must choose which SBC you are using in the Board-dropdown in the inspector. This will allow the initialization of all the relevant interfaces when your project is run.

### SBCDevice 

The base device node, not meant to be used in scenes. Unifies the initialization and shutdown of the various device types.

### I2cDevice

The base node for I2C devices with generic interfaces to interact with an I2C device. Under this node you will find specific device nodes. All I2C devices need the index of the i2c device file and the address of the device. While you can type in the address as hexadecimal, it will show up in the inspector as an integer. The derived I2C device nodes will have additional properties depending on the device in question.

### AdcDevice 

The base node for Analog-to-digital converter devices built-in to the SBC. If there are ADC pins on the SBC, you can use the device nodes derived from this to interface with them. 

### SpiDevice 

The base node for SPI devices. Not implemented yet.


## I2cDevice nodes 

### I2cAds1115

A ADS1115 based device for interfacing with analog sensors using I2C. Currently in development.

Default I2C address 0x48.

### I2cPca9685

A PCA9685 based device for controlling leds or servo motors. Currently the servo controls are implemented for a 16 channel PCA9685 device. You can set the minimum and maximum ms for each of the 16 channels (which each correspond with a servo), and the angle for each servo. 

Default I2C address 0x40.

### I2cVl53l0x

A VL53L0X based distance sensor for measuring distances. Currently initializes automatically in to continuous (or back-to-back) reading mode. You can get the measured distance in millimeters from the `distance_mm` property of the node and in inches from the `distance_inch` property. Note that the maximum distance measured by the VL53L0X sensor is 2 meters (2000 millimeters or 78.7 inches). If the distance returned is greater than the maximum, it meas there is nothing found in range of the sensor.

Default I2C address 0x29.

## AdcDevice nodes 

### AdcTemperatureSensor

A temperature sensor that interfaces with an ADC built in to the SingleBoardComputer. While this just now returns values, they may be wrong.


## Helper nodes 

The helper nodes do not need to be child nodes of the SingleBoardComputer node.

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

