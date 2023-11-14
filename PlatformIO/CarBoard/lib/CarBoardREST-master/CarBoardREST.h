#include "Arduino.h"
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>

// JSON data buffer
StaticJsonDocument<1200> jsonDocument;
char buffer[1200];

Preferences* _preferences;

//Funciones de manejo JSON Auxiliares
void create_json(float node, float latitude, float longitude, float speed, float acce) {  
  jsonDocument.clear();
  jsonDocument["nodo"] = node;
  jsonDocument["latitud"] = latitude;
  jsonDocument["longitud"] = longitude;
  jsonDocument["velocidad"] = speed;
  jsonDocument["aceleracion"] = acce;
 
}


namespace CarBoardREST{

    WebServer* _serverPointer;
    int _bufferSize;

    float *_latitude;
    float *_longitude;
    float *_azimuth;

    float *_acceleration;
    float *_speed;
   

    void setBufferSize(int bufferSize){
        _bufferSize = bufferSize;
    }

    void linkServer(WebServer* serverPointer){
        _serverPointer = serverPointer;
    }

    void linkAzimuth(float* azimuth){
        _azimuth = azimuth;
    }

   void linkAcce(float* acce){
        _acceleration = acce;
    }
   void linkSpeed(float* speed){
        _speed = speed;
    }
   void linkLatitude(float* latitude){
        _latitude = latitude;
    }
   void linkLongitude(float* longitude){
        _longitude = longitude;
    }

    //Métodos POST
    char* postContent(){
      jsonDocument.clear();
      create_json(1, (*_latitude), (*_longitude), (*_speed), (*_acceleration));
      serializeJson(jsonDocument, buffer); 
      return buffer;
    }


    //Métodos PUT




 
    
  


    

}