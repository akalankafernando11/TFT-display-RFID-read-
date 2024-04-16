#ifndef RFIDREADSEND_H
#define RFIDREADSEND_H

#include <Arduino.h>
#include <MFRC522.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "buzztone.h"
#include "error422.h"
#include "error500.h"
#include "out.h"

#define RST_PIN 26     // Reset pin for RC522 module
#define SDA_PIN 5      // Slave Select pin for RC522 module

            String deviceID1 = "";
            String device_name1 = "";
            String client_validate_url1 = "";
            String general_feed_url1 = "";
            String attendance_in_url1 = "";
            String attendance_out_url1 = "";
            String class_id1 = "";
            String device_id1 = "";
            String class_name1 = "";
            String teacher_name1 = "";
            String status1 = "";
            String name1 = "";
            String gender1 = "";
            String error1 = "";
            
const int buttonPin = 21; 
            int buttonState = 0;
static bool RFID__READ = false;
static bool RFID__READagain = false;
        static bool connectwifi = false;
    static bool NOkeytoch = false;
    bool sendDataIN = true;
    bool sendDataOUT = true;
    bool genaralData = true;
static bool clientsKEY = false; 

    static bool keypadDisplayed = false; // Flag to track if the keypad is displayed
    static bool tochDisplay = false; 
    static bool openpage = false; 
     static bool errorpassword = false;             
extern MFRC522 mfrc522; // Declare MFRC522 instance from main file
extern TFT_eSPI tft;    // Declare TFT instance from main file
extern String uid;      // Declare RFID UID from main file
extern String chip_id;
extern const char* serverUrl; // Declare server URL from main file
static bool card = false; 
   static bool ChipID__SendDevice = false;
   static bool RFID__SendAttendance = true;

MFRC522 mfrc522(SDA_PIN, RST_PIN);   // Create MFRC522 instance
StaticJsonDocument<290> Chipreceved;
StaticJsonDocument<290> Clientsreceved;
StaticJsonDocument<290> rfidreceved;

void displayData(){
                    tft.fillScreen(0x255C);
                    tft.pushImage(1, 1,  30, 30, R);
                    tft.setTextSize(1);
                    tft.setFreeFont(LABEL2_FONT);
                    tft.setCursor(85, 35);
                    tft.setTextColor(TFT_WHITE); // White text
                    tft.print(device_name1);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.fillRect(20, 55, 280, 175, TFT_WHITE);
                    tft.setCursor(35, 80);
                    tft.setTextColor(TFT_BLUE); // White text
                    tft.print(class_name1);
                    tft.setFreeFont(LABEL1_FONT1);
                    tft.setCursor(85, 105);
                    tft.setTextColor(TFT_BLUE); // White text
                    tft.print(teacher_name1);
                    
}

void displayRFIDsendData(){
    tft.setSwapBytes(true);
    tft.pushImage(130, 120,  65, 65, done);
    //tft.fillRect(60, 130, 200, 40, TFT_WHITE);
    tft.setCursor(115, 215);
    tft.setFreeFont(LABEL2_FONT1);
    tft.setTextColor(0x006400); // White text
    tft.print("Scan Done");
}

