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
#ifndef GFRTC_H
#define GFRTC_H

/*-------------------------------------------------------------*
 *		Includes and dependencies			*
 *-------------------------------------------------------------*/
#include <Wire.h>
#include "TimeLib.h"

/*-------------------------------------------------------------*
 *		Library configuration				*
 *-------------------------------------------------------------*/

/**
 * Version string, should be changed in every release
 */
#define GFRTC_VERSION_STRING     "2.0.0"

/**
 * I2C address of the RTC chip
 */
#define GFRTC_I2C_ADDRESS        0x68

/*-------------------------------------------------------------*
 *		Macros and definitions				*
 *-------------------------------------------------------------*/


/*-------------------------------------------------------------*
 *		Typedefs enums & structs			*
 *-------------------------------------------------------------*/

/**
 * Defines the available types of alarms
 */
enum gfrtc_alarm_types {
	ALM1_EVERY_SECOND = 0x0F,
	ALM1_MATCH_SECONDS = 0x0E,
	ALM1_MATCH_MINUTES = 0x0C,
	ALM1_MATCH_HOURS = 0x08,
	ALM1_MATCH_DATE = 0x00,
	ALM1_MATCH_DAY = 0x10,
	ALM2_EVERY_MINUTE = 0x8E,
	ALM2_MATCH_MINUTES = 0x8C,
	ALM2_MATCH_HOURS = 0x88,
	ALM2_MATCH_DATE = 0x80,
	ALM2_MATCH_DAY = 0x90,
};

/**
 * Square wave output selection
 */
enum gfrtc_wave_frequencies {
	SQWAVE_1_HZ,
	SQWAVE_1024_HZ,
	SQWAVE_4096_HZ,
	SQWAVE_8192_HZ,
	SQWAVE_NONE
};

/**
 * Defines the available alarms available on the RTC
 */
enum gfrtc_alarms {
	NONE,
	ALARM_1,
	ALARM_2
};

/*-------------------------------------------------------------*
 *		Class declaration				*
 *-------------------------------------------------------------*/
class GFRTCClass {
public:
	GFRTCClass();

	/**
	 * Prepares the GFRTC library for use, if parameter is set to true, also
	 * initializes the underying I2C interface.
	 * 
	 * @param begini2c Parameter that indicates if we want to intialize the Wire
	 * library on this call (calls Wire.begin() if set to true).
	 * 
	 * @return Returns true if communication with I2C RTC is successfull.
	 */
	static bool begin(bool beginI2C = true);

	/**
	 * Reads the RTC time/date registers and converts the value to a unix timestamp.
	 *
	 * This method reads the RTC date/time registers and performs the conversion
	 * to a 32 bit integer value representing the number of seconds elapsed since
	 * 00:00 hours, Jan 1, 1970 UTC (a unix timestamp).
	 * 
	 * If the library fails to obtain date/time from RTC, this will return 0.
	 *
	 * @return A Unix timestamp representing the number of seconds elapsed since
	 * 00:00 hours, Jan 1, 1970 UTC to the present date.
	 */
	static timelib_t get();

	/**
	 * Writes the RTC time/date registers with the value provided as unix timestamp.
	 *
	 * This method writes the RTC date/time registers. It performs the conversion
	 * from a 32 bit integer value representing the number of seconds elapsed since
	 * 00:00 hours, Jan 1, 1970 UTC (a unix timestamp).
	 *
	 * @param t The timestamp corresponding to current time.
	 *
	 * @return Return true if successfully written data to RTC chip.
	 */
	static bool set(timelib_t t);

	/**
	 * Read the RTC time/date registers to structure.
	 *
	 * This method reads the RTC date/time registers and performs the conversion
	 * to a timelib_tm struct from raw BCD encoded data.
	 *
	 * @param dt Reference to a timelib_tm struct to write data from RTC.
	 *
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool read(struct timelib_tm &dt);

	/**
	 * Write the RTC time/date registers from structure.
	 *
	 * This method writes the RTC date/time registers, data is provided in a
	 * timelib_tm struct reference, then encoded to BCD and written to RTC
	 * registers.
	 *
	 * @param dt Reference to a timelib_tm struct that holds data to write to RTC.
	 *
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool write(struct timelib_tm &dt);

	/**
	 * Writes general purpose NVRAM on the RTC chip. This only works on RTC chips
	 * that have built-in NVRAM.
	 *
	 * @param address The address to write to.
	 * @param buffer Pointer to buffer containing data to write on NVRAM.
	 * @param size Size of the data to transfer.
	 *
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool writeNVRAM(uint16_t address, const void * buffer, uint16_t size);

	/**
	 * Reads from general purpose NVRAM on the RTC chip. This only works on RTC chips
	 * that have built-in NVRAM.
	 *
	 * @param address The address to read from.
	 * @param buffer Pointer where data from NVRAM should be stored.
	 * @param size Size of the data to transfer.
	 *
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool readNVRAM(uint16_t address, void * buffer, uint16_t size);

	/**
	 * Configures an alarm on the RTC, this method writes to the Alarm 1 or Alarm 2
	 * registers according to the type paramter.
	 * 
	 * @param type The type of alarm to configure according to gfrtc_alarm_types ennumeration.
	 * @param hour The hour of the alarm.
	 * @param minute The minute of the alarm.
	 * @param second The second of the alarm.
	 * @param dow The day of the week for the alarm.
	 * 
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool setAlarm(enum gfrtc_alarm_types type, uint8_t hour, uint8_t minute, uint8_t second, uint8_t dow);

	/**
	 * Configures the interrupt to drive the corresponding pin on the RTC chip.
	 * 
	 * @param alarm The alarm number to configure.
	 * @param enable Enable / disable interrupt for specified alarm.
	 * 
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool setAlarmInterrupt(enum gfrtc_alarms alarm, bool enable);

	/**
	 * Determines the alarm that generated an interrupt.
	 * 
	 * This method can also be used to poll the interrupt flags even if no interrupt
	 * output is configured.
	 * 
	 * @return The number of the alarm that requested an interrupt.
	 */
	static enum gfrtc_alarms getAlarmInterrupt();

	/**
	 * Enables the square wave output on the RTC.
	 * 
	 * @param frequency The operating frequency of the square wave output.
	 * 
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool setSquareWave(enum gfrtc_wave_frequencies frequency);

	/**
	 * Reads the flag that indicates that the oscillator failed. If this flag is 
	 * set, it might indicate that the oscillator stopped at some time and the time
	 * may no longer be valid.
	 * 
	 * @param clearosf If set to true, this call clears the oscillator fail bit after
	 * reading it.
	 * 
	 * @return Returns true if oscillator stopped at some time, false if oscillator
	 * operation is normal.
	 */
	bool getOscStoppedFlag(bool clearosf = false);

	/**
	 * Reads the RTC´s internal temperature sensor.
	 * 
	 * @return The temperature measured by internal temperature sensor.
	 */
	static int16_t getTemperature();

	/**
	 * Checks if the library is able to talk to the RTC chip over the I2C bus.
	 * 
	 * @return Returns true if the library is able to communicate with the RTC
	 * chip. If the chip doesn´t responds or is not connected this will return
	 * false.
	 */
	static bool chipIsPresent();

private:
	static bool exists;
	static uint8_t dec2bcd(uint8_t num);
	static uint8_t bcd2dec(uint8_t num);
};


extern GFRTCClass GFRTC;

#endif
// End of Header file
