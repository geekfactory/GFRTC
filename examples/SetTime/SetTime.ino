/**
 * GeekFactory - "Construye tu propia tecnologia"
 * Distribucion de materiales para el desarrollo e innovacion tecnologica
 * www.geekfactory.mx
 *
 * Ejemplo para ajustar la fecha y hora del modulo RTC con la hora del compilador.
 * Requiere el reloj en tiempo real (RTC) DS1307 conectado en el bus I2C.
 * Requiere la instalacion de nuestra libreria TimeLib con funciones de tiempo.
 */

#include <Wire.h>
#include <TimeLib.h>
#include <RTCLib.h>

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

// Declaramos una estructura "tm" llamada datetime para almacenar 
// La fecha y la hora actual obtenida desde el chip RTC.
struct tm datetime;

void setup() {
  // Preparamos la comunicacion serial  
  Serial.begin(9600);
  while (!Serial);
  delay(200);

  bool parse=false;
  bool config=false;

  // Obtener la fecha y la hora en la que se ejecuta el compilador
  // Y convertirla al formato de la estructura tm para poderla manipular
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // Escribir los datos al reloj en tiempo real
    if (RTC.write(datetime)) {
      config = true;
    }
  }

  if (parse && config) {
    // Se obtuvo el tiempo del compilador y se configuro correctamente
    Serial.print("DS1307 Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  }
  else if (parse) {
    // Error al comunicarse con el DS1307, revisar conexiones
    Serial.println("DS1307 error");
    Serial.println("Check circuit.");
  }
  else {
    // No se puede interpretar la fecha del compilador
    Serial.print("Cannot obtain compiler date & time");
    Serial.print("Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }
}

void loop() {
  // No hacemos nada en el loop principal
}

/**
 * Obtiene el tiempo desde la cadena de hora suministrada por el compilador
 * y la almacena en la estructura datetime.
 */
bool getTime(const char *str)
{
  int hour, minute, second;

  if (sscanf(str, "%d:%d:%d", &hour, &minute, &second) != 3) return false;
  datetime.tm_hour = hour;
  datetime.tm_min = minute;
  datetime.tm_sec = second;
  return true;
}

/**
 * Obtiene la fecha desde la cadena de fecha suministrada por el compilador
 * y la almacena en la estructura datetime.
 */
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



