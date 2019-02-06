# DKRZero

An embedded Arduino library for an open source accelerometer, based on the ESP32 Thing and Motion Shield. 

# Documentation

The documentation is hosted on .

# Installation and Dependencies

## Python (for streaming mode only)

Please install Python 3 on your system. Then, in order to use the units in streaming mode, please install 
the libraries numpy and pyserial. 

### Linux Python Libraries Installation 

Type the following command within a terminal 

sudo pip install numpy pyserial

### Windows 

python -m pip install numpy pyserial

python esp32_receiver.py

## Arduino

Download the latest Arduino integrated development environment (IDE) from https://www.arduino.cc/en/main/software 

Install the ESP32 boards into boards manager using the instructions here: https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md

Install the ArduinoJSON library by opening the boards manager window in the Arduino IDE (Sketch->Include Library->Manage Libraries) and search for ArduinoJSON. Then click on the ArduinoJSON and then on install. Alternatively, the library is located here: https://github.com/bblanchon/ArduinoJson. 




# Attribution

# License

See the AUTHORS.rst file for a complete list of contributors to the project.
