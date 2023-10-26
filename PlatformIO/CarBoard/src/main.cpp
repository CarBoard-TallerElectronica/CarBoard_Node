#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ConnecT.h>
#include <CarBoardREST.h>
#include <QMC5883LCompass.h>
#include <UbxGpsNavPvt8.h>


// ### PINES ### //
#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

// ### Constantes ### //
#define SCREEN_ADDRESS 0x3D
#define COMPUTER_BAUDRATE 115200
#define GPS_BAUDRATE 115200

// ### Variables ### //
float latitude = 0;
float longitude = 0;

float acce_x = 0;
float acce_y = 0;
float acce_z = 0;
float acceleration = 0;

float azimuth = 0;

float speed = 0;
   

// ### Objetos ### //
ConnecT connecT; // Instancia de la clase ConnecT: Permite la conexión a internet y la comunicación con el servidor web
Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
QMC5883LCompass compass;
UbxGpsNavPvt8<HardwareSerial> gps(Serial1);

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

  //# Compass config
  compass.init();

  // # GPS Config
  gps.begin(GPS_BAUDRATE);

  // ## Inicialización del WiFi
  connecT.setDualMode();
  connecT.setWiFi_AP("CarBoard", "carboard");
  connecT.setWebServer(80); // Creación del servidor web en el puerto 80

  /* Se vinculan los apuntadores de los arrays de medición al protocolo REST */
  CarBoardREST::linkServer(connecT.getServerPointer());


}

void loop() {


  //Lectura del compás
  compass.read();
  azimuth = compass.getAzimuth();

}
