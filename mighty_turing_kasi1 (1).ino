

#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Pantalla LCD I2C
LiquidCrystal_I2C lcd(0x26, 16, 2);

// Servomotor
Servo puerta;

// Pines de los LED
const int patio1 = 4;
const int patio2 = 5;
const int led = 13;

// Sensor ultrasonico
const int trig = 8;
const int echo = 9;

// Sensores
const int sensorTemp = A3;
const int sensorluz = A1;

// Variables
float t = 0;
int valorLuz = 0;

long duracion = 0;
long distancia = 0;

// Umbral de luz
const int umbralLuz = 500;

void setup() {
  puerta.attach(3);
  puerta.write(90);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(patio1, OUTPUT);
  pinMode(patio2, OUTPUT);
  pinMode(led, OUTPUT);

  digitalWrite(trig, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("SISTEMA DE");
  lcd.setCursor(2, 1);
  lcd.print("TEMPERATURA");

  Serial.begin(9600);

  delay(2000);
  lcd.clear();
}

void loop() {
  SistemaDeTemperatura();
  SistemaAutomatico();
  SistemaIluminacion();

  delay(100);
}

// SISTEMA DE TEMPERATURA
void SistemaDeTemperatura() {
  int lectura = analogRead(sensorTemp);

  float voltaje = lectura * (5.0 / 1023.0);
  t = (voltaje - 0.5) * 100.0;

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t, 1);
  lcd.write((uint8_t)223);
  lcd.print("C  ");

  Serial.print("Temperatura: ");
  Serial.println(t, 1);
}

// SISTEMA AUTOMATICO DE PUERTA
void SistemaAutomatico() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duracion = pulseIn(echo, HIGH, 30000);

  if (duracion == 0) {
    distancia = 999;
  } else {
    distancia = duracion / 58;
  }

  if (distancia < 15) {
    puerta.write(180);
  } else {
    puerta.write(90);
  }
}

// SISTEMA DE ILUMINACION
void SistemaIluminacion() {
  valorLuz = analogRead(sensorluz);

  Serial.print("Lectura de luz: ");
  Serial.println(valorLuz);

  // EN OSCURIDAD: ENCENDER LOS LED
  // CON LUZ DEL SOL: APAGAR LOS LED
  if (valorLuz < umbralLuz) {
    digitalWrite(patio1, HIGH);
    digitalWrite(patio2, HIGH);
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(patio1, LOW);
    digitalWrite(patio2, LOW);
    digitalWrite(led, LOW);
  }
}
