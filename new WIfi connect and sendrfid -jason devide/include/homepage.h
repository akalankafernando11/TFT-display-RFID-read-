#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <Arduino.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include "keypad.h"
#include "RFIDReadSend.h"


     

String clients1 = "";
  
    static bool NOkey = false; 
//const char* serverUrl1 = "http://86.48.1.248:6060/clients";

#define KEY_X3  150  // X coordinate of the first button
#define KEY_Y3  90   // Y coordinate of the first button
#define KEY_W3  250
#define KEY_H3  42
#define KEY_SPACING_X3  10
#define KEY_SPACING_Y3  10
#define KEY_TEXTSIZE3  0

#define MAX_NAME_LENGTH 32
#define MAX_NAME 3

extern TFT_eSPI tft; // Declare TFT instance from main file
TFT_eSPI_Button Homekey[MAX_NAME]; // Array to store buttons, adjust size as necessary
char homekeyLabel[MAX_NAME][MAX_NAME_LENGTH + 1]  = {"In TIME", "Out TIME", "General Reader"}; 
// Add any other declarations or definitions related to your homepage here
bool inTimeKeypadDisplayed = false; 
TFT_eSPI_Button cancelB[1]; // Array to store buttons, adjust size as necessary

void cancelbutton(){
 tft.setFreeFont(LABEL1_FONT1);

      cancelB[0].initButton(&tft, 245, 25 , 24, 24, TFT_BLACK, TFT_WHITE, TFT_BLUE,
                        " ", KEY_TEXTSIZE);
      cancelB[0].drawButton();
        tft.pushImage(235, 15,  20, 20, cancel);
}

void canseltouch(){
                      uint16_t x1, y1;
    bool press1 = tft.getTouch(&x1, &y1);
     if (press1 && cancelB[0].contains(x1, y1)) {
      NOkey = false;
            clients = true;
            RFID__READ = true;
 
      }
      
}

void sendClient() {
  //Serial.println(clientvalue.length());

  if (WiFi.status() == WL_CONNECTED) {

    StaticJsonDocument<200> jsonDocument;
    int intdeviceID1 = deviceID1.toInt();
    int intclientvalue = clientvalue.toInt();
    jsonDocument["device_id"] = intdeviceID1;//"f444f444";//uid;
    jsonDocument["client_code"] = intclientvalue;//uid;//"f444f444";//uid;

    String jsonString;
    serializeJson(jsonDocument, jsonString);

    HTTPClient http;
    Serial.println("Sending client_code data to server...");
    Serial.print("JSON Payload: ");
    Serial.println(jsonString);

    http.begin(client_validate_url1);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonString);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    //buzzMe1();

if (httpResponseCode == 200) {
        tft.fillScreen(TFT_WHITE);
        tft.setRotation(3);
        tft.pushImage(1, 1,  30, 30, CO);
        tft.setTextSize(1);
        tft.setFreeFont(LABEL1_FONT1);
        tft.setCursor(50, 100);
        tft.setTextColor(TFT_BLACK); // White text
        tft.print("Logging in");
     clientvalue = "";
    sendDataIN = false; // Set the flag to false to switch to readRFID loop
    sendDataOUT = false; 
} else if (httpResponseCode == 500) {
    tft.fillScreen(TFT_WHITE); // Clear the screen
    tft.setCursor(50, 70);
    tft.setTextColor(TFT_RED); // White text
    tft.setTextSize(1);
    tft.print("DEVICE ERROR !");
    tft.pushImage(30, 90,  260, 120, er500);
    delay(1500); 
    sendClient();
} else if (httpResponseCode == 422) {
    tft.fillScreen(TFT_WHITE); // Clear the screen
    tft.setCursor(50, 70);
    tft.setTextColor(TFT_RED); // White text
    tft.setTextSize(1);
    tft.print("DEVICE ERROR !");
    tft.pushImage(30, 90,  260, 120, e422);
    delay(1500);
    tft.fillScreen(TFT_BLACK);
          tft.fillRect(0, 0, 320, 240, TFT_WHITE);
          tft.pushImage(1, 1,  30, 30, CO);
          tft.setFreeFont(LABEL1_FONT1);
          tft.setCursor(60, 30);
          tft.setTextColor(TFT_BLACK); // White text
          tft.setTextSize(1);
          tft.print("Enter your code");
          tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
          tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
          drawNumberKeypad();
          cancelbutton();
         // clientsKEY = true; 
    clientvalue = "";
    // sendDataIN = true; 
    // clientsKEY = false;
   // sendClient();
    clients = false;
} else {
    // Handle other HTTP response codes if needed
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
}


   
    
    String response = http.getString();
    Serial.println(response);

              deserializeJson(Clientsreceved, response);
              JsonObject json_response = Clientsreceved.as<JsonObject>();
              String class_id = json_response["class_id"];
              String device_id = json_response["device_id"];
              String class_name = json_response["class_name"];
              String teacher_name = json_response["teacher_name"];

              class_id1 = class_id;
              device_id1 = device_id;
              class_name1 =class_name;
              teacher_name1 = teacher_name;

              Serial.println(class_id1);
              Serial.println(device_id1);
              Serial.println(class_name1);
              Serial.println(teacher_name1);
    // Add a delay before closing the HTTP client
    delay(2000);
    http.end();
  } else {
    Serial.println("WiFi not connected. Unable to send RFID data.");
  }
 
}

