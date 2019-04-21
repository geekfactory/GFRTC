/**
   GeekFactory - "INNOVATING TOGETHER"
   Distribucion de materiales para el desarrollo e innovacion tecnologica
   www.geekfactory.mx

   Basic example for GFRTC, this example shows how to write date/time to RTC
   using a timelib_tm structure and display it on the serial terminal.

   The time and date is always set to the same value provided during compilation.
*/
#include <GFRTC.h>

void setup() {
  // prepare serial interface
  Serial.begin(115200);
  while (!Serial);

  // show message on serial monitor
  Serial.println(F("----------------------------------------------------"));
  Serial.println(F("             GFRTC LIBRARY TEST PROGRAM             "));
  Serial.println(F("             https://www.geekfactory.mx             "));
  Serial.println(F("----------------------------------------------------"));

  // prepare the RTC class, this also calls Wire.begin()
  GFRTC.begin(true);

  // check if we can communicate with RTC
  if (GFRTC.isPresent()) {
    Serial.println(F("RTC connected and ready."));
  } else {
    Serial.println(F("Check RTC connections and try again."));
    for (;;);
  }

  // structure to hold data from RTC
  struct timelib_tm datetime;

  // use the datetime structure to group date time information
  // SET THE DESIRED TIME / DATE HERE
  datetime.tm_hour = 12;
  datetime.tm_min = 0;
  datetime.tm_sec = 0;
  datetime.tm_wday = 7;
  datetime.tm_mday = 20;
  datetime.tm_mon = 4;
  datetime.tm_year = timelib_y2k2tm(19);

  // write data on struct to RTC registers
  if (GFRTC.write(datetime)) {
    // write ok, print data sent to RTC
    Serial.print(F("Set date / time to: "));
    print2digits(datetime.tm_hour);
    Serial.write(':');
    print2digits(datetime.tm_min);
    Serial.write(':');
    print2digits(datetime.tm_sec);
    Serial.print(F(", Date (D/M/Y) = "));
    Serial.print(datetime.tm_mday);
    Serial.write('/');
    Serial.print(datetime.tm_mon);
    Serial.write('/');
    Serial.print(timelib_tm2y2k(datetime.tm_year));
    Serial.println();
  } else {
    // error reading the RTC
    Serial.println(F("Cannot write RTC."));
  }
}

void loop() {
  // do nothing on main loop
}

/**
   Helper function to print always two digits
*/
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
