**# Automated RGB Color Sorting System**



An automated sorting system designed to detect and sort candies (specifically skittles) into designated bins by color using an Arduino, two servo motors, an RGB sensor, and an audio notification buzzer that notifies the user when sorting is complete.



**## Features**



\- **TCS34725 RGB Color Sensing:** Measures normalized RGB values under calibrated lighting.

\- **Servo 1:** Positions mechanical slide for skittles to slide down into correct bin out of 5 possible positions that each correspond to a color. 

\- **Servo 2:** Handles a 3-stage mechanical transfer sequence for each candy (Pick up, Detect color, Bin route).

\- **Automatic Shutdown and Audio Alert:** Detects when system is empty when the RGB sensor picks up the color blue (background under where candy color is typically detected when full) for 3 consecutive cycles and initiates buzzer alert then shuts down.



**## Hardware**



\- **Microcontroller:** Arduino Uno

\- **Sensor:** Adafruit TCS34725 RGB Color Sensor

\- **Actuators:** 2x Servo Motors (Pins D3, D5)

\- **Audio Output:** Piezoelectric Buzzer (Pin D6)

\- **Power:** Directly Power Arduino or use 9V battery



**## Software \& Dependencies**



1\. `Wire.h`

2\. `Servo.h`

3\. `Adafruit TCS34725`



**## Notes**



\- Serial Monitor displays real time RGB values and color detection logs.

\- Servos are attached to custom built 3D printed and machined parts.

