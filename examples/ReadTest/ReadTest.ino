#include <Wire.h>
#include <TimeLib.h>
#include <RTCLib.h>

void setup() {
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);

  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}

void loop() {
  struct tm datetime;

  if (RTC.read(datetime)) {
    Serial.print("Ok, Time = ");
    print2digits(datetime.tm_hour);
    Serial.write(':');
    print2digits(datetime.tm_min);
    Serial.write(':');
    print2digits(datetime.tm_sec);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(datetime.tm_mday);
    Serial.write('/');
    Serial.print(datetime.tm_mon);
    Serial.write('/');
    Serial.print(time_tm2y2k(datetime.tm_year));
    Serial.println();
  }
  else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    }
    else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
