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

  // Setting the time using unix timestamp
  Serial.println(F("Setting time to Wednesday, 01/Jan/20 14:10:30 (1577887830)"));
  GFRTC.set(1577887830);
}

void loop() {
  // read unix time
  timelib_t ut = GFRTC.get();

  // print unix time and then human readable time
  Serial.print(ut);
  Serial.print(F(" unix equals "));
  readAndDisplayTime();

  // wait a second before reading again
  delay(1000);
}

/**
   Prints date and time to serial monitor
*/
void readAndDisplayTime() {
  // structure to hold data from RTC
  struct timelib_tm datetime;

  // get date and time
  if (GFRTC.read(datetime)) {
    // read ok, print data from RTC
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
    Serial.println(F("Cannot read RTC."));
  }
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
