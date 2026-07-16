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
#define CURRENT_MINUTES 10
#define CURRENT_HOURS 4
#define CURRENT_DAY_OF_WEEK 5
#define CURRENT_DAY_OF_MONTH 9
#define CURRENT_MONTH 07
#define CURRENT_YEAR 2026

virtuabotixRTC RTC(DS1302_CLK_PIN, DS1302_DAT_PIN, DS1302_RST_PIN);

// == END CONFIGURATION ==========

// get number of empty cells at month start
int getLeadingOffset(int year, int month) {
	// Zellers congruence
	if (month < 3) {
		month += 12;
		year -= 1;
	}
	int k = year % 100;
	int j = year / 100;
	int h = (1 + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
	// h: 0=Sat, 1=Sun, 2=Mon, 3=Tue, 4=Wed, 5=Thu, 6=Fri

	int weekdayMon0 = (h + 5) % 7; // into 0=Mon..6=Sun

	return weekdayMon0;
}

struct Vec2 {
  double x;
  double y;
};

Vec2 dayToCoord(int year, int month, int day) {
  int y = day / 7;
  int x = (getLeadingOffset(year, month)) + ( day - (7 * y) );

  return Vec2{x - 1, y};
}

void displayDate(int year, int month, int day) {
  Vec2 pos = dayToCoord(year, month, day);

  myDisplay.getGraphicObject()->setPoint(pos.x, pos.y, true);
}

void displayHours(int hours) {
	Vec2 pos = Vec2{0,0};
	for(int i = 0; i < hours; i++) {
		if (pos.x > 7) {
			pos.x = 0;
			pos.y++;
		};

		myDisplay.getGraphicObject()->setPoint(pos.x, pos.y+5, true);
		pos.x++;
	};
}

void displaySeconds(int seconds) {
	for(int i = 0; i < seconds/12; i++) {
		myDisplay.getGraphicObject()->setPoint(7, i, true);
	};

	if (seconds>12) {
		myDisplay.getGraphicObject()->setPoint(7, 0, seconds%2);
	}
}


void setup() {
	Serial.begin(9600); 

	// UNCOMMENT THIS WHEN BUILDING FOR THE FIRST TIME
  RTC.setDS1302Time(CURRENT_SECONDS, CURRENT_MINUTES, CURRENT_HOURS,
                    CURRENT_DAY_OF_WEEK, CURRENT_DAY_OF_MONTH, CURRENT_MONTH,
                    CURRENT_YEAR);

  // Intialize the object:
  myDisplay.begin();
  myDisplay.setFont(_fontVertical);
  // Set the inDnsity (brightness) of the display (0-15):
  myDisplay.setIntensity(0);
  // Clear the display:
  myDisplay.displayClear();
  // myDisplay.setZoneEffect(0, true, PA_FLIP_UD);
  // myDisplay.setZoneEffect(0, true, PA_FLIP_LR);
  // myDisplay.displayText("Scrolling text", PA_CENTER, 100, 0, PA_SCROLL_LEFT,
  // PA_SCROLL_LEFT);
}

void loop() {
	RTC.updateTime();
  myDisplay.displayClear();

	displayDate(RTC.year, RTC.month, RTC.dayofmonth);
	displayHours(RTC.hours);
	displaySeconds(RTC.seconds);

	if (Serial) {
		Serial.print(RTC.hours);
		Serial.print(":");
		Serial.print(RTC.minutes);
		Serial.print(":");
		Serial.print(RTC.seconds);
		Serial.println();
	}

	delay(1*1000);
}