void sendChipIDData(String Chipid) {
  if (WiFi.status() == WL_CONNECTED) {
    StaticJsonDocument<200> jsonDocument;
    jsonDocument["chip_id"] = Chipid;

    String jsonString;
    serializeJson(jsonDocument, jsonString);

    HTTPClient http;
    Serial.println("Sending RFID data to server...");
    Serial.print("JSON Payload: ");
    Serial.println(jsonString);

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

        tft.fillScreen(TFT_WHITE);
        tft.setRotation(3);
        tft.pushImage(1, 1,  30, 30, CO);
        tft.setTextSize(1);
        tft.setFreeFont(LABEL1_FONT1);
        tft.setCursor(50, 100);
        tft.setTextColor(TFT_BLACK); // White text
        tft.println("Authenticating Device");
        delay(150);

    int httpResponseCode = http.POST(jsonString);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
   // buzzMe1();

if (httpResponseCode == 200) {
    // Handle HTTP response code 200
        tft.fillScreen(TFT_WHITE);
        tft.setRotation(3);
        tft.pushImage(1, 1,  30, 30, CO);
        tft.setTextSize(1);
        tft.setFreeFont(LABEL1_FONT1);
        tft.setCursor(50, 100);
        tft.setTextColor(TFT_BLACK); // White text
        tft.print("Authenticating Device");
   // SPI.end();
    Chipid = "";
    ChipID__SendDevice = true;

} else if (httpResponseCode == 500) {
    tft.fillScreen(TFT_WHITE); // Clear the screen
    tft.setCursor(50, 70);
    tft.setTextColor(TFT_RED); // White text
    tft.setTextSize(1);
    tft.print("DEVICE ERROR !");
    tft.pushImage(30, 90,  260, 120, er500);
    delay(1500); 
    sendChipIDData(Chipid);
} else if (httpResponseCode == 422) {
    tft.fillScreen(TFT_WHITE); // Clear the screen
    tft.setCursor(50, 70);
    tft.setTextColor(TFT_RED); // White text
    tft.setTextSize(1);
    tft.print("DEVICE ERROR !");
    tft.pushImage(30, 90,  260, 120, e422);
    delay(1500); 
    sendChipIDData(Chipid);
} else {
    // Handle other HTTP response codes if needed
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    sendChipIDData(Chipid);
}



    String response = http.getString();
    Serial.println(response);

              deserializeJson(Chipreceved, response);
              JsonObject json_response = Chipreceved.as<JsonObject>();
              String deviceID= json_response["device_id"];
              String device_name = json_response["device_name"];
              String client_validate_url = json_response["client_validate_url"];
              String general_feed_url = json_response["general_feed_url"];
              String attendance_in_url = json_response["attendance_in_url"];
              String attendance_out_url = json_response["attendance_out_url"];
              deviceID1 = deviceID;
              device_name1 = device_name;
              client_validate_url1=client_validate_url;
              general_feed_url1 = general_feed_url;
              attendance_in_url1 = attendance_in_url;
              attendance_out_url1 = attendance_out_url;
              Serial.println(deviceID1);
              Serial.println(device_name1);
              Serial.println(client_validate_url1);
              Serial.println(general_feed_url1);
              Serial.println(attendance_in_url1);
              Serial.println(attendance_out_url1);
    // Add a delay before closing the HTTP client
    delay(2000);
    http.end();
  } else {
    Serial.println("WiFi not connected. Unable to send RFID data.");
  }
}

