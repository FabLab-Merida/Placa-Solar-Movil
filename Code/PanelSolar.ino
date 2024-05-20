#include <Servo.h> // Incluye la librería para el control de servos

// Definición de pines para los LDR (Fotodiodos)
#define PIN_LDR_SUPERIOR_DERECHA A0
#define PIN_LDR_INFERIOR_DERECHA A1
#define PIN_LDR_SUPERIOR_IZQUIERDA A2
#define PIN_LDR_INFERIOR_IZQUIERDA A3

// Definición de pines para los servomotores
#define PIN_SERVO_V 9 // Pin para el servo vertical
#define PIN_SERVO_H 10 // Pin para el servo horizontal

// Constantes de configuración
#define ANGULO_DE_GIRO 5 // Ángulo de giro para el movimiento de los servos
#define TOL 20 // Tolerancia para el movimiento del seguidor solar
#define POS_SUP_MAX 180 // Posición máxima para el servo vertical
#define POS_INF_MAX 0 // Posición mínima para el servo vertical

// Declaración de objetos servo
Servo servoHorizontal; // Servo para el movimiento horizontal
Servo servoVertical; // Servo para el movimiento vertical

// Variables para almacenar los valores de los LDR
int VLdrSD, VLdrSI, VLdrID, VLdrII;

// Configuración inicial
void setup() {
  Serial.begin(9600); // Inicia la comunicación serial a 9600 baudios

  // Configuración de los pines de los LDR como entradas
  pinMode(PIN_LDR_SUPERIOR_DERECHA, INPUT);
  pinMode(PIN_LDR_INFERIOR_DERECHA, INPUT);
  pinMode(PIN_LDR_SUPERIOR_IZQUIERDA, INPUT);
  pinMode(PIN_LDR_INFERIOR_IZQUIERDA, INPUT);

  // Inicialización de los servomotores
  servoHorizontal.attach(PIN_SERVO_H); // Conecta el servo horizontal al pin 10
  servoVertical.attach(PIN_SERVO_V); // Conecta el servo vertical al pin 9
  servoHorizontal.write(90); // Establece la posición inicial del servo horizontal en 90 grados
  servoVertical.write(90); // Establece la posición inicial del servo vertical en 90 grados
}

// Bucle principal
void loop() {
  // Medición del valor analógico de cada LDR
  VLdrSD = analogRead(PIN_LDR_SUPERIOR_DERECHA);
  VLdrID = analogRead(PIN_LDR_INFERIOR_DERECHA);
  VLdrSI = analogRead(PIN_LDR_SUPERIOR_IZQUIERDA);
  VLdrII = analogRead(PIN_LDR_INFERIOR_IZQUIERDA);
  
  // Cálculo de la media de los valores de los LDR
  int mSuperior = (VLdrSI + VLdrSD) / 2;
  int mInferior = (VLdrII + VLdrID) / 2;
  int mDerecha = (VLdrSD + VLdrID) / 2;
  int mIzquierda = (VLdrII + VLdrSI) / 2;

  // Llamada a la función para mover los servomotores
  realizarMovimientoMotores(mSuperior, mInferior, mDerecha, mIzquierda);

  delay(1000); // Retardo de 1 segundo
}

// Función para mover los servomotores según la posición de los LDR
void realizarMovimientoMotores(int mSuperior, int mInferior, int mDerecha, int mIzquierda) {
  float mTotal = (mSuperior + mInferior + mIzquierda + mDerecha) / 4;

  // Movimiento vertical del seguidor solar
  if ((mSuperior - mInferior) > TOL && servoVertical.read() < POS_SUP_MAX) {
    servoVertical.write(servoVertical.read() + ANGULO_DE_GIRO); // Movimiento hacia arriba
  } else if ((mInferior - mSuperior) > TOL && servoVertical.read() > POS_INF_MAX) {
    servoVertical.write(servoVertical.read() - ANGULO_DE_GIRO); // Movimiento hacia abajo
  }

  // Movimiento horizontal del seguidor solar
  if ((mIzquierda - mDerecha) > TOL && servoHorizontal.read() < POS_SUP_MAX) {
    servoHorizontal.write(servoHorizontal.read() + ANGULO_DE_GIRO); // Movimiento hacia la izquierda
  } else if ((mDerecha - mIzquierda) > TOL && servoHorizontal.read() > POS_INF_MAX) {
    servoHorizontal.write(servoHorizontal.read() - ANGULO_DE_GIRO); // Movimiento hacia la derecha
  }
}
