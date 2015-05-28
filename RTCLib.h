/*	Arduino Library for DS1307 and similar Real Time Clock ICs
	Copyright (C) 2015 Jesus Ruben Santa Anna Zamudio.

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

	Author website: http://www.geekfactory.mx
	Author e-mail: ruben at geekfactory dot mx
 */
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
	/**
	 * Class constructor
	 */
	RTCLib();

	/**
	 * Reads the RTC time/date registers and converts the value to a unix timestamp
	 *
	 * This method reads the RTC date/time registers and performs the conversion
	 * to a 32 bit integer value representing the number of seconds elapsed since
	 * 00:00 hours, Jan 1, 1970 UTC (a unix timestamp).
	 *
	 * @return A Unix timestamp representing the number of seconds elapsed since
	 * 00:00 hours, Jan 1, 1970 UTC to the present date.
	 */
	static time_t get();

	/**
	 * Writes the RTC time/date registers with the value provided as unix timestamp
	 *
	 * This method writes the RTC date/time registers. It performs the conversion
	 * from a 32 bit integer value representing the number of seconds elapsed since
	 * 00:00 hours, Jan 1, 1970 UTC (a unix timestamp).
	 *
	 * @param t The timestamp corresponding to current time
	 *
	 * @return Return true if successfully written data to RTC chip
	 */
	static bool set(time_t t);

	/**
	 * Read the RTC time/date registers to structure
	 *
	 * This method reads the RTC date/time registers and performs the conversion
	 * to a tm struct from raw BCD encoded data.
	 *
	 * @param dt Reference to a tm struct to write data from RTC
	 *
	 * @return Returns true if communication is successfull, false otherwise
	 */
	static bool read(struct tm &dt);

	/**
	 * Write the RTC time/date registers from structure
	 *
	 * This method writes the RTC date/time registers, data is provided in a tm
	 * struct reference, then encoded to BCD and written to RTC registers.
	 *
	 * @param dt Reference to a tm struct that holds data to write to RTC
	 *
	 * @return Returns true if communication is successfull, false otherwise
	 */
	static bool write(struct tm &dt);

	/**
	 * Write to RTC chip general purpose NVRAM
	 *
	 * @param addr The address to write to
	 * @param src Pointer to buffer containing data to write
	 * @param size Size of the data to write
	 *
	 * @return Returns true if communication is successfull, false otherwise
	 */
	static bool write_nvram(uint16_t addr, uint8_t * src, uint16_t size);

	/**
	 * Read from RTC chip general purpose NVRAM
	 *
	 * @param addr The address to read from
	 * @param dst Pointer to buffer to put data comming from RTC
	 * @param size Size of the data to transfer
	 *
	 * @return Returns true if communication is successfull, false otherwise
	 */
	static bool read_nvram(uint16_t addr, uint8_t * dst, uint16_t size);

	/**
	 * Returns true if chip is present on I2C bus
	 */
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