void drawHomeKeypad() {
  
  for (uint8_t row = 0; row < 3; row++) {
    for (uint8_t col = 0; col < 1; col++) {
      uint8_t b = col + row * 1;

      if (b < 3) tft.setFreeFont(LABEL1_FONT1);
      else tft.setFreeFont(LABEL2_FONT1);

      Homekey[b].initButton(&tft, KEY_X3 + col * (KEY_W3 + KEY_SPACING_X3),
                        KEY_Y3 + row * (KEY_H3 + KEY_SPACING_Y3),
                        KEY_W3, KEY_H3, TFT_BLACK, TFT_WHITE, TFT_BLUE,
                        " ", KEY_TEXTSIZE);
      Homekey[b].drawButton();
        tft.setCursor(80, 95 + row * (KEY_H3 + KEY_SPACING_Y3));
        tft.setTextColor(TFT_BLUE);
        tft.setTextSize(1);
        tft.print(homekeyLabel[b]);
    }
  }
}

void drawHomeKeypadtouch() {
                      uint16_t x1, y1;
    bool press1 = tft.getTouch(&x1, &y1);

    tft.setFreeFont(LABEL1_FONT);

if (press1 && Homekey[0].contains(x1, y1)) {
    if (sendDataIN) {
        if(!clientsKEY) { 
          tft.fillScreen(TFT_BLACK);
          tft.fillRect(0, 0, 320, 240, TFT_WHITE);
          tft.pushImage(1, 1,  30, 30, CO);
          tft.setCursor(60, 30);
          tft.setTextColor(TFT_BLACK); // White text
          tft.setTextSize(1);
          tft.print("Enter your code");
          tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
          tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
          drawNumberKeypad();
          cancelbutton();
          clientsKEY = true; 
        }
        while (!clients) {
          drawNumberKeypadtouch(); 
          canseltouch();               
            if (clientvalue.length() > 0) {
              clients1 = clientvalue;  
              sendClient();         
              clientvalue = ""; 
              RFID__READ = false;
            }
        } 
              
    } 
           
           while(!RFID__READ){
                    displayData();
                    tft.setSwapBytes(true);
                    tft.pushImage(130, 120,  65, 65, scan);
                    //tft.fillRect(60, 130, 200, 40, TFT_WHITE);
                    tft.setCursor(70, 215);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.setTextColor(TFT_RED); // White text
                    tft.print("Scan Your Card Now");
            readRFID(1);
            card = false;
            }

            NOkey = false;
            clientsKEY = false;
            clients = false;
            sendDataIN = true;
            sendDataOUT = true;
            RFID__READ = false;
            card = false;
            genaralData =true; 
            
      }
      
 if (press1 && Homekey[1].contains(x1, y1)) {
    if (sendDataOUT) {
        if(!clientsKEY) { 
          tft.fillScreen(TFT_BLACK);
          tft.fillRect(0, 0, 320, 240, TFT_WHITE);
          tft.pushImage(1, 1,  30, 30, CO);
          tft.setCursor(60, 30);
          tft.setTextColor(TFT_BLACK); // White text
          tft.setTextSize(1);
          tft.print("Enter your code");
          tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
          tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
          drawNumberKeypad();
          cancelbutton();
          clientsKEY = true; 
        }
        while (!clients) {
          drawNumberKeypadtouch(); 
          canseltouch();               
            if (clientvalue.length() > 0) {
              clients1 = clientvalue;  
              sendClient();         
              clientvalue = ""; 
              RFID__READ = false;
            }
        }            
    } 
    while(!RFID__READ){
                    displayData();
                    tft.setSwapBytes(true);
                    tft.pushImage(130, 120,  65, 65, scan);
                    //tft.fillRect(60, 130, 200, 40, TFT_WHITE);
                    tft.setCursor(70, 215);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.setTextColor(TFT_RED); // White text
                    tft.print("Scan Your Card Now");
            readRFID(2);
            card = false;
            }
    Serial.println("2");
   NOkey = false;
            clientsKEY = false;
            clients = false;
            sendDataIN = true;
            sendDataOUT = true;
            RFID__READ = false;
            card = false;
            genaralData = true;        

      }

 if (press1 && Homekey[2].contains(x1, y1)) {
      while(genaralData){
       // card = false;
       tft.fillScreen(0x255C);
                    tft.pushImage(1, 1,  30, 30, R);
                    tft.setTextSize(1);
                    tft.setFreeFont(LABEL2_FONT);
                    tft.setCursor(85, 35);
                    tft.setTextColor(TFT_WHITE); // White text
                    tft.print(device_name1);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.fillRect(20, 55, 280, 175, TFT_WHITE);
                     tft.setSwapBytes(true);
                    tft.pushImage(130, 120,  65, 65, scan);
                    //tft.fillRect(60, 130, 200, 40, TFT_WHITE);
                    tft.setCursor(70, 215);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.setTextColor(TFT_RED); // White text
                    tft.print("Scan Your Card Now");
            readRFID(3);
           card = false;  
      }
            NOkey = false;
            clientsKEY = false;
            clients = false;
            sendDataIN = true;
            sendDataOUT = true;
            RFID__READ = false;
            card = false;
            genaralData = true;
      }

      tft.setTextDatum(TL_DATUM);
      tft.setFreeFont(&FreeSans18pt7b);
      tft.setTextColor(DISP_TCOLOR);

      delay(10);
   // }
//  }

}


#endif // HOMEPAGE_H