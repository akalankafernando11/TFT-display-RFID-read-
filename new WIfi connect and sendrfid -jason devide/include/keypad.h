#ifndef KEYPAD_H
#define KEYPAD_H

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h>


extern TFT_eSPI tft; // Declare tft object as external

#define CALIBRATION_FILE "/TouchCalData1"
#define REPEAT_CAL false

        static bool password1 = false;
        static bool password2 = true;
        static bool clients = false; 
        
#define KEY_X 25
#define KEY_Y 117
#define KEY_W 40//62
#define KEY_H 30
#define KEY_SPACING_X 5//18
#define KEY_SPACING_Y 4//20
#define KEY_TEXTSIZE 0

#define KEY_X1 20//40
#define KEY_Y1 150//96
#define KEY_W1 31//62
#define KEY_H1 30
#define KEY_SPACING_X1 4//18
#define KEY_SPACING_Y1 4//20

#define KEY_X2 40
#define KEY_Y2 110
#define KEY_W2 60
#define KEY_H2 30
#define KEY_SPACING_X2 18
#define KEY_SPACING_Y2 7//20
#define KEY_TEXTSIZE2 0

#define LABEL1_FONT &FreeSansOblique12pt7b
#define LABEL2_FONT &FreeSansBold12pt7b

#define LABEL1_FONT1 &FreeSans9pt7b
#define LABEL2_FONT1 &FreeSansBold9pt7b

#define DISP_X 1
#define DISP_Y 45
#define DISP_W 318
#define DISP_H 45
#define DISP_TSIZE 1
#define DISP_TCOLOR TFT_CYAN

#define NUM_LEN 12
char numberBuffer[NUM_LEN + 1] = "";
uint8_t numberIndex = 0;

char numberBuffer1[NUM_LEN + 1] = "";
uint8_t numberIndex1 = 0;

#define STATUS_X 60
#define STATUS_Y 80
String sendvalue = "";
String clientvalue = "";

char keyLabel0[7][5]  = {"AC", "Del", "->", "lo", "UP", "NO", " "};                
char keyLabel[27][5]  = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "@", "#", ",", "+", "-", "*", "/", "_", "&", "^", "$", "!", "~", "(", ")", "{", "}"};
char keyLabel2[27][5] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "."};
char keyLabel3[27][5] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "."};

char onlyNO[16][5] = {"New", "Del", "Send", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "@", "#" };

uint16_t keyColor[7] = {TFT_RED, TFT_DARKGREY, TFT_DARKGREEN, TFT_MAGENTA, TFT_PINK, TFT_PURPLE, TFT_BLUE};
uint16_t keyColor2[27] = {
                         TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE
                        };
uint16_t onlyNOColor[16] = {TFT_RED, TFT_DARKGREY, TFT_DARKGREEN, TFT_BLUE, 
                         TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, 
                         TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE
                        };

TFT_eSPI_Button key[7];
TFT_eSPI_Button key1[27];
TFT_eSPI_Button key2[16];

#define LOWERCASE_KEYBOARD 0
#define UPPERCASE_KEYBOARD 1
#define NUMBER_KEYBOARD 2

int currentKeyboard = NUMBER_KEYBOARD; // Set the default keyboard to lowercase

void touch_calibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  if (!SPIFFS.begin()) {
    Serial.println("formatting file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL) {
      SPIFFS.remove(CALIBRATION_FILE);
    } else {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    tft.setTouch(calData);
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 16);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

void status(const char *msg) {
  tft.setTextPadding(320);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextFont(1);
  tft.setTextDatum(TC_DATUM);
  tft.setFreeFont(LABEL1_FONT1);
  tft.setTextSize(1);
  tft.drawString(msg, STATUS_X, STATUS_Y);
}

void drawKeypadM() {

  for (uint8_t row = 0; row < 1; row++) {
    for (uint8_t col = 0; col < 7; col++) {
      uint8_t b = col + row * 7;

      if (b < 7) tft.setFreeFont(LABEL1_FONT1);
      else tft.setFreeFont(LABEL2_FONT1);

      key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y),
                        KEY_W, KEY_H, TFT_BLACK, keyColor[b], TFT_WHITE,
                        keyLabel0[b], KEY_TEXTSIZE);
      key[b].drawButton();
    }
  }
}

