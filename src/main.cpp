#include "virtuabotixRTC.h" 
#include "MD_Parola.h"
#include "MD_MAX72xx.h"
#include "SPI.h"
#include "Parola_Fonts_data.h"


// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 2

// Create a new instance of the MD_Parola class with hardware SPI connection:
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Setup for software SPI:
#define DATAPIN 11
#define CLK_PIN 13
// MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void setup() {
  // Intialize the object:
  myDisplay.begin();
	myDisplay.setFont(_fontVertical);
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.setIntensity(0);
  // Clear the display:
  myDisplay.displayClear();
	// myDisplay.setZoneEffect(0, true, PA_FLIP_UD);
	// myDisplay.setZoneEffect(0, true, PA_FLIP_LR);
  // myDisplay.displayText("Scrolling text", PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
	myDisplay.getGraphicObject()->setPoint(3, 5, true);
}

void loop() {
  if (myDisplay.displayAnimate()) {
    myDisplay.displayReset();
  }
}

