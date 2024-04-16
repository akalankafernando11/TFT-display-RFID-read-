#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
//#include <XPT2046_Touchscreen.h> 
//#include "out.h"
#include "keypad.h"
#include "buzztone.h"
#include "wifiConnect.h"
#include "RFIDReadSend.h"
#include "homepage.h"
//#include "error422.h"
//#include "error500.h"
#define BUZZ 25


const char* serverUrl = "http://86.48.1.248:6060/devices";

String ssid = "";
String password = "";


TFT_eSPI tft = TFT_eSPI(); // Create TFT instance
 String uid = "";
 String chip_id = "";

void tftInit(){
    tft.init();
    tft.fillScreen(TFT_WHITE);
    tft.setRotation(3);
    touch_calibrate();
    tft.setSwapBytes(true);
}

String get_chip_id() {
    byte mac[6];
    WiFi.macAddress(mac);
    return (String)mac[5] + (String)mac[4] + (String)mac[3] + (String)mac[2] + (String)mac[1] + (String)mac[0];
}

/*String get_chip_id() {
    byte mac[6];
    WiFi.macAddress(mac);
    
    char hexChars[] = "0123456789ABCDEF";
    String hexID = "";
    
    for (int i = 0; i < 6; i++) {
        byte b = mac[i];
        hexID += hexChars[(b >> 4) & 0x0F];
        hexID += hexChars[b & 0x0F];
    }
    
    return hexID;
}*/

void setup() {
    pinMode(BUZZ, OUTPUT);

    Serial.begin(115200); // Initialize serial communications 
    // Add delay to control the speed of the loop
      // Set the button pin as an input
  pinMode(buttonPin, INPUT_PULLUP); // Use INPUT_PULLUP to enable the internal pull-up resistor

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.println("Setup done");
    
}

String p = "";

    //static bool card = false; 

void loop() {
    uint16_t x, y;
    bool press = tft.getTouch(&x, &y);
    if (!keypadDisplayed) { 
        if (!tochDisplay) {
            if (!openpage) {
            tftInit();
            tft.pushImage(40, 50,  240, 150, pk);
            tft.pushImage(1, 1,  30, 30, NC);
            delay(800); 
            openpage = true;
            }
            scanWiFiNetworks();
             refrece();
            tochDisplay = true; 
        } else {
//Serial.println("123");
refrecetouch();
            // Display SSIDs as buttons
            for (uint8_t i = 0; i < n; i++) {
                //Serial.println("456");
                if (press && key0[i].contains(x, y)) {
                    ssid = SSIDs[i];
                    tft.fillScreen(TFT_BLACK);
                    tft.fillRect(0, 0, 320, 240, TFT_WHITE);
                    tft.pushImage(1, 1,  30, 30, NC);
                    tft.setCursor(60, 30);
                    tft.setTextColor(TFT_BLACK); // White text
                    tft.setTextSize(1);
                    tft.print("Enter password code");
                    tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
                    tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

                    drawKeypadM(); 
                    drawKeypadNO();
                    exit();
                    keypadDisplayed = true;
                    password1 = false;
                    break;
                }
            }
        }
  
    } else {
        //if (press) {
            while (!password1) { // Add while loop here
                drawKeypadLowercaseM();
                exittouch();
                if (sendvalue.length() > 0) {
                    //Serial.println(sendvalue.length());
                //connectToWiFi(ssid, password);
                password = sendvalue;
                connectwifi = false;   
                }  
            }
            if (!connectwifi && password1 && !password2){
                connectToWiFi(ssid, password);
                password = "";

            }
            
       // }
     //   }
    }
                     //tft.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_RST);
            if (keypadDisplayed && connectwifi) {
           // if (!clients) {
                    //while (!card){
            while(!ChipID__SendDevice){
            uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
            Serial.println("chip_id: " + get_chip_id());
            chip_id = get_chip_id();
            sendChipIDData(chip_id);
            }

                if (!NOkey) {
                        tft.fillScreen(TFT_WHITE);
    tft.setRotation(3);
    tft.setSwapBytes(true);
                tft.pushImage(1, 1,  30, 30, CO);
                    tft.setTextColor(TFT_BLUE); // White text
                    tft.setTextSize(1);
                    tft.setFreeFont(LABEL1_FONT1);
                    tft.setCursor(40, 45);
                    tft.print("DEVICE NAME: ");
                    tft.setCursor(170, 45);
                    tft.setTextColor(TFT_BLACK); // White text
                    tft.setTextSize(1);
                    tft.print(device_name1);
                drawHomeKeypad();
                NOkey = true; 
                } else {
                drawHomeKeypadtouch();
                }
           // }
        }
    

}