void drawKeypadNO() {
  
  for (uint8_t row = 0; row < 3; row++) {
    for (uint8_t col = 0; col < 9; col++) {
      uint8_t b = col + row * 9;

      if (b < 27) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      key1[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1),
                        KEY_W1, KEY_H1, TFT_BLACK, keyColor2[b], TFT_WHITE,
                        keyLabel[b], KEY_TEXTSIZE);
      key1[b].drawButton();
    }
  }
}

void drawKeypadLowercase() {
  
  for (uint8_t row = 0; row < 3; row++) {
    for (uint8_t col = 0; col < 9; col++) {
      uint8_t b = col + row * 9;

      if (b < 27) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      key1[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1),
                        KEY_W1, KEY_H1, TFT_BLACK, keyColor2[b], TFT_WHITE,
                        keyLabel3[b], KEY_TEXTSIZE);
      key1[b].drawButton();
    }
  }

}

void drawKeypadUppercase() {
  
  for (uint8_t row = 0; row < 3; row++) {
    for (uint8_t col = 0; col < 9; col++) {
      uint8_t b = col + row * 9;

      if (b < 27) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      key1[b].initButton(&tft, KEY_X1 + col * (KEY_W1 + KEY_SPACING_X1),
                        KEY_Y1 + row * (KEY_H1 + KEY_SPACING_Y1),
                        KEY_W1, KEY_H1, TFT_BLACK, keyColor2[b], TFT_WHITE,
                        keyLabel2[b], KEY_TEXTSIZE);
      key1[b].drawButton();
    }
  }
 
}

void drawKeypadLowercaseM() {
  
  uint16_t t_x = 0, t_y = 0;
  bool pressed = tft.getTouch(&t_x, &t_y);


  for (uint8_t b = 0; b < 7; b++) {
    if (pressed && key[b].contains(t_x, t_y)) {
      key[b].press(true);
    } else {
      key[b].press(false);
    }
  }

  for (uint8_t b = 0; b < 27; b++) {
    if (pressed && key1[b].contains(t_x, t_y)) {
      key1[b].press(true);
    } else {
      key1[b].press(false);
    }
  }

 for (uint8_t b = 0; b < 7; b++) {
    if (b < 7) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (key[b].justReleased()) key[b].drawButton();

    if (key[b].justPressed()) {
      key[b].drawButton(true);

          if (b == 1) {
        numberBuffer[numberIndex] = 0;
        if (numberIndex > 0) {
          numberIndex--;
          numberBuffer[numberIndex] = 0;
        }
        status("");
      }
      if (b == 6) {
        if (numberIndex < NUM_LEN) {
          numberBuffer[numberIndex] = keyLabel0[b][0];
          numberIndex++;
          numberBuffer[numberIndex] = 0;
        }
        status("");
      }

      if (b == 2) {

        // Display data in Serial Monitor
      //  Serial.print("Received data: ");
      //  Serial.println(numberBuffer);
        sendvalue = numberBuffer;
        status("Value cleared");
        numberIndex = 0;
        numberBuffer[numberIndex] = 0;
        status("Sent value");
        Serial.print("Received data: ");
        Serial.println(sendvalue);
        password1 = true;
        password2 = false;
      }

      if (b == 0) {
        status("Value cleared");
        numberIndex = 0;
        numberBuffer[numberIndex] = 0;
      }

            if (b == 3) {
                currentKeyboard = LOWERCASE_KEYBOARD;
                drawKeypadLowercase();
            } else if (b == 4) {
                currentKeyboard = UPPERCASE_KEYBOARD;
                drawKeypadUppercase();
            } else if (b == 5) {
                currentKeyboard = NUMBER_KEYBOARD;
                drawKeypadNO();
            }
      tft.setTextDatum(TL_DATUM);
      tft.setFreeFont(&FreeSans18pt7b);
      tft.setTextColor(DISP_TCOLOR);

      int xwidth = tft.drawString(numberBuffer, DISP_X + 4, DISP_Y + 12);
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10);
    }
  }



  for (uint8_t b = 0; b < 27; b++) {
    if (b < 27) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (key1[b].justReleased()) key1[b].drawButton();

    if (key1[b].justPressed()) {
      key1[b].drawButton(true);

 if (b >= 0) {
                if (numberIndex < NUM_LEN) {
                    if (currentKeyboard == LOWERCASE_KEYBOARD) {
                        numberBuffer[numberIndex] = keyLabel3[b][0]; // Use lowercase characters
                    } else if (currentKeyboard == UPPERCASE_KEYBOARD) {
                        numberBuffer[numberIndex] = keyLabel2[b][0]; // Use uppercase characters
                    } else if (currentKeyboard == NUMBER_KEYBOARD) {
                        numberBuffer[numberIndex] = keyLabel[b][0]; // Use numbers
                    }
                    numberIndex++;
                    numberBuffer[numberIndex] = 0;
                }
                status("");
            }

       tft.setTextDatum(TL_DATUM);
      tft.setFreeFont(&FreeSans18pt7b);
      tft.setTextColor(DISP_TCOLOR);

      int xwidth = tft.drawString(numberBuffer, DISP_X + 4, DISP_Y + 12);
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10);
    }
  }

}

