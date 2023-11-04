# Geek Factory GFRTC Library #

This is a library to read / write time and date information when using the very popular __DS1307__, __DS3231__ & __DS3232__ RTC chips from Dallas/maxim.

The library can provide time in standard structures or as a unix timestamp to cover a wide range of applications. Examples provide enough information to get started and the API is fully documented on the GFRTC.h file.

For more information regarding the basic usage and applications of this library please check the following website (spanish only, sorry):

https://www.geekfactory.mx/tutoriales-arduino/ds3231-el-reloj-en-tiempo-real-de-gran-precision-para-arduino/

Testing and evaluation of the chips was performed using the following modules:

* [Chinese DS3231 real time clock module](https://www.geekfactory.mx/tienda/modulos/generales/ds3231-modulo-reloj-en-tiempo-real-rtc/)
* [Chinese DS1307 tiny RTC module](https://www.geekfactory.mx/tienda/modulos/generales/ds1307-modulo-tiny-rtc-reloj-en-tiempo-real/)
* [DS3231 Adafruit precision RTC FeatherWing](https://www.geekfactory.mx/tienda/tarjetas/feather/adafruit-ds3231-precision-rtc-featherwing/)

## Basic library usage ##

Just include the library header file. The library **automatically creates an instance called "GFRTC"** which exposes all the funcionality of the RTC chip.

```cpp
#include <GFRTC.h>
```

In setup function call GFRTC.begin(true); This calls Wire.begin() (when the parameter pased to the funcion is set to true) to prepare the I2C interface if it is not done elsewhere.

```cpp
GFRTC.begin(true);
```

And then use the read / write methods to get or set time information as required:

```cpp
// structure to hold data from RTC
struct timelib_tm datetime;

// get date and time
if (GFRTC.read(datetime)) {
    // do something with date and time information now in struct
}

if (GFRTC.write(datetime)) {
    // write ok, print data sent to RTC
    Serial.print(F("Set date / time ok"));
} else {
    // error reading the RTC
  Serial.println(F("Cannot write RTC."));
}
```

Please see the examples folder for complete demo code.

## Project objectives ##

* Create a library that supports common RTC chips, including DS1307 & DS3231.
* Access full functionality of the targetted RTC chips.
* The library should provide support for setting / getting time on unix format.
* Compatibility with our lightweight time management library

## Supported devices ##

The library has been tested on the following hardware:

* Arduino UNO
* Arduino MEGA
* Arduino MKR1000

## Contact me ##

* Feel free to write for any inquiry: ruben at geekfactory.mx 
* Check our website: https://www.geekfactory.mx
