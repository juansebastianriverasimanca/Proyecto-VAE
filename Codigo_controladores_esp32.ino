// Constantes
#define ANALOG_IN_PIN 27    // Pin analógico de entrada
#define PWM_OUT_PIN 25      // Pin de salida PWM
#define REFERENCE_VOLTAGE 3.3   // Voltaje de referencia

// Variables globales
float u_k = 0; // Valor inicial de u(k)
float e_k = 0; // Valor inicial de e(k)
float e_k_1 = 0; // Valor inicial de e(k-1)

void setup() {
  Serial.begin(115200);
  
  // Configurar pin de salida PWM
  pinMode(PWM_OUT_PIN, OUTPUT);
}

void loop() {
  float error = readError();  // Leer valor de error
  float u = 0.9 * u_k + 3.333 * error - 0.333 * e_k_1;  // Calcular señal de control
  
  // Actualizar valores
  e_k_1 = error;
  u_k = u;
  
  setControlSignal(u,PWM_OUT_PIN);  // Salida de control
}

// Función para leer la señal de error
float readError() {
int analogValue = analogRead(ANALOG_IN_PIN);  // Leer voltaje analógico de entrada

float voltage = (analogValue / 4095.0) * REFERENCE_VOLTAGE; // Convertir el valor analógico a voltaje

float error = REFERENCE_VOLTAGE - voltage;  // Calcular el error como la diferencia entre el voltaje objetivo y el voltaje de entrada
  
return error;
}

// Función para establecer la señal de control
void setControlSignal(float u, int PWM_PIN) {
//Parámetros PWM
const int freq = 1500;      // set the frequency
const int ledChannel = 0;   // set the PWM channel
const int resolution = 10;  //set the PWM resolution

u = constrain(u, 0.0, REFERENCE_VOLTAGE); // Limitar el valor de la señal de control dentro del rango válido

int pwmValue = map(u, 0.0, REFERENCE_VOLTAGE, 0, 1023); // Convertir el valor de la señal de control a un valor de ciclo de trabajo PWM

ledcSetup(ledChannel, freq, resolution);  // define the PWM Setup
ledcAttachPin(PWM_PIN, ledChannel);
ledcWrite(ledChannel, pwmValue);
  
// Imprimir el valor de la señal de control en el monitor serial
Serial.println(u);
}
