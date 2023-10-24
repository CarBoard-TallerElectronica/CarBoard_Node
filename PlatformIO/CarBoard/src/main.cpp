#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// ### PINES ### //
#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

// ### Constantes ### //
#define SCREEN_ADDRESS 0x3D

// ### Variables ### //

// ### Objetos ### //
Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

// ### Funciones de setUp ### //

// ### Funciones auxiliares ### //

void setup() {

  //## Inicialización de los sensores

  //# MPU config
  if (!mpu.begin()) {
    Serial.println("No se encontró el MPU!");
    while (1) {
      delay(10);
    }
  }

  //# Screen config
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("No se encontró la pantalla!"));
    for(;;); // Don't proceed, loop forever
  }

}

void loop() {

}
