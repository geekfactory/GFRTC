/**
   GeekFactory - "INNOVATING TOGETHER"
   Distribucion de materiales para el desarrollo e innovacion tecnologica
   www.geekfactory.mx

   Example that shows how to configure the DS3231 interrupt / square wave
   output pin on DS3231 and DS3232 devices.

   This code configures the INT/SQW pin to output a 1 Hz signal and uses an
   interrupt to read the date / time from RTC.
*/
#include <GFRTC.h>

// Flag used to indicate that DS3231 generated an interrupt
volatile bool rtcflag = false;

void setup() {
  // prepare serial interface
  Serial.begin(115200);
  while (!Serial);

  // show message on serial monitor
  Serial.println(F("----------------------------------------------------"));
  Serial.println(F("             GFRTC LIBRARY TEST PROGRAM             "));
  Serial.println(F("             https://www.geekfactory.mx             "));
  Serial.println(F("----------------------------------------------------"));

  // prepare the GFRTC class, this also calls Wire.begin()
  GFRTC.begin(true);

  // check if we can communicate with RTC
  if (GFRTC.isPresent()) {
    Serial.println(F("RTC connected and ready."));
  } else {
    Serial.println(F("Check RTC connections and try again."));
    for (;;);
  }

  // enable sqrwave output with 1 Hz frequency
  if (GFRTC.setIntSqwMode(E_SQRWAVE_1_HZ)) {
    Serial.println(F("Set square wave output OK"));
  }

  // use other values on the gfrtc_intsqw_modes enum to configure different
  // frequencies or use the pin as interrupt output for alarms, for example:
  // GFRTC.setIntSqwMode(E_SQRWAVE_1024_HZ);
  // GFRTC.setIntSqwMode(E_SQRWAVE_4096_HZ);
  // GFRTC.setIntSqwMode(E_SQRWAVE_8192_HZ);
  // GFRTC.setIntSqwMode(E_INTERRUPT_OUTPUT);
  // Note that DS3231M chips can only output 1 Hz frequency, on DS3231 you
  // can configure the output frequency.

  // attach interrupt, this calls isrhandler function on every falling edge
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), isrhandler, FALLING);
}

void loop()
{
  // check if we received interrupt signal
  if (rtcflag) {
    // clear flag and display time
    rtcflag = false;
    readAndDisplayTime();
  }
}

/**
   Sets a flag when interrupt signal is received from DS3231
*/
void isrhandler()
{
  rtcflag = true;
}

/**
   Prints date and time to serial monitor
*/
void readAndDisplayTime() {
  // structure to hold data from RTC
  struct timelib_tm datetime;

  Serial.print(F("Got interrupt signal, time is = "));

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
