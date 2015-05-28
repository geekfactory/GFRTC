/**
 * GeekFactory - "Construye tu propia tecnologia"
 * Distribucion de materiales para el desarrollo e innovacion tecnologica
 * www.geekfactory.mx
 *
 * Ejemplo de libreria RTCLib para lectura del valor fecha y hora almacenado.
 * Requiere el reloj en tiempo real (RTC) DS1307 conectado en el bus I2C.
 * Requiere la instalacion de nuestra libreria TimeLib con funciones de tiempo.
 */

#include <Wire.h>
#include <TimeLib.h>
#include <RTCLib.h>

void setup() {
  // NOTA: Estas instrucciones se requieren para alimentar directamente el
  // chip RTC desde los pines A3 Y A2 (colocar directamente el modulo sobre
  // la tarjeta arduino, sin la necesidad de cablear en Arduino UNO).
  // Si no se quiere hacer esto, se pueden eliminar o comentar sin problemas
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);

  //Iniciar el  puerto serie y esperar a que este listo (Leonardo)
  Serial.begin(9600);
  while (!Serial); // wait for serial
  delay(200);

  // Mostrar la leyenda inicial
  Serial.println("www.geekfactory.mx ");
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}

void loop() {
  // Declaramos una estructura "tm" llamada datetime para almacenar
  // La fecha y la hora actual obtenida desde el chip RTC.
  struct tm datetime;

  // Obtener la fecha y hora desde el chip RTC
  if(RTC.read(datetime)) {

    // Comenzamos a imprimir la informacion
    Serial.print("OK, Time = ");
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
    delay(1000);
  }
  else {
    if (RTC.chipPresent()) {
      // El reloj esta detenido, es necesario ponerlo a tiempo
      Serial.println("The DS1307 is stopped.");
      Serial.println("Run timeset example.");
      Serial.println();
    }
    else {
      // No se puede comunicar con el RTC en el bus I2C, revisar las conexiones.
      Serial.println("DS1307 Not Detected.");
      Serial.println("Check hardware connections and reset board.");
      Serial.println();
    }
    // Esperamos indefinidamente
    for(;;);
  }
}

/**
 * Funcion auxiliar para imprimir siempre 2 digitos
 */
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
