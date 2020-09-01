#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#include <WiFi.h>
#include "time.h"
const char* ssid       = "speechbee";
const char* password   = "speechbee";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 5.5;
const int   daylightOffset_sec = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////                   Void Setup                  //////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  ////// Setting the basic display //////
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("TIME: ");

  ////// Setting up the wifi connection fpt NTP //////
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  ////// Blynk Setup //////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////                   Void Loop                   //////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  delay(1000);
  printLocalTime();
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Function for Printing Time on the display ////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%H:%M");
  lcd.setCursor(6, 0);
  lcd.print(&timeinfo, "%H:%M");
}
