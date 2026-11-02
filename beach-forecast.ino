#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h> 
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h> 
#include <ArduinoJson.h> 
#include <time.h> 
#include <math.h>
#include <WiFiManager.h>

// --- Custom ESP32-C6 SPI Pins ---
#define SPI_SCK  6
#define SPI_MISO 1
#define SPI_MOSI 7

// --- Display & Touch Pins ---
#define display_CS 10
#define display_DC 18  
#define display_RST 4       
#define RESET_BUTTON_PIN 5   
#define TOUCH_CS 19          

Adafruit_ILI9341 display = Adafruit_ILI9341(&SPI, display_CS, display_DC, display_RST);
Adafruit_STMPE610 ts = Adafruit_STMPE610(TOUCH_CS, &SPI);

// --- Global Variables ---
bool isMainScreen = true;       
unsigned long lastTouchTime = 0; 
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 600000; // 10 minutes

// --- GRAPHICS / BITMAPS ---
// PASTE YOUR ORIGINAL HEX DATA INSIDE THESE ARRAYS
const unsigned char image_arrow_diagonal_left_up_bits[] PROGMEM = { /* paste data */ };
const unsigned char image_arrow_diagonal_right_up_bits[] PROGMEM = { /* paste data */ };
const unsigned char image_arrow_diagonal_left_down_bits[] PROGMEM = { /* paste data */ };
const unsigned char image_arrow_diagonal_right_down_bits[] PROGMEM = { /* paste data */ };
static const unsigned char PROGMEM image_arrow_curved_left_down_bits[] = { /* paste data */ };
static const unsigned char PROGMEM image_paint_7_bits[] = { /* paste data */ };
const uint16_t PROGMEM icon_house[513] = { /* paste data */ };
const uint16_t PROGMEM icon_boat[441] = { /* paste data */ };
static const uint16_t PROGMEM image_paint_13_pixels[] = { /* paste data */ };

// --- COLORS ---
#define B 0x0000 
#define W 0xFFFF 
#define R 0xF800 
#define U 0x24BE 

// --- API & Time Settings ---
const char* serverName = "https://api.open-meteo.com/v1/forecast?latitude=38.29713&longitude=22.02661&hourly=temperature_2m,wind_speed_10m,wind_gusts_10m,wind_direction_10m&wind_speed_unit=kmh&timezone=auto&forecast_days=3";
const char* ntpServer = "pool.ntp.org";
const char* tzOffset = "EET-2EEST,M3.5.0/3,M10.5.0/4"; 
const char* dayNames[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};

// --- Helper Functions ---
uint16_t getTempColor(int temp) {
  if (temp < 5)  return 0x001F;
  if (temp < 15) return 0x07FF; 
  if (temp < 25) return 0x07E0;
  if (temp < 32) return 0xFD20;
  return 0xF800;             
}

uint16_t getWindColor(int bft) {
  if (bft <= 2) return 0xFFFF;
  if (bft <= 3) return 0x775D;
  if (bft <= 4) return 0x07E0; 
  if (bft <= 5) return 0xFD20; 
  if (bft <= 6) return 0xFD20; 
  return 0xF800;              
}

int getBeaufort(float kmh) {
  if (kmh < 2.0) return 0;
  if (kmh < 6.0) return 1;
  if (kmh < 12.0) return 2;
  if (kmh < 20.0) return 3;
  if (kmh < 29.0) return 4;
  if (kmh < 39.0) return 5;
  if (kmh < 50.0) return 6;
  if (kmh < 62.0) return 7;
  if (kmh < 75.0) return 8;
  if (kmh < 89.0) return 9;
  if (kmh < 103.0) return 10;
  if (kmh < 118.0) return 11;
  return 12;
}

void drawWindArrow(int cx, int cy, int dir, uint16_t color) {
  float rad = (dir + 180) * 3.14159265 / 180.0;
  int tailX = cx - 5 * sin(rad), tailY = cy + 5 * cos(rad);
  int tipX = cx + 5 * sin(rad), tipY = cy - 5 * cos(rad);
  int baseX = cx + 1 * sin(rad), baseY = cy - 1 * cos(rad);
  int leftX = baseX - 4 * cos(rad), leftY = baseY - 4 * sin(rad);
  int rightX = baseX + 4 * cos(rad), rightY = baseY + 4 * sin(rad);
  display.drawLine(tailX, tailY, baseX, baseY, color);
  display.fillTriangle(tipX, tipY, leftX, leftY, rightX, rightY, color);
}