void sendRFIDInAttendaces(String uid) {
  if (WiFi.status() == WL_CONNECTED) {
    SPI.end();
    SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
    tft.init();
    displayData();
    displayRFIDsendData();

      StaticJsonDocument<200> jsonDocument;
      int intdevice_id = device_id1.toInt();
      int intclass_id = class_id1.toInt();
      jsonDocument["device_id"] = intdevice_id;
      jsonDocument["class_id"] = intclass_id;
      jsonDocument["card_mac"] = uid;
      String jsonString;
      serializeJson(jsonDocument, jsonString);

      HTTPClient http;
      Serial.println("Sending RFID data to server...");
      Serial.print("JSON Payload: ");
      Serial.println(jsonString);

      http.begin(attendance_in_url1);
      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(jsonString);

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      //buzzMe1();

      String response = http.getString();
      Serial.println(response);

              deserializeJson(rfidreceved, response);
              JsonObject json_response = rfidreceved.as<JsonObject>();
              String status = json_response["status"];
              String name = json_response["name"];
              String gender = json_response["gender"];
              String error = json_response["error"];
              status1 = status;
              name1 = name;
              gender1=gender;
              error1=error;
              Serial.println(status1);
              Serial.println(name1);
              Serial.println(gender1);
              Serial.println(error1);
                if (httpResponseCode == 200) {
                  displayData();
                  tft.setSwapBytes(true);

                  if(status1 == "PAID"){
                  tft.pushImage(100, 115,  120, 30, paid);
                  tft.pushImage(40, 150,  30, 30, person);
                  tft.setCursor(80, 170);
                  tft.setTextColor(TFT_BLACK); // White text
                  tft.print(name1);
                  tft.setCursor(130, 200);
                  tft.setTextColor(TFT_BLUE); // White text
                  tft.print(gender1);
                  buzzMe();
                  }else if(status1 == "NOT_PAID"){
                  tft.pushImage(100, 115,  120, 30, notpaid);
                  tft.pushImage(40, 150,  30, 30, person);
                  tft.setCursor(80, 170);
                  tft.setTextColor(TFT_BLACK); // White text
                  tft.print(name1);
                  tft.setCursor(130, 200);
                  tft.setTextColor(TFT_BLUE); // White text
                  tft.print(gender1);
                  buzzMe1();
                  }else if(status1 == "BLOCKED"){
                    playDangerousTone();
                  tft.pushImage(100, 115,  120, 30, block);
                  tft.pushImage(40, 150,  30, 30, person);
                  tft.setCursor(80, 170);
                  tft.setTextColor(TFT_BLACK); // White text
                  tft.print(name1);
                  tft.setCursor(130, 200);
                  tft.setTextColor(TFT_BLUE); // White text
                  tft.print(gender1);
                    delay(1500); 
                    stopBuzzer();
                  }


                      // Add a delay before closing the HTTP client

                } else if (httpResponseCode == 500) {
                    tft.fillScreen(TFT_WHITE); // Clear the screen
                    tft.setCursor(50, 70);
                    tft.setTextColor(TFT_RED); // White text
                    tft.setTextSize(1);
                    tft.print("DEVICE ERROR !");
                    tft.pushImage(30, 90,  260, 120, er500);
                    delay(1500); 
                   // sendRFIDInAttendaces(uid);
                } else if (httpResponseCode == 422) {
                  displayData();
                  tft.setSwapBytes(true);
                  playDangerousTone();
                  if(error1 == "Invalid Card !"){
                    tft.setFreeFont(LABEL2_FONT);
                    tft.setCursor(80, 160);
                    tft.setTextColor(TFT_RED); // White text
                    tft.print(error1);
                  }
                    delay(1500); 
                    stopBuzzer();
                  //  sendRFIDInAttendaces(uid);
                } else {
                    // Handle other HTTP response codes if needed
                    Serial.print("HTTP Response code: ");
                    Serial.println(httpResponseCode);
                    tft.fillScreen(0x255C);
                    tft.pushImage(1, 1,  30, 30, R);
                    tft.setTextSize(1);
                    tft.setFreeFont(LABEL2_FONT);
                    tft.setCursor(85, 35);
                    tft.setTextColor(TFT_WHITE); // White text
                    tft.print(device_name1);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.fillRect(20, 55, 280, 175, TFT_WHITE);
                  tft.pushImage(120, 120,  70, 70, tryagain);
                  //  sendRFIDInAttendaces(uid);
                }

                delay(2000);
                http.end();
    
  } else {
    Serial.println("WiFi not connected. Unable to send RFID data.");
  }
}

void sendRFIDOutAttendaces(String uid) {
  if (WiFi.status() == WL_CONNECTED) {
    SPI.end();
    SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
    tft.init();
      displayData();
    displayRFIDsendData();

      StaticJsonDocument<200> jsonDocument;
      int intdevice_id = device_id1.toInt();
      int intclass_id = class_id1.toInt();
      jsonDocument["device_id"] = intdevice_id;
      jsonDocument["class_id"] = intclass_id;
      jsonDocument["card_mac"] = uid;
      String jsonString;
      serializeJson(jsonDocument, jsonString);

      HTTPClient http;
      Serial.println("Sending RFID data to server...");
      Serial.print("JSON Payload: ");
      Serial.println(jsonString);

      http.begin(attendance_out_url1);
      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(jsonString);
//buzzMe1();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      

      String response = http.getString();
      Serial.println(response);


                if (httpResponseCode == 200) {
                  displayData();
                  tft.setSwapBytes(true);
                  tft.setSwapBytes(true);
                  tft.pushImage(130, 120,  65, 65, send);
                  tft.setCursor(115, 215);
                  tft.setFreeFont(LABEL2_FONT1);
                  tft.setTextColor(0x006400); // White text
                  tft.print("Send Done");
buzzMe();

                } else {
                    // Handle other HTTP response codes if needed
                    Serial.print("HTTP Response code: ");
                    Serial.println(httpResponseCode);
                     tft.fillScreen(0x255C);
                    tft.pushImage(1, 1,  30, 30, R);
                    tft.setTextSize(1);
                    tft.setFreeFont(LABEL2_FONT);
                    tft.setCursor(85, 35);
                    tft.setTextColor(TFT_WHITE); // White text
                    tft.print(device_name1);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.fillRect(20, 55, 280, 175, TFT_WHITE);
                  tft.pushImage(120, 120,  70, 70, tryagain);
                //    sendRFIDOutAttendaces(uid);
                }

                delay(2000);
                http.end();
    
  } else {
    Serial.println("WiFi not connected. Unable to send RFID data.");
  }
}

