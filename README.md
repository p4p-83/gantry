# Gantry driver firmware

This is the firmware for a Raspberry Pi Pico. This connects to the main Raspberry Pi 5 board via USB to take in instructions as to how to move the mechanical parts. It then commands the various stepper drivers and manages limit switches. It also provides a few auxiliary outputs as required for the purpose of switching the vacuum feed etc.

The principle of operation is simple. This firmware divides time into fixed slices (the length of which is still to be determined).
- The code will at any point of time accept input from USB specifying a certain number of steps. This sets the number of steps in each direction to be made in the *next* (not yet started) time slice.
- At the conclusion of a time slice, the code will output over USB the number of steps successfully taken in that previous time slot.
- The firmware will manage each individual step during the current time slot. It will count the number of steps as commanded of each driver. It will stop immediately if a limit switch is encountered.

The time slot method makes the operation of the firmware more predictable from a control system theory and simplifies the code and understanding of it somewhat.

The firmware does not directly support homing. Instead, you should continue issuing move commands at each time slot until the firmware return value indicates no further steps are being taken. More advanced homing routines are entirely implementable on the controlling Pi 5.

This firmware has no concept of absolute position outside of its respect for the limit switches — it only responds to the relative movement commands documented above. Therefore it is entirely on the Pi 5's code to integrate the return values from this code to keep any desired state.

This architecture has been chosen to make this firmware as simple as possible and provide the greatest flexibility to the code on the Pi 5, while removing the burden of sending all of the individual step pulses.

## Building this project

This is managed by CMake. Set up for building with (as an example):

```sh
# start in the same dir as this README
mkdir build	# but you can choose the name
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

Run this once when you start, and each time you change the dependencies or file structure or otherwise modify CMakeLists.txt.

When you wish to build the project:

```sh
cd build # assuming you aren't already in the build dir
make
```