#include "MD_MAX72xx.h"
#include "MD_Parola.h"
#include "Parola_Fonts_data.h"
#include "SPI.h"
#include "virtuabotixRTC.h"

// == DISPLAY ==========

// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 2

// Create a new instance of the MD_Parola class with hardware SPI connection:
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Setup for software SPI:
#define DATAPIN 11
#define CLK_PIN 13
// MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN,
// MAX_DEVICES);

// == CLOCK ==========

#define DS1302_CLK_PIN A5
#define DS1302_DAT_PIN A4
#define DS1302_RST_PIN A3

#define CURRENT_SECONDS 00
#define CURRENT_MINUTES 30
#define CURRENT_HOURS 10
#define CURRENT_DAY_OF_WEEK 3
#define CURRENT_DAY_OF_MONTH 3
#define CURRENT_MONTH 03
#define CURRENT_YEAR 2026

virtuabotixRTC RTC(DS1302_CLK_PIN, DS1302_DAT_PIN, DS1302_RST_PIN);

// == END CONFIGURATION ==========

struct Vec2 {
  double x;
  double y;
};

Vec2 day_to_coord(int day) {
  int y = day / 7;
  int x = day - (7 * y);

  return Vec2{x - 1, y};
}

void print_date(int day) {
  Vec2 pos = day_to_coord(day);

  myDisplay.getGraphicObject()->setPoint(pos.x, pos.y, true);
}

void setup() {
	Serial.begin(9600); 

  RTC.setDS1302Time(CURRENT_SECONDS, CURRENT_MINUTES, CURRENT_HOURS,
                    CURRENT_DAY_OF_WEEK, CURRENT_DAY_OF_MONTH, CURRENT_MONTH,
                    CURRENT_YEAR);

  // Intialize the object:
  myDisplay.begin();
  myDisplay.setFont(_fontVertical);
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.setIntensity(0);
  // Clear the display:
  myDisplay.displayClear();
  // myDisplay.setZoneEffect(0, true, PA_FLIP_UD);
  // myDisplay.setZoneEffect(0, true, PA_FLIP_LR);
  // myDisplay.displayText("Scrolling text", PA_CENTER, 100, 0, PA_SCROLL_LEFT,
  // PA_SCROLL_LEFT);
  // print_date(10);
}

void loop() {
	RTC.updateTime();
	// print_date(RTC.dayofmonth);

	print_date(RTC.seconds);

	Serial.print("Current time: ");
	Serial.print(RTC.seconds);
	Serial.println(); 

	delay(1000); 
}