void sendRFIDGeneralAttendaces(String uid) {
  if (WiFi.status() == WL_CONNECTED) {
    SPI.end();
    SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
    tft.init();
                    tft.fillScreen(0x255C);
                    tft.pushImage(1, 1,  30, 30, R);
                    tft.setTextSize(1);
                    tft.setFreeFont(LABEL2_FONT);
                    tft.setCursor(85, 35);
                    tft.setTextColor(TFT_WHITE); // White text
                    tft.print(device_name1);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.fillRect(20, 55, 280, 175, TFT_WHITE);
    displayRFIDsendData();

      StaticJsonDocument<200> jsonDocument;
      int intdevice_id = device_id1.toInt();
      int intclass_id = class_id1.toInt();
      jsonDocument["device_id"] = intdevice_id;
      jsonDocument["card_mac"] = uid;
      String jsonString;
      serializeJson(jsonDocument, jsonString);

      HTTPClient http;
      Serial.println("Sending RFID data to server...");
      Serial.print("JSON Payload: ");
      Serial.println(jsonString);

      http.begin(general_feed_url1);
      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(jsonString);
//buzzMe1();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);


      String response = http.getString();
      Serial.println(response);


                if (httpResponseCode == 200) {
                     tft.fillScreen(0x255C);
                    tft.pushImage(1, 1,  30, 30, R);
                    tft.setTextSize(1);
                    tft.setFreeFont(LABEL2_FONT);
                    tft.setCursor(85, 35);
                    tft.setTextColor(TFT_WHITE); // White text
                    tft.print(device_name1);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.fillRect(20, 55, 280, 175, TFT_WHITE);
                  tft.pushImage(130, 120,  65, 65, send);
                  tft.setCursor(115, 215);
                  tft.setFreeFont(LABEL2_FONT1);
                  tft.setTextColor(0x006400); // White text
                  tft.print("Send Done");
                  buzzMe();
                }
                else {
                    // Handle other HTTP response codes if needed
                    Serial.print("HTTP Response code: ");
                    Serial.println(httpResponseCode);
                     tft.fillScreen(0x255C);
                    tft.pushImage(1, 1,  30, 30, R);
                    tft.setTextSize(1);
                    tft.setFreeFont(LABEL2_FONT);
                    tft.setCursor(85, 35);
                    tft.setTextColor(TFT_WHITE); // White text
                    tft.print(device_name1);
                    tft.setFreeFont(LABEL2_FONT1);
                    tft.fillRect(20, 55, 280, 175, TFT_WHITE);
                  tft.pushImage(120, 120,  70, 70, tryagain);
                   // sendRFIDGeneralAttendaces(uid);
                }

                delay(2000);
                http.end();
    
  } else {
    Serial.println("WiFi not connected. Unable to send RFID data.");
  }
}

void readRFID(int number) {
  SPI.end();
    while (!card){
      SPI.begin(); 
      mfrc522.PCD_Init(SDA_PIN, RST_PIN); // Init MFRC522
      // Read the state of the button
      buttonState = digitalRead(buttonPin);

      // Check if the button is pressed
      if (buttonState == LOW) { // Check for LOW state if button is configured with a pull-up resistor
        // Button is pressed
        Serial.println("Button pressed!");
        SPI.end();
        SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
        tft.init();
        card = true;
        RFID__READ = true;
        genaralData = false;
        // Add your code here to perform actions when the button is pressed
        // For example, you can toggle an LED, send a message over WiFi, etc.
      }

      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        uid = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
          uid += String(mfrc522.uid.uidByte[i], HEX);
        }

        Serial.print("TagID = ");
        Serial.println(uid);
        buzzMe2();
        if(number == 1){
          sendRFIDInAttendaces(uid);
        }else if( number == 2){
          sendRFIDOutAttendaces(uid);
        }else if( number == 3){
          sendRFIDGeneralAttendaces(uid);
        }
    
        card = true;
      }

    }
}
#endif // RFIDREADSEND_H
