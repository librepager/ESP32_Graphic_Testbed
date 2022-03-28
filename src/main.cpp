#include <Arduino.h> // file necessary for all Arduino framework code in platformio
// This program was copied from the examples folder of the TFT_eSPI library
// under the sub-folder for 320 x 240 display screen code,

// this project copied from "Keypad_240x320"
// to make ESP32_Graphic_Testbed 10/5/21 Mongo
// #define TFT_CS   5 // instead of 15 in User_Setup.h to avoid conflict
// with using pin 15 to connect to ESP-Prog Board for JTAG hardware debugging
// in the LibrePager Touchscreen Testbed (TS_Testbed) 9-25-21

// this is a marker in branch "start_clean"




// The SPIFFS (FLASH filing system) is used to hold touch screen
// calibration data

#include "FS.h"

#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library


// This is the file name used to store the calibration data
// You can change this to create new calibration files.
// The SPIFFS file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData2"

// Set REPEAT_CAL to true instead of false to run calibration
#define REPEAT_CAL false

// the following lines were added by Mongo 
// they are prototype of functions 
void touch_calibrate();

//------------------------------------------------------------------------------------------

void setup() {
  // Use serial port
  Serial.begin(9600);

  // Initialise the TFT screen
  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(0);

  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();

  // Clear the screen
tft.fillScreen(TFT_BLACK);

//tft.drawPixel(100, 100, TFT_RED);

//tft.drawLine(130,100,70,200,TFT_YELLOW);

//tft.drawFastVLine(150,175,150,TFT_GREEN);

//tft.drawRect(10,10,100,100,TFT_GREENYELLOW);
//tft.fillRect(20,20,50,70,TFT_GOLD);
// tft.drawChar(200,300,'K',TFT_NAVY,6,3); // 5th parameter?? the prototype says "uint16_t bg". 0,1,6 all do same .

         
//*****************************************************************************  
tft.setCursor(10,100);
tft.setTextColor(TFT_GREEN);
tft.setTextSize(1);
tft.setTextWrap(true);
tft.print("Four score and seven years ago our fathers brought forth, upon this continent, a new na");
tft.println( "tion, conceived in liberty, and dedicated to the proposition that all men ...");
/*
delay(10000);

 tft.fillScreen(TFT_BLACK);
tft.setCursor(10,10);
 tft.println("Four score and seven years ago, our fathers came to this land...");

 tft.setCursor(10,50);

 tft.setFreeFont(&FreeSans9pt7b); //tft.setFreeFont(&FreeSansBold12pt7b);

tft.print("Four score and seven years ago our fathers brought forth, upon this continent, a new na");
tft.println( "tion, conceived in liberty, and dedicated to the proposition that all men ...");

//*****************************************************************************
*/
}

//------------------------------------------------------------------------------------------

void loop(void) {

}



void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
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

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

