#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ConnecT.h>
#include <CarBoardREST.h>
#include <QMC5883LCompass.h>
#include "SparkFun_Ublox_Arduino_Library.h" 
#include <HTTPClient.h>
SFE_UBLOX_GPS gps;


const char* serverName = "http://192.168.1.2:8080/measurement/log/";
unsigned long lastTime_serve = 0;
unsigned long timerDelay_serve = 5000;


// ### PINES ### //
#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

// ### Constantes ### //
#define SCREEN_ADDRESS 0x3C
#define COMPUTER_BAUDRATE 9600
#define GPS_BAUDRATE 9600

// ### Variables ### //
float latitude = 0;
float longitude = 0;

double acce_x = 0;
double acce_y = 0;
double acce_z = 0;
float acceleration = 0;

float azimuth = 0;

float speed = 0;
   

// ### Objetos ### //
ConnecT connecT; // Instancia de la clase ConnecT: Permite la conexión a internet y la comunicación con el servidor web
Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
QMC5883LCompass compass;
Adafruit_Sensor *mpu_temp, *mpu_accel, *mpu_gyro;


// ### Funciones de setUp ### //

// ### Funciones auxiliares ### //
void setup() {

  Serial.begin(COMPUTER_BAUDRATE);

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

  pinMode(LED_BUILTIN, OUTPUT);

  //# Compass config
  compass.init();

  // # GPS Config
  Serial0.begin(GPS_BAUDRATE);
  gps.begin(Serial0);
  gps.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
  gps.saveConfiguration(); //Save the current settings to flash and BBR

  // ## Inicialización del WiFi
  connecT.setDualMode();
  connecT.setWiFi_AP("CarBoard", "carboard");
  connecT.setWebServer(80); // Creación del servidor web en el puerto 80

  /* Se vinculan los apuntadores de los arrays de medición al protocolo REST */
  CarBoardREST::linkServer(connecT.getServerPointer());
  CarBoardREST::linkAzimuth(&azimuth);

  CarBoardREST::linkAcce(&acceleration);
  CarBoardREST::linkSpeed(&speed);
  CarBoardREST::linkLatitude(&latitude);
  CarBoardREST::linkLongitude(&longitude);


  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Acce X: ");
  display.print(acce_x);
  display.display(); // 


  // Se inicializa el giro
  mpu.begin();
  mpu_temp = mpu.getTemperatureSensor();
  mpu_temp->printSensorDetails();

  mpu_accel = mpu.getAccelerometerSensor();
  mpu_accel->printSensorDetails();

  mpu_gyro = mpu.getGyroSensor();
  mpu_gyro->printSensorDetails();

  (connecT.getServerPointer())->begin(); //Init webserver

}

void loop() {
  // Loop Led
  digitalWrite(LED_BUILTIN, HIGH);

  // Lectura del compás
  compass.read();
  azimuth = compass.getAzimuth();
  
  // Lectura del MPU
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  mpu_temp->getEvent(&temp);
  mpu_accel->getEvent(&accel);
  mpu_gyro->getEvent(&gyro);



  //Listen requests
  (connecT.getServerPointer())->handleClient();


  //POST TO SERVER
  if ((millis() - lastTime_serve) > timerDelay_serve) {


    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/json");
    String httpRequestData = CarBoardREST::postContent();           
    int httpResponseCode = http.POST(httpRequestData);
    

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
      
    // Free resources
    http.end();
    lastTime_serve = millis();
    }


  display.display();
  }


