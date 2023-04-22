/*** RTCwakeUpComputer                  ***/
/*** Turn on your computer using an RTC ***/
/*** By Stach Redeker, 2023             ***/ 

#include "RTClib.h"

RTC_DS3231 rtc;

// PIN LAYOUT
#define RELAY 2
#define MEASURE 3

// DAYS OF THE WEEK
// stupid American format, hence why I use #define
// see: https://adafruit.github.io/RTClib/html/
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6
#define SUNDAY 0


void setup() {
  
  Serial.begin(9600);

  pinMode(RELAY, OUTPUT);
  pinMode(MEASURE, INPUT);

  // case: RTC is not connected
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC.");
    while (1);
  }

//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // case: RTC needs time set (e.g. on boot)
  if (rtc.lostPower()) {
    Serial.println("Resetting the RTC time.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

}

void loop() {

  DateTime now = rtc.now();

  // I want to run a system backup on Sunday at 03:00h. Another service will initiate this backup.
  // It is now UTC+2, so 03:00h might become 02:00h at some point.
  // I don't know if the system is able to cope with daylight saving time, so I am incorporating a safety margin.
  // The system takes some time to boot up, and to actually run the backup. I also that that into account.
  // turn on: 01:00h, turn off: 05:00h
  if (now.dayOfTheWeek() == SUNDAY) {
    if (now.hour() >= 1 && now.hour() < 5) { // computer should be turned on between 01:00h and 05:00h
      if (validateStatus(false)) { // if the computer is not turned on
        switchComputer(); // turn on
        delay(1000*60); // wait for one minute before trying again
      }
    }
      else if (now.hour() == 5) { // this is the dead zone: the computer can never be turned on between the 05:00h and 06:00h. I can accept this for my application.
        if (validateStatus(true)) { // if the computer is on
          switchComputer(); // turn off
          delay(1000*60*3); // wait for three minutes before trying again
        }
      }
    }
  
  delay(1000*60);
    
//    Serial.print(now.dayOfTheWeek(), DEC);
//    Serial.print("Current time: ");
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println();

                     
}

// Function that uses the voltage on a USB port to validate if the computer is turned on
bool validateStatus(bool expectedStatus) {
  if (digitalRead(MEASURE) == expectedStatus) {
    return true;
  }
  return false;
}

// Function that emulates a press on the power button of the computer 
void switchComputer() {
  digitalWrite(RELAY, HIGH);
  delay(1000);
  digitalWrite(RELAY, LOW);
}
