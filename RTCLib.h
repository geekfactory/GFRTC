#ifndef RTCLIB_h
#define RTCLIB_h

/*-------------------------------------------------------------*/
/*		Includes and dependencies			*/
/*-------------------------------------------------------------*/
#include <Wire.h>
#include <TimeLib.h>

/*-------------------------------------------------------------*/
/*			Class declaration			*/
/*-------------------------------------------------------------*/

class RTCLib {
public:
	RTCLib();
	static time_t get();
	static bool set(time_t t);
	static bool read(struct tm &dt);
	static bool write(struct tm &dt);

	static bool chipPresent()
	{
		return exists;
	}

private:
	static bool exists;
	static uint8_t dec2bcd(uint8_t num);
	static uint8_t bcd2dec(uint8_t num);
};

#ifdef RTC
#undef RTC //workaround for Arduino Due
#endif

extern RTCLib RTC;

#endif
// End of Header file
