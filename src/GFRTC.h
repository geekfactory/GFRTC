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
#include <TimeLib.h>

/*-------------------------------------------------------------*
 *		Library configuration				*
 *-------------------------------------------------------------*/

/**
 * Version string, should be changed in every release
 */
#define GFRTC_VERSION_STRING	"2.0.0"

/*-------------------------------------------------------------*
 *		Macros and definitions				*
 *-------------------------------------------------------------*/

/**
 * I2C address of the RTC chip
 */
#define GFRTC_I2C_ADDRESS	0x68

/**
 * registers addresses
 */
#define GFRTC_REG_SECONDS 0x00
#define GFRTC_REG_MINUTES 0x01
#define GFRTC_REG_HOURS 0x02
#define GFRTC_REG_DAY 0x03
#define GFRTC_REG_DATE 0x04
#define GFRTC_REG_MONTH 0x05
#define GFRTC_REG_YEAR 0x06
#define GFRTC_REG_ALM1_SECONDS 0x07
#define GFRTC_REG_ALM1_MINUTES 0x08
#define GFRTC_REG_ALM1_HOURS 0x09
#define GFRTC_REG_ALM1_DAYDATE 0x0A
#define GFRTC_REG_ALM2_MINUTES 0x0B
#define GFRTC_REG_ALM2_HOURS 0x0C
#define GFRTC_REG_ALM2_DAYDATE 0x0D
#define GFRTC_REG_CONTROL 0x0E
#define	GFRTC_REG_STATUS 0x0F
#define GFRTC_REG_AGING 0x10
#define GFRTC_REG_MSB_TEMP 0x11
#define GFRTC_REG_LSB_TEMP 0x12

/**
 * size for SRAM
 */
#define SRAM_START_ADDR 0x14
#define SRAM_SIZE 236

/**
 * alarm bits
 */
#define GFRTC_BIT_A1M1 7
#define GFRTC_BIT_A1M2 7
#define GFRTC_BIT_A1M3 7
#define GFRTC_BIT_A1M4 7
#define GFRTC_BIT_A2M2 7
#define GFRTC_BIT_A2M3 7
#define GFRTC_BIT_A2M4 7

/**
 * control register bits
 */
#define GFRTC_BIT_EOSC 7
#define GFRTC_BIT_BBSQW 6
#define GFRTC_BIT_CONV 5
#define GFRTC_BIT_RS2 4
#define GFRTC_BIT_RS1 3
#define GFRTC_BIT_INTCN 2
#define GFRTC_BIT_A2IE 1
#define GFRTC_BIT_A1IE 0

/**
 * status register bits
 */
#define GFRTC_BIT_OSF 7
#define GFRTC_BIT_BB32KHZ 6
#define GFRTC_BIT_CRATE1 5
#define GFRTC_BIT_CRATE0 4
#define GFRTC_BIT_EN32KHZ 3
#define GFRTC_BIT_BSY 2
#define GFRTC_BIT_A2F 1
#define GFRTC_BIT_A1F 0

/**
 * other bit definitions
 */
#define GFRTC_BIT_DS1307_CH 7
#define GFRTC_BIT_HR1224 6
#define GFRTC_BIT_CENTURY 7
#define GFRTC_BIT_DYDT 6

/*-------------------------------------------------------------*
 *		Typedefs enums & structs			*
 *-------------------------------------------------------------*/

/**
 * Defines the available types of alarms
 */
enum gfrtc_alarm_types {
	E_ALM1_EVERY_SECOND = 0x0F,
	E_ALM1_MATCH_SECONDS = 0x0E,
	E_ALM1_MATCH_MINUTES = 0x0C,
	E_ALM1_MATCH_HOURS = 0x08,
	E_ALM1_MATCH_DATE = 0x00,
	E_ALM1_MATCH_DAY = 0x10,
	E_ALM2_EVERY_MINUTE = 0x8E,
	E_ALM2_MATCH_MINUTES = 0x8C,
	E_ALM2_MATCH_HOURS = 0x88,
	E_ALM2_MATCH_DATE = 0x80,
	E_ALM2_MATCH_DAY = 0x90,
};

/**
 * Square wave output selection
 */
