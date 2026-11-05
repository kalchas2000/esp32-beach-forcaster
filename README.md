# ESP32-C6 TFT Display Project
A local beach weather forecaster that displays live graphics and accurate 24-hour predictions for wind and temperature, connecting to WiFi via a simple mobile portal.

## Hardware Used
* **ESP32-C6-DevKitC-1 V1.2**[cite: 3, 4]
* **Adafruit 2.8" TFT LCD shield w/Touchscreen (v2.0)**[cite: 1, 7]
* Assorted jumper wires[cite: 1, 2]

## Wiring Connections
Because this uses a shield designed for an Arduino, standard jumper wires are used to bridge the connections to the ESP32[cite: 1].
The pins mentioned in the Adafruit TFT Shield Pins are at the lots where the arduino would have been so the name of the pins follow that of the arduino.
Here is the pin mapping:

ESP32-C6 Pin -> Adafruit TFT Shield Pin                                                                                                                                                                                                                                                                                          
4 -> RST                
5V -> 5V                                                                                                                                                                                                                                                                                                                         
G -> GND                                                                                                                                                                                                                                                                                                                         
6 -> 13                                                                                                                                                                                                                                                                                                                        
7 -> 11                                                                                                                                                                                                                                                                                                                         
1 -> 12                                                                                                                                                                                                                                                                                                                         
18 -> 10                                                                                                                                                                                                                                                                                                                         
10 -> 9                                                                                                                                                                                                                                                                                                                         
19 -> 8                                                                                                                                                                                                                                                                                                                         


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

##Case
A 3d case is also available now on stl form and is 3d printer ready with a big opening on the back for easy install of both the screen and the esp32 contoller inside of the case.I personally used glue to stick both the Screen and the lid on their final positions.

##Notes
Here I have uploaded some of the final results of my project fully working with the custom 3d printed case.
<img width="3024" height="4032" alt="IMG_0441" src="https://github.com/user-attachments/assets/5e46118a-e8fa-4f7b-b5c7-c6784a83c712" />
<img width="3024" height="4032" alt="IMG_0442" src="https://github.com/user-attachments/assets/a9a614a4-7086-4c9e-9ec0-22be806862e4" />
<img width="6048" height="8064" alt="IMG_0443" src="https://github.com/user-attachments/assets/00e0c5fc-482b-4ac9-ae8c-0c87f22ea3f8" />

