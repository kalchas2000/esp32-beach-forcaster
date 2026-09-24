# ESP32-C6 TFT Display Project

This project connects an ESP32 microcontroller to an Adafruit touchscreen display.

## Hardware Used
* **ESP32-C6-DevKitC-1 V1.2**[cite: 3, 4]
* **Adafruit 2.8" TFT LCD shield w/Touchscreen (v2.0)**[cite: 1, 7]
* Assorted jumper wires[cite: 1, 2]

## Wiring Connections
Because this uses a shield designed for an Arduino, standard jumper wires are used to bridge the connections to the ESP32[cite: 1].
The pins mentioned in the Adafruit TFT Shield Pins are at the lots where the arduino would have been so the name of the pins follow that of the arduino.
Here is the pin mapping:

| ESP32-C6 Pin | Adafruit TFT Shield Pin 
| RST          | RESET                   
| 5V           | 5V                      
| G            | GND                     
| 6            | 13                      
| 7            | 11
| 1            | 12
| 10           | 10
| 18           | 9 
| 19           | 8


## Hardware Setup Photos
Photos of the project in for wiring backup can also be found in the project a separate files.


## Dependencies
To run this code, you will need to install the following libraries via the Arduino IDE Library Manager:
* **Adafruit ILI9341** (Driver for the TFT display)[cite: 10]
* **Adafruit GFX Library** (Core graphics library)[cite: 10]
* **Adafruit BusIO** (Required hardware abstraction)[cite: 10]
* **Adafruit TouchScreen** (or **XPT2046_Touchscreen**, depending on your specific `.ino` code)[cite: 8, 9]

## How to Run
1. Open the `.ino` file in the Arduino IDE.
2. Verify that the libraries listed above are installed.
3. Select the ESP32-C6 board and the correct COM port from the tools menu.
4. Click Upload.

##Notes 
A 3d case is also coming because the first one was not successful.
