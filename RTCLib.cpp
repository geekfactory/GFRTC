#include "RTCLib.h"

#define DS1307_ID 0x68

/*-------------------------------------------------------------*/
/*			Class implementation			*/
/*-------------------------------------------------------------*/

RTCLib::RTCLib()
{
	// Prepare I2C
	Wire.begin();
}

time_t RTCLib::get()
{
	struct tm dt;
	// Read information from RTC to structure
	if (read(dt) == false)
		return 0;
	// Convert to timestamp
	return(time_make(&dt));
}

bool RTCLib::set(time_t t)
{
	struct tm dt;
	// Get human readable time information
	time_break(t, &dt);
	// Write information on struct to hardware clock
	dt.tm_sec |= 0x80;
	write(dt);
	dt.tm_sec &= 0x7f;
	write(dt);
}

bool RTCLib::read(struct tm &dt)
{
	uint8_t sec;
	Wire.beginTransmission(DS1307_ID);
#if ARDUINO >= 100  
	Wire.write((uint8_t) 0x00);
#else
	Wire.send(0x00);
#endif  
	if (Wire.endTransmission() != 0) {
		exists = false;
		return false;
	}
	exists = true;

	// request the 7 data fields   (secs, min, hr, dow, date, mth, yr)
	Wire.requestFrom(DS1307_ID, 7);
	if (Wire.available() < 7) return false;
#if ARDUINO >= 100
	sec = Wire.read();
	dt.tm_sec = bcd2dec(sec & 0x7f);
	dt.tm_min = bcd2dec(Wire.read());
	dt.tm_hour = bcd2dec(Wire.read() & 0x3f); // mask assumes 24hr clock
	dt.tm_wday = bcd2dec(Wire.read());
	dt.tm_mday = bcd2dec(Wire.read());
	dt.tm_mon = bcd2dec(Wire.read());
	dt.tm_year = time_y2k2tm((bcd2dec(Wire.read())));
#else
	sec = Wire.receive();
	dt.tm_sec = bcd2dec(sec & 0x7f);
	dt.tm_min = bcd2dec(Wire.receive());
	dt.tm_hour = bcd2dec(Wire.receive() & 0x3f); // mask assumes 24hr clock
	dt.tm_wday = bcd2dec(Wire.receive());
	dt.tm_mday = bcd2dec(Wire.receive());
	dt.tm_mon = bcd2dec(Wire.receive());
	dt.tm_year = time_y2k2tm((bcd2dec(Wire.receive())));
#endif
	if (sec & 0x80) return false; // clock is halted
	return true;
}

bool RTCLib::write(struct tm &dt)
{
	Wire.beginTransmission(DS1307_ID);
#if ARDUINO >= 100  
	Wire.write((uint8_t) 0x00); // reset register pointer
	Wire.write(dec2bcd(dt.tm_sec));
	Wire.write(dec2bcd(dt.tm_min));
	Wire.write(dec2bcd(dt.tm_hour)); // sets 24 hour format
	Wire.write(dec2bcd(dt.tm_wday));
	Wire.write(dec2bcd(dt.tm_mday));
	Wire.write(dec2bcd(dt.tm_mon));
	Wire.write(dec2bcd(time_tm2y2k(dt.tm_year)));
#else  
	Wire.send(0x00); // reset register pointer
	Wire.send(dec2bcd(dt.tm_sec));
	Wire.send(dec2bcd(dt.tm_min));
	Wire.send(dec2bcd(dt.tm_hour)); // sets 24 hour format
	Wire.send(dec2bcd(dt.tm_wday));
	Wire.send(dec2bcd(dt.tm_mday));
	Wire.send(dec2bcd(dt.tm_mon));
	Wire.send(dec2bcd(time_tm2y2k(dt.tm_year)));
#endif
	if (Wire.endTransmission() != 0) {
		exists = false;
		return false;
	}
	exists = true;
	return true;
}

uint8_t RTCLib::dec2bcd(uint8_t num)
{
	return((num / 10 * 16) + (num % 10));
}

uint8_t RTCLib::bcd2dec(uint8_t num)
{
	return((num / 16 * 10) + (num % 16));
}

bool RTCLib::exists = false;

RTCLib RTC = RTCLib(); // create an instance for the user