enum gfrtc_intsqw_modes {
	E_SQRWAVE_1_HZ = 0,
	E_SQRWAVE_1024_HZ,
	E_SQRWAVE_4096_HZ,
	E_SQRWAVE_8192_HZ,
	E_INTERRUPT_OUTPUT,
};

/**
 * Defines the available alarms available on the RTC
 */
enum gfrtc_alarms {
	E_ALARM_1,
	E_ALARM_2
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
	 * Reads a register on the indicated address.
	 * 
	 * @param addr The address of the register to read.
	 * @param result Optional pointer to bool variable where the result of the
	 * operation is stored, the method writes true of operation successful.
	 * 
	 * @return Returns the value of the indicated register.
	 */
	static uint8_t readRegister(uint8_t addr, bool * result = NULL);

	/**
	 * Reads multiple registers starting at the indicated address storing the
	 * received data on the indicated buffer.
	 * 
	 * @param addr The address of the first register to read.
	 * @param data Pointer to memory where recevied data will be stored.
	 * @param size The number of bytes to read.
	 * 
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool readRegister(uint8_t addr, void * data, uint8_t size);

	/**
	 * Writes a register on the indicated address with the given value.
	 * 
	 * @param addr The address of the register to write.
	 * @param value The value to write on the register.
	 * 
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool writeRegister(uint8_t addr, uint8_t value);

	/**
	 * Writes multiple registers starting at the indicated address with the
	 * given data.
	 * 
	 * @param addr The address of the first register to write.
	 * @param data Pointer to data that will be written to registers.
	 * @param size The number of bytes to write.
	 * 
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool writeRegister(uint8_t addr, const void * data, uint8_t size);
	
	/**
	 * Reads the status of a bit on an specific register.
	 * 
         * @param addr The address of the register.
         * @param bit The bit number to set or clear.
         * @param result Optional pointer to buffer to store the result of the operation.
	 * 
         * @return The status of the requested bit.
         */
	static bool readBit(uint8_t addr, uint8_t bit, bool * result = NULL);
	
	/**
	 * Sets or clears a bit on the indicated register.
	 * 
         * @param addr The address of the register.
         * @param bit The bit number to set or clear.
         * @param value The value to write on the selected bit.
	 * 
         * @return Returns true if communication is successfull, false otherwise.
         */
	static bool writeBit(uint8_t addr, uint8_t bit, bool value);

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
	 * Enables the square wave output on the RTC.
	 * 
	 * @param frequency The operating frequency of the square wave output.
	 * 
	 * @return Returns true if communication is successfull, false otherwise.
	 */
	static bool setIntSqwMode(enum gfrtc_intsqw_modes frequency);

	/**
	 * Determines the alarm that generated an interrupt.
	 * 
	 * This method can also be used to poll the interrupt flags even if no interrupt
	 * output is configured.
	 * 
	 * @return The number of the alarm that requested an interrupt.
	 */
	static bool getAlarmInterruptFlag(enum gfrtc_alarms alarm);

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
	static bool getOscillatorStopFlag(bool clearosf = false);

	/**
	 * Reads the RTC´s internal temperature sensor.
	 * 
	 * @return The temperature measured by internal temperature sensor.
	 */
	static int16_t getTemperature();

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
	static bool readNVRAM(uint8_t address, void * buffer, uint16_t size);

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
	static bool writeNVRAM(uint8_t address, const void * buffer, uint16_t size);

	/**
	 * Checks if the library is able to talk to the RTC chip over the I2C bus.
	 * 
	 * @return Returns true if the library is able to communicate with the RTC
	 * chip. If the chip doesn´t responds or is not connected this will return
	 * false.
	 */
	static bool isPresent();

private:
	/**
	 * This variable is set to true when the communication is successful.
	 */
	static bool _isPresent;

	/**
	 * Used internally to convert from binary to BCD.
	 */
	static uint8_t dec2bcd(uint8_t num);

	/**
	 * Used internally to convert from BCD to binary.
	 */
	static uint8_t bcd2dec(uint8_t num);
};

/**
 * Instance of the GFRTCClass as declared in GFRTC.cpp
 */
extern GFRTCClass GFRTC;

#endif
// End of Header file
