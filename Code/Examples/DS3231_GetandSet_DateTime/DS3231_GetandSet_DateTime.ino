#include <DS3231.h>

DS3231 rtc(SDA, SCL);
Time t;

void setup() {
  Serial.begin(115200);
  rtc.begin();
  // The following lines can be uncommented to set the date and time
//  rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
//  rtc.setTime(18, 58, 40);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(3, 5, 2020);   // Set the date to January 1st, 2014
}

void loop() {
  t = rtc.getTime();
  // Send date over serial connection
  Serial.print("Today is the ");
  Serial.print(t.date, DEC);
  Serial.print(". day of ");
  Serial.print(rtc.getMonthStr());
  Serial.print(" in the year ");
  Serial.print(t.year, DEC);
  Serial.println(".");
  // Send Day-of-Week and time
  Serial.print("It is the ");
  Serial.print(t.dow, DEC);
  Serial.print(". day of the week (counting monday as the 1th), and it has passed ");
  Serial.print(t.hour, DEC);
  Serial.print(" hour(s), ");
  Serial.print(t.min, DEC);
  Serial.print(" minute(s) and ");
  Serial.print(t.sec, DEC);
  Serial.println(" second(s) since midnight.");
  // Send a divider for readability
  Serial.println("  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -");
  // Wait one second before repeating :)
  delay (1000);
}
