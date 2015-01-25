#include <Wire.h>
#include <TimeLib.h>
#include <RTCLib.h>

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

struct tm datetime;

void setup() {
  bool parse=false;
  bool config=false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(datetime)) {
      config = true;
    }
  }

  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(200);
  if (parse && config) {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  }
  else if (parse) {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  }
  else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }
}

void loop() {
}

bool getTime(const char *str)
{
  int hour, minute, second;

  if (sscanf(str, "%d:%d:%d", &hour, &minute, &second) != 3) return false;
  datetime.tm_hour = hour;
  datetime.tm_min = minute;
  datetime.tm_sec = second;
  return true;
}

bool getDate(const char *str)
{
  char month[12];
  int day, year;

  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", month, &day, &year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  datetime.tm_mday = day;
  datetime.tm_mon = monthIndex + 1;
  datetime.tm_year = time_calendar2tm(year);
  return true;
}

