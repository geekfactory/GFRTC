/**
   GeekFactory - "INNOVATING TOGETHER"
   Distribucion de materiales para el desarrollo e innovacion tecnologica
   www.geekfactory.mx

   This example shows how to read the internal temperature sensor on the
   DS3231. The reading is displayed in a serial monitor window.
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

  // prepare the GFRTC class, this also calls Wire.begin()
  GFRTC.begin(true);

  // check if we can communicate with RTC
  if (GFRTC.isPresent()) {
    Serial.println(F("RTC connected and ready."));
  } else {
    Serial.println(F("Check RTC connections and try again."));
    for (;;);
  }
}

void loop()
{
  // reads temperature to variable
  int temperature = GFRTC.getTemperature();

  // print to serial monitor
  Serial.print(F("Temperature: "));
  Serial.println(temperature);

  // wait before next reading
  delay(1000);
}

