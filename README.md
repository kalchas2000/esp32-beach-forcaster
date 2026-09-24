# ESP32-C6 TFT Display Project

This project connects an ESP32 microcontroller to an Adafruit touchscreen display.

## Hardware Used
* **ESP32-C6-DevKitC-1 V1.2**[cite: 3, 4]
* **Adafruit 2.8" TFT LCD shield w/Touchscreen (v2.0)**[cite: 1, 7]
* Assorted jumper wires[cite: 1, 2]

## Wiring Connections
Because this uses a shield designed for an Arduino, standard jumper wires are used to bridge the connections to the ESP32[cite: 1].
The pins mentoned in the Adafruit TFT Shield Pins are at the lots where the arduino would have been so the name of the pins follow that of the arduino.
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
<img width="3013" height="3807" alt="IMG_E0087" src="https://github.com/user-attachments/assets/79dbb7bc-580d-4549-9277-94af2627a43c" />
<img width="3021" height="3762" alt="IMG_E0084" src="https://github.com/user-attachments/assets/3ccc18f1-328a-4fd6-8626-cfea930b318f" />
<img width="2741" height="3972" alt="IMG_E0083" src="https://github.com/user-attachments/assets/597c7f6f-f22a-4914-bad7-47331bc75e3e" />
<img width="3024" height="4032" alt="IMG_0086" src="https://github.com/user-attachments/assets/a4f3367b-d64a-4d2c-94b9-1c4efb6c4602" />


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
