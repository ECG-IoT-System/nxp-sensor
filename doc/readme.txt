Overview
========
The i2c_read_accel_value example shows how to use I2C driver to communicate with an i2c device:

 1. How to use the i2c driver to read a i2c device who_am_I register.
 2. How to use the i2c driver to write/read the device registers.

In this example, the values of three-axis accelerometer print to the serial terminal on PC through
the virtual serial port on board.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- MCUXpresso10.0.2

Hardware requirements
=====================
- Mini/micro USB cable
- QN908XDK board
- Personal Computer

Board settings
==============
No special settings are required.
Use J2 as the default power supply port. (Use J1 as power supply port, accelerometer device will not be found)

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following message shows in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPI2C example -- Read Accelerometer Value
Found a MMA8652 on board , the device address is 0x1d . 
The accel values:
status_reg = 0xff , x =    12 , y =   -16 , z =  2060 
status_reg = 0xff , x =    20 , y =   -20 , z =  2056 
status_reg = 0xff , x =    20 , y =   -24 , z =  2052 
status_reg = 0xff , x =    20 , y =   -28 , z =  2056 
status_reg = 0xff , x =     4 , y =   -12 , z =  2064 
status_reg = 0xff , x =    16 , y =   -16 , z =  2040 
status_reg = 0xff , x =     8 , y =    -8 , z =  2052 
status_reg = 0xff , x =    16 , y =    -4 , z =  2052 
status_reg = 0xff , x =    16 , y =   -20 , z =  2056 
status_reg = 0xff , x =    36 , y =   -24 , z =  2084 

End of LPI2C example .
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

