#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <Arduino.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include "keypad.h"
#include "RFIDReadSend.h"

#define KEY_X0  150  // X coordinate of the first button
#define KEY_Y0  80   // Y coordinate of the first button
#define KEY_W0  250
#define KEY_H0  25
#define KEY_SPACING_X0  6
#define KEY_SPACING_Y0  6
#define KEY_TEXTSIZE0  0

extern TFT_eSPI tft; // Declare TFT instance from main file

TFT_eSPI_Button key0[50]; // Array to store buttons, adjust size as necessary
int n;
#define MAX_SSID_LENGTH 32
#define MAX_SSIDS 50
int RSSIs[MAX_SSIDS]; // Array to store RSSI values
char SSIDs[MAX_SSIDS][MAX_SSID_LENGTH + 1]; // Array to store SSIDs with a maximum length of 32 characters
TFT_eSPI_Button refrss[1]; // Array to store buttons, adjust size as necessary
char rrefress[1][5]  = {"Scan"}; 
TFT_eSPI_Button exit1[1]; // Array to store buttons, adjust size as necessary
char exitbu[1][5]  = {"Scan"}; 

void refrece(){
 tft.setFreeFont(LABEL1_FONT1);

      refrss[0].initButton(&tft, 285, 35 , 32, 32, TFT_BLACK, TFT_WHITE, TFT_BLUE,
                        " ", KEY_TEXTSIZE);
      refrss[0].drawButton();
       // tft.setCursor(310, 40 + 1 * (30 + 3));
        tft.pushImage(275, 24,  20, 20, ref);
       // tft.setTextColor(TFT_BLUE);
        //tft.setTextSize(1);
        //tft.print(rrefress[0]);   
}

void exit(){
 tft.setFreeFont(LABEL1_FONT1);

      exit1[0].initButton(&tft, 245, 25 , 24, 24, TFT_BLACK, TFT_WHITE, TFT_BLUE,
                        " ", KEY_TEXTSIZE);
      exit1[0].drawButton();
       // tft.setCursor(310, 40 + 1 * (30 + 3));
        tft.pushImage(235, 15,  20, 20, cancel);
       // tft.setTextColor(TFT_BLUE);
        //tft.setTextSize(1);
        //tft.print(rrefress[0]);   
}
void refrecetouch(){
                      uint16_t x1, y1;
    bool press1 = tft.getTouch(&x1, &y1);
     if (press1 && refrss[0].contains(x1, y1)) {
      tochDisplay = false;  
      tft.fillScreen(TFT_WHITE);
     tft.setFreeFont(LABEL1_FONT);
     tft.setCursor(50, 100);
        tft.setTextColor(TFT_BLACK); // White text
        tft.println("Network scan..."); 

      }
      

}

void exittouch(){
                      uint16_t x1, y1;
    bool press1 = tft.getTouch(&x1, &y1);
     if (press1 && exit1[0].contains(x1, y1)) {
      Serial.println("touch me");
password1 = true;
keypadDisplayed = false;
tochDisplay = false; 
 tft.fillScreen(TFT_WHITE);
     tft.setFreeFont(LABEL1_FONT);
     tft.setCursor(50, 100);
        tft.setTextColor(TFT_BLACK); // White text
        tft.println("Network scan..."); 
      }
      
}

