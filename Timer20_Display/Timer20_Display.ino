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
  

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////                   Void Loop                   //////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  delay(1000);
  lcd.clear();
  timer20min();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Function for 2 Minutes Timer //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

int c_min = 0;

void timer20min()
{

  for (c_min=0;c_min<=2*60;c_min++){
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print(c_min);
    lcd.setCursor(0, 1);
    lcd.print("Timer Started");
  }
}
