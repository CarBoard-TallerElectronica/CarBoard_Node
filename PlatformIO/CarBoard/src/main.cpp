#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ConnecT.h>
#include <CarBoardREST.h>
#include <QMC5883LCompass.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> 
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
SFE_UBLOX_GNSS gps;


const char* rootCACertificate = \ 
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"-----END CERTIFICATE-----\n";



const char* serverName = "https://carboard.lat/measurement/log/";
unsigned long lastTime_serve = 0;
unsigned long lastTime_mpu = 0;
unsigned long timerDelay_serve = 4000;
unsigned long timerDelay_mpu = 100;


// ### PINES ### //
#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9

// ### Constantes ### //
#define SCREEN_ADDRESS 0x3C
#define COMPUTER_BAUDRATE 9600
#define GPS_BAUDRATE 9600

// ### Variables ### //
int32_t latitude = 0;
int32_t longitude = 0;

double acce_x = 0;
double acce_y = 0;
double acce_z = 0;
float acceleration = 0;

int azimuth = 0;

uint32_t speed = 0;

uint16_t position_dop = 0;

uint16_t year = 0;
uint8_t month = 0;
uint8_t day = 0;
uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;
   
sensors_event_t accel;
sensors_event_t gyro;
sensors_event_t temp;

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
  
  while (!mpu.begin()) {
    Serial.println("No se encontró el MPU!");
  }

  //# Screen config
  while(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("No se encontró la pantalla!"));
  }

  pinMode(LED_BUILTIN, OUTPUT);



  // # GPS Config
  Serial0.begin(GPS_BAUDRATE);
  gps.begin(Serial0);
  gps.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
  gps.saveConfiguration(); //Save the current settings to flash and BBR

  // ## Inicialización del WiFi
  connecT.setDualMode();
  connecT.setWiFi_AP("CarBoard", "carboard");
  connecT.setWiFi_STA("COMPETENCE", "Mafu2408");
  connecT.setWebServer(80); // Creación del servidor web en el puerto 80

  /* Se vinculan los apuntadores de los arrays de medición al protocolo REST */
  CarBoardREST::linkServer(connecT.getServerPointer());
  CarBoardREST::linkAzimuth(&azimuth);

  CarBoardREST::linkAcce(&acceleration);
  CarBoardREST::linkSpeed(&speed);
  CarBoardREST::linkLatitude(&latitude);
  CarBoardREST::linkLongitude(&longitude);

  CarBoardREST::linkPositionDOP(&position_dop);
  CarBoardREST::linkTime(&year, &month, &day, &hour, &minute, &second);

  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.print("Lat: ");

  display.setCursor(0,10);
  display.print("Lon: ");
  display.display(); // 


  // Se inicializa el giro
  mpu.begin();
  mpu_temp = mpu.getTemperatureSensor();
  mpu_temp->printSensorDetails();

  mpu_accel = mpu.getAccelerometerSensor();
  mpu_accel->printSensorDetails();

  mpu_gyro = mpu.getGyroSensor();
  mpu_gyro->printSensorDetails();


  //# Compass config
  compass.init();

  //WebServer Paths
  connecT.addPOSTtoWeb("/wifi-info", CarBoardREST::POSTWifi);


  (connecT.getServerPointer())->begin(); //Init webserver

}

void loop() {


  // Loop Led
  digitalWrite(LED_BUILTIN, HIGH);

  // Lectura del compás
  compass.read();
  azimuth = compass.getAzimuth();
  

  if ((millis() - lastTime_mpu) > timerDelay_mpu) {
    // Lectura del MPU
    mpu_temp->getEvent(&temp);
    mpu_accel->getEvent(&accel);
    mpu_gyro->getEvent(&gyro);

    acce_x = accel.acceleration.x;
    acce_y = accel.acceleration.y;
    acce_z = accel.acceleration.z;
    acceleration = sqrt(pow(acce_x, 2) + pow(acce_y, 2) + pow(acce_z, 2));
    lastTime_mpu= millis();
  }


  //Listen requests
  (connecT.getServerPointer())->handleClient();



  
  //POST TO SERVER
  if ((millis() - lastTime_serve) > timerDelay_serve) {


    // Lectura del GPS
    latitude = gps.getLatitude();
    longitude = gps.getLongitude();
    speed = gps.getGroundSpeed();
    position_dop = gps.getPositionDOP();

    //Tiempo del GPS
    year = gps.getYear();
    month = gps.getMonth();
    day = gps.getDay();
    hour = gps.getHour();
    minute = gps.getMinute();
    second = gps.getSecond();

    //Secure
    WiFiClientSecure *client = new WiFiClientSecure;
      if(client) {
      // set secure client with certificate
      client->setCACert(rootCACertificate);
      //create an HTTPClient instance
      }

    HTTPClient https;

    https.begin(*client, serverName);
    https.addHeader("Content-Type", "application/json");
    String httpRequestData = CarBoardREST::postContent();           
    int httpResponseCode = https.POST(httpRequestData);
    

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
      
    // Free resources

    lastTime_serve = millis();
    }


  //Gráfica de la pantalla

  display.clearDisplay();

  display.setCursor(0,0);
  display.print("Lat: ");
  display.print(latitude);

  display.setCursor(0,10);
  display.print("Lon: ");
  display.print(longitude);

  display.setCursor(0,20);
  display.print("Az: ");
  display.print(azimuth);

  display.setCursor(0,30);
  display.print("Acce: ");
  display.print(acceleration);


  display.setCursor(0,40);
  display.print("Speed: ");
  display.print(speed);


  display.setCursor(0,50);
  display.print("Time: ");
  display.print(month);
  display.display();



  }