void scanWiFiNetworks() {
    Serial.println("scan start");
    // WiFi.scanNetworks will return the number of networks found
    n = WiFi.scanNetworks();
    // Clear the screen
    delay(50);
    if (n == 0) {
        Serial.println("no networks found");
        scanWiFiNetworks();
    } else {
        Serial.print(n);
        Serial.println(" networks found");

        // Store SSIDs and RSSIs
        for (int i = 0; i < n; ++i) {
            strncpy(SSIDs[i], WiFi.SSID(i).c_str(), MAX_SSID_LENGTH);
            SSIDs[i][MAX_SSID_LENGTH] = '\0'; // Null-terminate the string
            RSSIs[i] = WiFi.RSSI(i);
        }

        // Sort SSIDs and RSSIs based on RSSI (Bubble Sort)
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (RSSIs[j] < RSSIs[j + 1]) {
                    // Swap RSSIs
                    int tempRSSI = RSSIs[j];
                    RSSIs[j] = RSSIs[j + 1];
                    RSSIs[j + 1] = tempRSSI;

                    // Swap SSIDs
                    char tempSSID[MAX_SSID_LENGTH + 1];
                    strncpy(tempSSID, SSIDs[j], MAX_SSID_LENGTH + 1);
                    strncpy(SSIDs[j], SSIDs[j + 1], MAX_SSID_LENGTH + 1);
                    strncpy(SSIDs[j + 1], tempSSID, MAX_SSID_LENGTH + 1);
                }
            }
        }

        // Print sorted SSIDs and RSSIs
        for (int i = 0; i < n; ++i) {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(SSIDs[i]);
            Serial.print(" (");
            Serial.print(RSSIs[i]);
            Serial.println("dBm)");
            delay(10);
        }
        // Display SSIDs on TFT screen
            tft.fillScreen(TFT_WHITE);
            tft.setRotation(3);
            tft.pushImage(1, 1,  30, 30, NC);
            tft.setTextSize(1);
            tft.setFreeFont(LABEL2_FONT1);
            tft.setCursor(45, 40);
            tft.setTextColor(TFT_BLACK); // White text
            tft.print("AVAILABLE NETWORKS");
            tft.setTextSize(1);
            for (uint8_t row = 0; row < n; row++) {
        for (uint8_t col = 0; col < 1; col++) {
            uint8_t b = col + row * 1;

            if (b < n) tft.setFreeFont(LABEL1_FONT1);
            else tft.setFreeFont(LABEL2_FONT1);

        
char label[5]; // Assuming the maximum length of the label will not exceed 5 characters
sprintf(label, "%d", b); // Convert integer b to character array
            key0[b].initButton(&tft, KEY_X0 + col * (KEY_W0 + KEY_SPACING_X0),
                KEY_Y0 + row * (KEY_H0 + KEY_SPACING_Y0),
                KEY_W0, KEY_H0, TFT_BLACK, TFT_WHITE, TFT_BLACK,
                " ", KEY_TEXTSIZE0); // Use full SSID for button label
            key0[b].drawButton(); 
 // Display SSID name
        tft.setCursor(50, 85 + row * (KEY_H0 + KEY_SPACING_Y0));
        tft.setTextColor(TFT_BLACK);
        tft.setTextSize(1);
        tft.print(SSIDs[b]);
        //tft.print(" (");
        //tft.print(RSSIs[b]);
        //tft.print("dBm)");
            
        }
    }


    }
    Serial.println("");
}

void connectToWiFi(String ssid, String password) {
    Serial.print("Connecting to ");
     Serial.println(password);
    Serial.println(ssid);
        tft.fillScreen(TFT_WHITE);
        tft.setRotation(3);
        tft.pushImage(1, 1,  30, 30, NC);
        tft.setTextSize(1);
        tft.setFreeFont(LABEL1_FONT1);
        tft.setCursor(50, 100);
        tft.setTextColor(TFT_BLACK); // White text
        tft.println("Connecting to ");
        tft.setCursor(80, 120);
        tft.println(ssid);
        delay(2500); 
    WiFi.begin(ssid.c_str(), password.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 12) {
        delay(1500);
        // Clear the screen
        tft.fillScreen(TFT_WHITE);
        tft.setRotation(3);
        tft.pushImage(1, 1,  30, 30, NC);
        tft.setTextSize(1);
        tft.setFreeFont(LABEL1_FONT1);
        tft.setCursor(50, 100);
        tft.setTextColor(TFT_BLACK); // White text
        tft.println("Attempting to connect...");
        Serial.println("Attempting to connect...");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        tft.fillScreen(TFT_WHITE);
        tft.setRotation(3);
        tft.pushImage(1, 1,  30, 30, CO);
        tft.setTextSize(1);
        tft.setFreeFont(LABEL1_FONT1);
        tft.setCursor(50, 100);
        tft.setTextColor(TFT_BLACK); // White text
        tft.println("Connected to WiFi!");
        //tft.setCursor(80, 120);
        //tft.println(WiFi.localIP());
        delay(2500); 
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        delay(100);
        password = " ";
        connectwifi= true;
       // readRFID();
    } else {
        tft.fillScreen(0x0000); // Clear the screen
        tft.setTextSize(1);
        tft.setCursor(40, 120);
        tft.setTextColor(0xFFFF); // White text
        tft.println("Failed to connect to WiFi");
        Serial.println("Failed to connect to WiFi");
        delay(100);
        sendvalue = " ";
        password = " ";
        keypadDisplayed = false;
        tochDisplay = false;
         password1 = false;
          connectwifi = false;
        // Add a delay before retrying to connect
         /* for (int i = 0; i < 3; i++) {
            connectToWiFi(ssid, password);
            delay(5000); // Delay between connection attempts (adjust as needed)
            }*/
         /*if(!errorpassword){
           
                
              
              errorpassword = true;
         } */   
               
    }

}

#endif // WIFICONNECT_H
