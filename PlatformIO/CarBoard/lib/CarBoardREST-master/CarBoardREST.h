#include "Arduino.h"
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>

// JSON data buffer
StaticJsonDocument<1200> jsonDocument;
StaticJsonDocument<600> timeDocument;
char buffer[1200];

Preferences* _preferences;

//Funciones de manejo JSON Auxiliares
void create_json(float node, float latitude, float longitude, uint32_t speed, float acce, int azimuth, uint16_t positionDOP,
                 uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second){  
  jsonDocument.clear();
  timeDocument.clear(); 

  jsonDocument["nodo"] = node;
  jsonDocument["latitud"] = latitude;
  jsonDocument["longitud"] = longitude;
  jsonDocument["velocidad"] = speed;
  jsonDocument["aceleracion"] = acce;
  jsonDocument["azimuth"] = azimuth;
  jsonDocument["positionDOP"] = positionDOP;

  timeDocument["year"] = year;
  timeDocument["month"] = month;
  timeDocument["day"] = day;
  timeDocument["hour"] = hour;
  timeDocument["minute"] = minute;
  timeDocument["second"] = second;

  jsonDocument["time"] = timeDocument;
}


namespace CarBoardREST{

    WebServer* _serverPointer;
    int _bufferSize;

    int32_t *_latitude;
    int32_t *_longitude;
    int *_azimuth;

    float *_acceleration;
    uint32_t *_speed;

    uint16_t *_positionDOP;
    

    uint16_t *_year;
    uint8_t *_month;
    uint8_t *_day;
    uint8_t *_hour;
    uint8_t *_minute;
    uint8_t *_second;
   

    void setBufferSize(int bufferSize){
        _bufferSize = bufferSize;
    }

    void linkServer(WebServer* serverPointer){
        _serverPointer = serverPointer;
    }

    void linkAzimuth(int* azimuth){
        _azimuth = azimuth;
    }

   void linkAcce(float* acce){
        _acceleration = acce;
    }
   void linkSpeed(uint32_t* speed){
        _speed = speed;
    }
   void linkLatitude(int32_t* latitude){
        _latitude = latitude;
    }
   void linkLongitude(int32_t* longitude){
        _longitude = longitude;
    }

    void linkPositionDOP(uint16_t* positionDOP){
          _positionDOP = positionDOP;
     }


     void linkTime(uint16_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second){
          _year = year;
          _month = month;
          _day = day;
          _hour = hour;
          _minute = minute;
          _second = second;
     }

    //Métodos POST
    char* postContent(){
      jsonDocument.clear();
      create_json(1, (*_latitude), (*_longitude), (*_speed), (*_acceleration), (*_azimuth), (*_positionDOP), (*_year), (*_month), (*_day), (*_hour), (*_minute), (*_second));
      serializeJson(jsonDocument, buffer); 
      return buffer;
    }


    //Métodos PUT




 
    
  


    

}