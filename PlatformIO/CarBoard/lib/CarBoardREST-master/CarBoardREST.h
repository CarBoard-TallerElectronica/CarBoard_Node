#include "Arduino.h"
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>

// JSON data buffer
StaticJsonDocument<1200> jsonDocument;
char buffer[1200];

Preferences* _preferences;

//Funciones de manejo JSON Auxiliares
void create_json(char *tag, float value, char *unit) {  
  jsonDocument.clear();
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);  
}


namespace CarBoardREST{

    WebServer* _serverPointer;
    int _bufferSize;

    float *_latitude;
    float *_longitude;

    float *_acceleration;
    float *_speed;
   
    void add_json_object(char *tag, float value, char *unit) {
        JsonObject obj = jsonDocument.createNestedObject();
        obj["type"] = tag;
        obj["value"] = value;
        obj["unit"] = unit; 
    }

    void setBufferSize(int bufferSize){
        _bufferSize = bufferSize;
    }

    void linkServer(WebServer* serverPointer){
        _serverPointer = serverPointer;
    }

    //Métodos GET
    void GETAll(){
      jsonDocument.clear();

      //Posición
      add_json_object("longitud", (*_longitude), "GMS");
      add_json_object("latitud", (*_latitude), "GMS");

      //Aceleración y velocidad
      add_json_object("acceleration", (*_acceleration), "m/s2");
      add_json_object("speed", (*_speed), "Km/h");

      serializeJson(jsonDocument, buffer); 
      (*_serverPointer).send(200, "application/json", buffer);
  }


    //Métodos PUT




 
    
  


    

}