void drawScreen_1(void) {
    display.fillScreen(0x0);
    display.drawRect(0, 0, 320, 240, 0xFFFF);
    
    // Draw grid lines
    int vLines[] = {24, 48, 73, 98, 122, 147, 171, 196, 220, 245, 269, 294};
    for(int i = 0; i < 12; i++) {
      display.drawLine(vLines[i], 1, vLines[i], 240, 0xFFFF);
    }
    
    display.drawLine(-1, 92, 318, 92, 0xFFFF);
    display.drawLine(1, 129, 320, 129, 0xFFFF);
    display.drawLine(1, 166, 320, 166, 0xFFFF);
    display.drawLine(1, 203, 320, 203, 0xFFFF);
    
    // Draw static text
    display.setTextSize(1);
    display.setTextWrap(false);
    display.setTextColor(0xFFFF);
    display.setCursor(1, 94);  display.print("Wind");
    display.setCursor(1, 103); display.print("Spd");
    display.setCursor(3, 113); display.print("Bft"); 
    display.setCursor(1, 131); display.print("Wind");
    display.setCursor(1, 141); display.print("Gust");
    display.setCursor(3, 151); display.print("Bft"); 
    display.setCursor(0, 168); display.print("dire");
    
    // Draw arrows and icons
    display.drawBitmap(5, 192, image_arrow_diagonal_left_down_bits, 5, 5, 0xFFFF);
    display.drawBitmap(5, 184, image_arrow_diagonal_left_up_bits, 5, 5, 0xFFFF);
    display.drawBitmap(13, 192, image_arrow_diagonal_right_down_bits, 5, 5, 0xFFFF);
    display.drawBitmap(13, 184, image_arrow_diagonal_right_up_bits, 5, 5, 0xFFFF);
    
    display.setTextSize(2);
    display.setCursor(7, 211); display.print("C");
    display.drawCircle(11, 64, 9, 0xFFFF);
    display.drawLine(20, 52, 2, 52, 0x3A96);
    display.drawLine(2, 51, 2, 39, 0x3A96);
    display.drawLine(20, 36, 2, 36, 0xFFFF);
    display.drawLine(2, 35, 2, 23, 0xFFFF);
    display.drawCircle(11, 12, 9, 0x3A96);
    display.drawLine(20, 75, 3, 82, 0x3A96);
    display.drawRGBBitmap(3, 83, image_paint_13_pixels, 18, 8);
}

void drawScreen_2(void) {
    display.fillScreen(0x0); 
    display.fillCircle(115, 118, 13, 0xFFFF);
    display.fillCircle(215, 119, 13, 0xFFFF);
    display.setTextColor(0xFFFF);
    display.setTextSize(1);
    display.setTextWrap(false);
    display.setCursor(99, 89); display.print("RETURN");
    display.setCursor(198, 77); display.print("FORGET");
    display.setCursor(195, 90); display.print("NETWORK");
    display.drawBitmap(108, 113, image_arrow_curved_left_down_bits, 16, 10, 0x0);
    display.drawBitmap(207, 113, image_paint_7_bits, 17, 14, 0x0);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Configure physical reset button
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  
  // Lock Touch CS (Pin 19) high to prevent SPI conflicts during display init
  pinMode(19, OUTPUT);
  digitalWrite(19, HIGH);
  
  // Initialize Custom SPI Bus
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, -1);
  
  // Initialize Display (20MHz for stability with custom pins)
  display.begin(20000000);
  display.setRotation(1);
  display.fillScreen(ILI9341_BLACK);
  display.setTextSize(2);
  display.setTextColor(ILI9341_WHITE, ILI9341_BLACK); 
  display.setCursor(10, 20);
  display.println("Connecting WiFi...");

  // Initialize Touch Controller
  ts.begin();

  // Start WiFiManager Config Portal
  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("Meteo_Config_AP")) {
    display.fillScreen(ILI9341_BLACK);
    display.setCursor(10, 30);
    display.setTextColor(ILI9341_RED);
    display.println("Failed to connect");
    delay(3000);
    ESP.restart();
  }

  // WiFi Connected Successfully
  display.fillScreen(ILI9341_BLACK);
  display.setCursor(10, 30);
  display.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  display.println("WiFi Connected!");
  display.setCursor(10, 60);
  display.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  display.println("Syncing Time...");

  // Synchronize NTP Time
  configTzTime(tzOffset, ntpServer);
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)){
    delay(500); 
  }
  
  delay(1000); 
  drawScreen_1(); // Load Main Weather UI
}

