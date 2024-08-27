#include "config.h"

int count = 0;

AdafruitIO_Feed *CanalUltrasonico = io.feed("Ultrasonico");
AdafruitIO_Feed *CanalDetector_de_metales = io.feed("Detector_de_metales");
AdafruitIO_Feed *CanalTemperatura = io.feed("Temperatura");

uint8_t dato1_anterior = 0;
uint8_t dato2_anterior = 0;
uint16_t temp_anterior = 0;

uint8_t dato1;
uint8_t dato2;
uint16_t temp; 

void setup() {
  Serial.begin(9600);
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  io.run();
  while (Serial.available() > 0) {
    char identifier;
    identifier = Serial.read();
    switch (identifier) {
      case 'a': // Sensor ultrasonico
        dato1 = Serial.read();
        Serial.print(dato1);
        if (dato1 != dato1_anterior) {
          delay(3000); //Espera 3 segundos a que se estabilice la señal
          if(dato1 != dato1_anterior){
            CanalUltrasonico->save(dato1);
            dato1_anterior = dato1;
          }
        }
        break;
      case 'b': // Sensor de metales
        dato2 = Serial.read();
        Serial.print(dato2);
        if (dato2 != dato2_anterior) {
          delay(3000); //Espera 3 segundos a que se estabilice la señal
          if (dato2 != dato2_anterior){
            CanalDetector_de_metales->save(dato2);
            dato2_anterior = dato2;
          }
        }
        break;
      case 'T': // Temperatura
        temp = Serial.read();
        Serial.print(temp);
        if (temp != temp_anterior) {
          delay(3000); //Espera 3 segundos a que se estabilice la señal
          if (temp != temp_anterior){
            CanalTemperatura->save(temp);
            temp_anterior = temp;
          }
        }
        break;
      default:
        Serial.println("Unknown identifier");
        break;
    }
  }
  delay(3000);
}