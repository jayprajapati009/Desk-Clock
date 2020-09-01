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

int Hour;

int buzzer = 14;


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////                   Void Setup                  //////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  ////// Setting the basic display //////
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  ////// Setup for buzzer //////
  pinMode(buzzer, OUTPUT);

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

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////                   Void Loop                   //////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  delay(1000);
  lcd.clear();
  timer20min();
  lcd.clear();
  timer20sec();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Function for 20 Minutes Timer //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

int c_min = 0;
int c_min1 = 0;
int min1 = 0;

void timer20min()
{
  
  for (c_min = 0; c_min <= 20 * 60; c_min++) {
    printLocalTime();
    delay(995);
    Serial.println(c_min);
    if (c_min <= 59) {
      lcd.setCursor(11, 0);
      lcd.print("00:");
      if (c_min <= 9) {
        lcd.setCursor(14, 0);
        lcd.print("0");
        lcd.setCursor(15, 0);
        lcd.print(c_min);
      }
      else {
        lcd.setCursor(14, 0);
        lcd.print(c_min);
      }
    }

    else {
      min1 = floor(c_min / 60);
      c_min1 = c_min - (min1 * 60);

      Serial.println(min1);
      if (min1 <= 9) {
        lcd.setCursor(11, 0);
        lcd.print("0");
        lcd.setCursor(12, 0);
        lcd.print(min1);
        Serial.println(min1);
      }
      else {
        lcd.setCursor(11, 0);
        lcd.print(min1);
      }

      lcd.setCursor(13, 0);
      lcd.print(":");

      if (c_min1 <= 9) {
        lcd.setCursor(14, 0);
        lcd.print("0");
        lcd.setCursor(15, 0);
        lcd.print(c_min1);
      }
      else {
        lcd.setCursor(14, 0);
        lcd.print(c_min1);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Function for 20 Seconds Timer //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

int c_sec = 0;

void timer20sec()
{
  beep();
  delay(300);
  beep();
  for (c_sec = 0; c_sec <= 20; c_sec++) {
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("00:");
    if (c_sec <= 9) {
      lcd.setCursor(3, 0);
      lcd.print("0");
      lcd.setCursor(4, 0);
      lcd.print(c_sec);
    }
    else {
      lcd.setCursor(3, 0);
      lcd.print(c_sec);
    }
    lcd.setCursor(0, 1);
    lcd.print("Blink Your Eyes");
  }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Function for ringing alarm //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void beep() {
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(50);

  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(50);

  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(50);

  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(50);
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
  //lcd.setCursor(6, 0);
  //lcd.print(&timeinfo, "%H:%M");

  Hour = timeinfo.tm_hour;

  if (Hour > 12) {

    if ((Hour - 12) <= 9) {
      lcd.setCursor(0, 0);
      lcd.print("0");
      lcd.setCursor(1, 0);
      lcd.print(Hour - 12);
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print(Hour - 12);
    }
    lcd.setCursor(2, 0);
    lcd.print(&timeinfo, ":%M");
    lcd.setCursor(6, 0);
    lcd.print("PM");
  }

  else if(Hour == 0){
    lcd.setCursor(0, 0);
    lcd.print("0");
    lcd.setCursor(1, 0);
    lcd.print(Hour);
    lcd.setCursor(2, 0);
    lcd.print(&timeinfo, ":%M");
    lcd.setCursor(6, 0);
    lcd.print("AM");
  }
  else if(Hour == 12){
    lcd.setCursor(0, 0);
    lcd.print(Hour);
    lcd.setCursor(2, 0);
    lcd.print(&timeinfo, ":%M");
    lcd.setCursor(6, 0);
    lcd.print("PM");
  }

  else {
    if ((Hour) <= 9) {
      lcd.setCursor(0, 0);
      lcd.print("0");
      lcd.setCursor(1, 0);
      lcd.print(Hour);
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print(Hour);
    }
    lcd.setCursor(2, 0);
    lcd.print(&timeinfo, ":%M");
    lcd.setCursor(6, 0);
    lcd.print("AM");
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  lcd.setCursor(0, 1);
  lcd.print(&timeinfo, "%d");
  lcd.setCursor(2, 1);
  lcd.print(" ");
  lcd.setCursor(3, 1);
  lcd.print(&timeinfo, "%B");
  lcd.setCursor(6, 1);
  lcd.print(" ");
  lcd.setCursor(7, 1);
  lcd.print(&timeinfo, "%Y");
  lcd.setCursor(11, 1);
  lcd.print(&timeinfo, ", ");
  lcd.setCursor(13, 1);
  lcd.print(&timeinfo, "%A");
}