void loop() {
  // Check physical reset button for WiFi reset
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    delay(500); // Debounce
    if (digitalRead(RESET_BUTTON_PIN) == LOW) {
      display.fillScreen(ILI9341_BLACK);
      display.setCursor(10, 30);
      display.setTextColor(ILI9341_RED);
      display.setTextSize(2);
      display.println("Wiping WiFi...");
      
      WiFiManager wifiManager;
      wifiManager.resetSettings(); 
      
      display.fillScreen(ILI9341_BLACK);
      display.setCursor(10, 20);
      display.setTextColor(ILI9341_YELLOW);
      display.println("Open Hotspot:");
      display.setCursor(10, 50);
      display.setTextSize(1);
      display.print("Connect to AP:");
      display.setCursor(10, 65);
      display.setTextColor(ILI9341_CYAN);
      display.print("Meteo_Config_AP");
      
      wifiManager.startConfigPortal("Meteo_Config_AP"); 
      
      display.fillScreen(ILI9341_BLACK);
      display.setCursor(10, 30);
      display.setTextColor(ILI9341_GREEN);
      display.println("Connected!");
      delay(1000);
      isMainScreen = true;
      drawScreen_1();
      lastUpdate = 0; // Force weather update
      return; 
    }
  }

  // Handle Touch Inputs
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    if (p.x == 0 && p.y == 0) return; // Ignore invalid coordinates
    
    if (millis() - lastTouchTime > 500) {
        // Map raw touch data to screen coordinates (Rotation 1)
        int touch_x = map(p.y, 300, 3800, 0, 320);
        int touch_y = map(p.x, 3600, 300, 0, 240);
        
        if (isMainScreen) {
          // Top-left area opens settings screen
          if (touch_x < 100 && touch_y < 100) {            
            isMainScreen = false;
            drawScreen_2(); 
          }
        } else {
          // Settings Screen Handling
          if (touch_x < 160) {
            // RETURN button clicked
            isMainScreen = true;
            drawScreen_1(); 
            lastUpdate = 0; 
          } else if (touch_x >= 160) {
            // FORGET NETWORK button clicked
            display.fillScreen(ILI9341_BLACK);
            display.setCursor(10, 30);
            display.setTextColor(ILI9341_RED);
            display.setTextSize(2);
            display.println("Wiping WiFi...");
            
            WiFiManager wifiManager;
            wifiManager.resetSettings();
            
            display.fillScreen(ILI9341_BLACK);
            display.setCursor(10, 20);
            display.setTextColor(ILI9341_YELLOW);
            display.println("Open Hotspot:");
            display.setCursor(10, 50);
            display.setTextSize(1);
            display.print("Connect to AP:");
            display.setCursor(10, 65);
            display.setTextColor(ILI9341_CYAN);
            display.print("Meteo_Config_AP");
            
            wifiManager.startConfigPortal("Meteo_Config_AP"); 
            
            display.fillScreen(ILI9341_BLACK);
            display.setCursor(10, 30);
            display.setTextColor(ILI9341_GREEN);
            display.println("Connected!");
            delay(1000);
            isMainScreen = true;
            drawScreen_1();
            lastUpdate = 0; 
          }
        }
        lastTouchTime = millis();
    }
  }

  // Fetch Weather API Data every 10 minutes
  if (isMainScreen && WiFi.status() == WL_CONNECTED) {
    if (lastUpdate == 0 || millis() - lastUpdate >= updateInterval) {
      lastUpdate = millis(); 
      
      struct tm timeinfo;
      if(!getLocalTime(&timeinfo)) return; 
      
      char currentHourStr[20];
      strftime(currentHourStr, sizeof(currentHourStr), "%Y-%m-%dT%H:00", &timeinfo);
      String targetTime = String(currentHourStr);
      time_t nowTime; 
      time(&nowTime); 
      
      WiFiClientSecure client;
      client.setInsecure(); // Bypass SSL certificate validation for Open-Meteo
      HTTPClient http;
      http.begin(client, serverName); 
      
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        String payload = http.getString(); 
        DynamicJsonDocument doc(16384); 
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error) {
          int startIndex = 0;
          // Find current hour index in the JSON array
          for (int i = 0; i < 72; i++) {
            if (doc["hourly"]["time"][i].as<String>() == targetTime) {
              startIndex = i;
              break;
            }
          }
          
          int colLeft[] = {25, 49, 74, 99, 123, 148, 172, 197, 221, 246, 270, 295};
          int colWidth[] = {23, 24, 24, 23, 24, 23, 24, 23, 24, 23, 24, 23};
          
          // Populate the 12 columns with 2-hour increments
          for (int i = 0; i < 12; i++) {
            int jsonIndex = startIndex + (i * 2); 
            time_t colTime = nowTime + (i * 2 * 3600);
            struct tm *colTm = localtime(&colTime);
            int left = colLeft[i];
            int width = colWidth[i];
            int cx = left + (width / 2);
            
            // Clear previous column data
            display.fillRect(left, 2, width, 89, ILI9341_BLACK); 
            display.fillRect(left, 93, width, 35, ILI9341_BLACK); 
            display.fillRect(left, 130, width, 35, ILI9341_BLACK); 
            display.fillRect(left, 167, width, 35, ILI9341_BLACK); 
            display.fillRect(left, 204, width, 35, ILI9341_BLACK); 
            
            // Print Day and Hour
            display.setTextSize(1);
            display.setTextColor(ILI9341_WHITE);
            display.setCursor(cx - 6, 15); 
            display.print(dayNames[colTm->tm_wday]);
            if(colTm->tm_mday < 10) display.setCursor(cx - 3, 30); 
            else display.setCursor(cx - 6, 30);
            display.print(colTm->tm_mday);
            display.setCursor(cx - 9, 45); 
            if(colTm->tm_hour < 10) display.print("0"); 
            display.print(colTm->tm_hour);
            display.print("h");
            
            // Fetch Weather Variables
            float speedKmh = doc["hourly"]["wind_speed_10m"][jsonIndex].as<float>();
            float gustsKmh = doc["hourly"]["wind_gusts_10m"][jsonIndex].as<float>();
            int temp       = (int)doc["hourly"]["temperature_2m"][jsonIndex].as<float>();
            int dir        = doc["hourly"]["wind_direction_10m"][jsonIndex].as<int>();
            int gustsBft = getBeaufort(gustsKmh);
            int speedBft = getBeaufort(speedKmh);
            
            // Draw corresponding weather icon (Boat vs House)
            if (speedBft <= 2 && gustsBft <= 3) {
              display.drawRGBBitmap(cx - 10, 70, icon_boat, 21, 21);
            } else {
              display.drawRGBBitmap(cx - 9, 64, icon_house, 19, 27);
            }
            
            // Print Wind Data
            display.setTextSize(2);
            display.setTextColor(getWindColor(speedBft), ILI9341_BLACK);
            if(speedBft < 10) display.setCursor(cx - 6, 103); 
            else display.setCursor(cx - 12, 103);
            display.print(speedBft);
            
            display.setTextColor(getWindColor(gustsBft), ILI9341_BLACK);
            if(gustsBft < 10) display.setCursor(cx - 6, 140); 
            else display.setCursor(cx - 12, 140);
            display.print(gustsBft);
            
            // Draw Wind Direction Arrow
            drawWindArrow(cx, 185, dir, ILI9341_WHITE);
            
            // Print Temperature
            display.setTextSize(1);
            display.setTextColor(getTempColor(temp), ILI9341_BLACK);
            if(temp < 10 && temp >= 0) display.setCursor(cx - 3, 217); 
            else display.setCursor(cx - 6, 217);
            display.print(temp);
          }
        }
      } 
      http.end(); // Close connection
    }
  }
}
