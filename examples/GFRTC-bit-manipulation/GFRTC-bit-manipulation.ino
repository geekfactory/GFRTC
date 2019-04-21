/**
   GeekFactory - "INNOVATING TOGETHER"
   Distribucion de materiales para el desarrollo e innovacion tecnologica
   www.geekfactory.mx

   This example shows how manipulate bits on the RTC registers, this is
   useful to access some advanced functionality that is not implemented
   by the main API.
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

  // enable 1 Hz square wave
  GFRTC.setIntSqwMode(E_SQRWAVE_1_HZ);
  // enable battery backed square wave output
  GFRTC.writeBit(GFRTC_REG_STATUS, GFRTC_BIT_BBSQW, true);
  // disable 32 KHz output
  GFRTC.writeBit(GFRTC_REG_STATUS, GFRTC_BIT_EN32KHZ, false);
}

void loop()
{
  // toggle 32 Khz output on and off
  GFRTC.writeBit(GFRTC_REG_STATUS, GFRTC_BIT_EN32KHZ, true);
  delay(1000);
  GFRTC.writeBit(GFRTC_REG_STATUS, GFRTC_BIT_EN32KHZ, false);
  delay(1000);
}
