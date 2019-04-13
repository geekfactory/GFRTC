# Geek Factory GFRTC Library #

This is a library to read / write time and date information when using the very popular __DS1307__, __DS3231__ & __DS3232__ RTC chips from dallas/maxim. The library can provide time in standard structures or as a unix timestamp to cover a wide range of applications.

The API of this library is designed to be very similar to other libaries for handling RTC communications on the  Arduino ecosystem, so migration from another library should be fairly straightforward.

Examples provide enough information to get started and the API is fully documented on the GFRTC.h file.

## Basic library usage ##

```cpp
/**
   GeekFactory - "INNOVATING TOGETHER"
   Distribucion de materiales para el desarrollo e innovacion tecnologica
   www.geekfactory.mx

   Basic example for GFRTC, this example shows how to read date/time from RTC
   and display it on the serial terminal.
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
}

void loop() {
  // structure to hold data from RTC
  struct timelib_tm datetime;

  // get date and time
  if (GFRTC.read(datetime)) {
    // read ok, print data from RTC
    Serial.print(F("OK, Time = "));
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
    Serial.print(time_tm2y2k(datetime.tm_year));
    Serial.println();
  } else {
    // error reading the RTC
    Serial.println(F("Cannot read RTC."));
  }

  // wait a second before reading again
  delay(1000);
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
```

## Project objectives ##

* Create a library that supports common RTC chips, including DS1307, DS3231 & DS3232.
* Access full functionality of the targetted RTC chips.
* The library should provide support for setting / getting time on unix format.
* API design should allow easy migration to / from existing libraries.

## Supported devices ##

The library has been tested on the following hardware:

* Arduino UNO
* Arduino MEGA
* Arduino MKR1000

## Contact me ##

* Feel free to write for any inquiry: ruben at geekfactory.mx 
* Check our website: https://www.geekfactory.mx