void drawNumberKeypad() {
  
  for (uint8_t row = 0; row < 4; row++) {
    for (uint8_t col = 0; col < 4; col++) {
      uint8_t b = col + row * 4;

      if (b < 3) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      key2[b].initButton(&tft, KEY_X2 + col * (KEY_W2 + KEY_SPACING_X2),
                        KEY_Y2 + row * (KEY_H2 + KEY_SPACING_Y2),
                        KEY_W2, KEY_H2, TFT_BLACK, onlyNOColor[b], TFT_WHITE,
                        onlyNO[b], KEY_TEXTSIZE2);
      key2[b].drawButton();
    }
  }

}

void drawNumberKeypadtouch() {  
  uint16_t t_x = 0, t_y = 0;
  bool pressed = tft.getTouch(&t_x, &t_y);

  for (uint8_t b = 0; b < 16; b++) {
    if (pressed && key2[b].contains(t_x, t_y)) {
      key2[b].press(true);
    } else {
      key2[b].press(false);
    }
  }

  for (uint8_t b = 0; b < 16; b++) {
    if (b < 3) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (key2[b].justReleased()) key2[b].drawButton();

    if (key2[b].justPressed()) {
      key2[b].drawButton(true);

      if (b >= 3) {
        if (numberIndex1 < NUM_LEN) {
          numberBuffer1[numberIndex1] = onlyNO[b][0];
          numberIndex1++;
          numberBuffer1[numberIndex1] = 0;
        }
        status("");
      }

      if (b == 1) {
        numberBuffer1[numberIndex1] = 0;
        if (numberIndex1 > 0) {
          numberIndex1--;
          numberBuffer1[numberIndex1] = 0;
        }
        status("");
      }

      if (b == 2) {
        clientvalue = numberBuffer1;
        status("Value cleared");
        numberIndex1 = 0;
        numberBuffer1[numberIndex1] = 0;
        status("Sent value");
        Serial.print("Received data: ");
        Serial.println(clientvalue);
        clients = true; 
      }

      if (b == 0) {
        status("Value cleared");
        numberIndex1 = 0;
        numberBuffer1[numberIndex1] = 0;
      }

      tft.setTextDatum(TL_DATUM);
      tft.setFreeFont(&FreeSans18pt7b);
      tft.setTextColor(DISP_TCOLOR);

      int xwidth = tft.drawString(numberBuffer1, DISP_X + 4, DISP_Y + 12);
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10);
    }
  }

}


#endif
