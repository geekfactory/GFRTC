/*	Geek Factory GFRTC Library
	Copyright (C) 2018 Jesus Ruben Santa Anna Zamudio.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Author website: https://www.geekfactory.mx
	Author e-mail: ruben at geekfactory dot mx
 */
#include "GFRTC.h"

/*-------------------------------------------------------------*
 *		Class implementation				*
 *-------------------------------------------------------------*/
GFRTCClass::GFRTCClass()
{
	// Prepare I2C (moved this call out of the constructor) as it causes problems in some architectures
	//Wire.begin();
}

bool GFRTCClass::begin(bool begini2c)
{
	// Request to initialize I2C?
	if (begini2c) {
		Wire.begin();
	}
	// check for presence
	get();

	return exists;
}

timelib_t GFRTCClass::get()
{
	struct timelib_tm dt;
	
	// read information from RTC to structure
	if (read(dt) == false) {
		// return 0 if cannot communicate with RTC
		return 0;
	}
	
	// convert to timestamp
	return time_make(&dt);
}

bool GFRTCClass::set(timelib_t t)
{
	struct timelib_tm dt;
	
	// get human readable time information (as required by RTC chip)
	time_break(t, &dt);
	
	// enable CH bit for DS1307 chip
	dt.tm_sec |= 0x80;
	// write information on struct to hardware clock
	if (!write(dt))
		return false;
	// disable CH bit on DS1307
	dt.tm_sec &= 0x7f;
	if (!write(dt))
		return false;

	// return true if communication was successful
	return true;
}

bool GFRTCClass::read(struct timelib_tm &dt)
{
	uint8_t sec;

	Wire.beginTransmission(GFRTC_I2C_ADDRESS);
	Wire.write((uint8_t) 0x00);
	if (Wire.endTransmission() != 0) {
		exists = false;
		return false;
	}
	exists = true;
	
	// request the 7 data fields secs, min, hr, dow, date, mth, yr
	Wire.requestFrom(GFRTC_I2C_ADDRESS, 7);
	if (Wire.available() < 7)
		return false;
	
	// read result and convert from BCD
	sec = Wire.read();
	dt.tm_sec = bcd2dec(sec & 0x7f);
	dt.tm_min = bcd2dec(Wire.read());
	dt.tm_hour = bcd2dec(Wire.read() & 0x3f); // mask assumes 24hr clock
	dt.tm_wday = bcd2dec(Wire.read());
	dt.tm_mday = bcd2dec(Wire.read());
	dt.tm_mon = bcd2dec(Wire.read());
	dt.tm_year = time_y2k2tm((bcd2dec(Wire.read())));
	// If clock is halted, return false
	if (sec & 0x80)
		return false;
	return true;
}

bool GFRTCClass::write(struct timelib_tm &dt)
{
	Wire.beginTransmission(GFRTC_I2C_ADDRESS);
	// reset register pointer
	Wire.write((uint8_t) 0x00);
	// Write date / time information
	Wire.write(dec2bcd(dt.tm_sec));
	Wire.write(dec2bcd(dt.tm_min));
	Wire.write(dec2bcd(dt.tm_hour));
	Wire.write(dec2bcd(dt.tm_wday));
	Wire.write(dec2bcd(dt.tm_mday));
	Wire.write(dec2bcd(dt.tm_mon));
	Wire.write(dec2bcd(time_tm2y2k(dt.tm_year)));
	if (Wire.endTransmission() != 0) {
		exists = false;
		return false;
	}
	exists = true;
	return true;
}

bool GFRTCClass::writeNVRAM(uint16_t address, const void * buffer, uint16_t size)
{
	uint16_t i;
	const uint8_t * src = (uint8_t *) buffer;

	// Limit operation to valid range
	// Wire library cannot transfer more than 32 bytes at a time
	if (size > 32 || address + size >= 64)
		return false;

	Wire.beginTransmission(GFRTC_I2C_ADDRESS);
	// Write start address for NVRAM
	Wire.write((uint8_t) (0x08 + address));
	// Write data
	for (i = 0; i < size; i++) {
		Wire.write(src[i]);
	}
	// Check if communication was successful
	if (Wire.endTransmission() != 0)
		return false;
	return true;
}

bool GFRTCClass::readNVRAM(uint16_t address, void * buffer, uint16_t size)
{
	uint16_t i;
	uint8_t * dst = (uint8_t *) buffer;

	// Limit operation to valid range
	// Wire library cannot transfer more than 32 bytes at a time
	if (size > 32 || address + size >= 64)
		return false;

	Wire.beginTransmission(GFRTC_I2C_ADDRESS);
	// Add address offset for NVRAM start
	Wire.write((uint8_t) (0x08 + address));
	if (Wire.endTransmission() != 0)
		return false;
	// Begin read operation
	Wire.requestFrom(GFRTC_I2C_ADDRESS, size);
	if ((uint16_t) (Wire.available()) < size)
		return false;
	// Read data to buffer
	for (i = 0; i < size; i++) {
		dst[i] = Wire.read();
	}
	return true;
}

uint8_t GFRTCClass::dec2bcd(uint8_t num)
{
	return((num / 10 * 16) + (num % 10));
}

uint8_t GFRTCClass::bcd2dec(uint8_t num)
{
	return((num / 16 * 10) + (num % 16));
}

bool GFRTCClass::exists = false;

GFRTCClass GFRTC = GFRTCClass(); // create an instance for the user
