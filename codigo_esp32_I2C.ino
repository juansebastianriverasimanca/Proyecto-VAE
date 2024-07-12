#include <Wire.h>

#define SLAVE_ADDRESS 0x68  // Dirección del dispositivo OpenMV H7 (esclavo)

void setup() {
  Wire.begin(SLAVE_ADDRESS);  // Inicia la comunicación I2C como esclavo
  Wire.onReceive(receiveEvent);  // Configura el evento de recepción
  Wire.onRequest(requestEvent);  // Configura el evento de solicitud
  Serial.begin(9600);  // Inicia la comunicación serial
}

void loop() {
  delay(100);
}

void receiveEvent(int bytes) {
  while(Wire.available()) {
    int received_data = Wire.read();  // Lee el dato recibido desde OpenMV H7
    Serial.print("Valor recibido desde OpenMV: ");
    Serial.println(received_data);  // Muestra el valor recibido desde OpenMV

    // Envía confirmación de recepción a OpenMV H7
    Wire.write(1);  // Envía el valor 1 como confirmación de recepción
  }
}

void requestEvent() {
  // Envía datos a OpenMV H7 cuando se solicita
  Wire.write(2);  // Envía el valor 2 a OpenMV H7
}



