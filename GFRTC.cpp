/* GeekFactory GFRTC Library for Arduino
 * 
 * Copyright (C) 2017 Jesus Ruben Santa Anna Zamudio.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author website: https://www.geekfactory.mx
 * Author e-mail: ruben at geekfactory dot mx
 */

#include "GFRTC.h"

/*-------------------------------------------------------------*/
/*		Class implementation				*/
/*-------------------------------------------------------------*/
GFRTC::GFRTC()
{
	// Prepare I2C
	Wire.begin();
}

time_t GFRTC::get()
{
	struct tm dt;
	// Read information from RTC to structure
	if (read(dt) == false)
		return 0;
	// Convert to timestamp
	return time_make(&dt);
}

bool GFRTC::set(time_t t)
{
	struct tm dt;
	// Get human readable time information
	time_break(t, &dt);
	// Write information on struct to hardware clock
	dt.tm_sec |= 0x80;
	if (!write(dt))
		return false;
	dt.tm_sec &= 0x7f;
	if (!write(dt))
		return false;
	else
		return true;
}

bool GFRTC::read(struct tm &dt)
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
	// Read result and convert from BCD
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

bool GFRTC::write(struct tm &dt)
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

bool GFRTC::writeNVRAM(uint16_t address, const void * buffer, uint16_t size)
{
	uint16_t i;
	const uint8_t * src = (uint8_t *)buffer;

	// Limit operation to valid range
	// Wire library cannot transfer more than 32 bytes at a time
	if (size >= 32 || address + size > 56)
		return false;

	Wire.beginTransmission(GFRTC_I2C_ADDRESS);
	// Write start address for NVRAM
	Wire.write((uint8_t) (0x08 + address));
	// Write data
	for (i = 0; i < size; i++) {
		Wire.write(src[i]);
	}
	if (Wire.endTransmission() != 0)
		return false;
	return true;
}

bool GFRTC::readNVRAM(uint16_t address, void * buffer, uint16_t size)
{
	uint16_t i;
	uint8_t * dst = (uint8_t *)buffer;

	// Wire library cannot transfer more than 32 bytes at a time
	if (size >= 32 || address + size > 56)
		return false;

	Wire.beginTransmission(GFRTC_I2C_ADDRESS);
	// Add address offset for NVRAM start
	Wire.write((uint8_t) (0x08 + address));

	if (Wire.endTransmission() != 0)
		return false;

	Wire.requestFrom(GFRTC_I2C_ADDRESS, size);
	if ((uint16_t) (Wire.available()) < size)
		return false;

	for (i = 0; i < size; i++) {
		dst[i] = Wire.read();
	}
	return true;
}

uint8_t GFRTC::dec2bcd(uint8_t num)
{
	return((num / 10 * 16) + (num % 10));
}

uint8_t GFRTC::bcd2dec(uint8_t num)
{
	return((num / 16 * 10) + (num % 16));
}

bool GFRTC::exists = false;

GFRTC RTC = GFRTC(); // create an instance for the user

