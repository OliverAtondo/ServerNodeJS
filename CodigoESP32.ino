#include <Arduino.h>
#include <WiFi.h>
#include <SocketIoClient.h>
#include <ArduinoJson.h>

String id = "20212987";
const char* ssid     = "TecNM-ITT";
const char* password = "Atondo2021";
const char* server = "172.16.11.37"; 
const uint16_t port = 80;
uint64_t messageTimestamp;
int payload_entero;
SocketIoClient socketIO;
#define ONBOARD_LED  2

#include <DHT.h>
#define DHTPIN 23     // Digital pin connected to the DHT sensor
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

/* SENSOR DHT*/ 
float h=0;
float t=0;
String str;
String str_luz = "OFF";
StaticJsonDocument<256> doc;
bool LIGHT = false;
bool TEMPERATURE = false;
bool HUMIDITY = false;

void setup() {
  pinMode(ONBOARD_LED,OUTPUT); 
  pinMode(15,OUTPUT);
  Serial.begin(115200);
  connectWiFi_STA();
  socketIO.begin(server, port);
  socketIO.on("controlar_ESP",procesar_mensaje_recibido);
  /* SENSOR DHT */
  dht.begin();
}

void loop() {
  socketIO.loop();
  uint64_t now = millis();
  if(now - messageTimestamp > 2000)
  {
      if(TEMPERATURE && !HUMIDITY)
      {
        t = dht.readTemperature();
        if (isnan(t)) {
          Serial.println(F("Failed to read from DHT sensor!")); // F(): Flash Mem instead RAM
          return;
        }
        LIGHT ? str_luz = "ON":str_luz = "OFF";
        str = "{\"id\":\""+id+"\",\"temperatura\":"+String(t)+", \"humedad\":"+"\"OFF\",\"luz\":\""+str_luz+"\"}";
        socketIO.emit("telemetria", str.c_str());
      }  
      else if(!TEMPERATURE && HUMIDITY)
      {
        h = dht.readHumidity();
        if (isnan(h)) {
          Serial.println(F("Failed to read from DHT sensor!")); // F(): Flash Mem instead RAM
          return;
        }
        LIGHT ? str_luz = "ON":str_luz = "OFF";
        str = "{\"id\":\""+id+"\",\"temperatura\":"+"\"OFF\""+", \"humedad\":"+String(h)+",\"luz\":\""+str_luz+"\"}";
        socketIO.emit("telemetria", str.c_str());
      }
      else if(TEMPERATURE && HUMIDITY)
      {
        t = dht.readTemperature();
        h = dht.readHumidity();
        if (isnan(h) || isnan(t)) {
          Serial.println(F("Failed to read from DHT sensor!")); // F(): Flash Mem instead RAM
          return;
        }
        LIGHT ? str_luz = "ON":str_luz = "OFF";
        str = "{\"id\":\""+id+"\",\"temperatura\":"+String(t)+", \"humedad\":"+String(h)+",\"luz\":\""+str_luz+"\"}";
        socketIO.emit("telemetria", str.c_str());
      }
      else if(!TEMPERATURE && !HUMIDITY)
      {
        LIGHT ? str_luz = "ON":str_luz = "OFF";
        str = "{\"id\":\""+id+"\",\"temperatura\":"+"\"OFF\""+", \"humedad\":"+"\"OFF\""+",\"luz\":\""+str_luz+"\"}";
        socketIO.emit("telemetria", str.c_str());
      }
      messageTimestamp = now; 
    }  
}

void procesar_mensaje_recibido(const char * payload, size_t length) {

 //Serial.printf("Mensaje recibido: %s\n", payload);
 
 auto error = deserializeJson(doc, payload);
 if (error) {
   Serial.print(F("deserializeJson() failed with code "));
   Serial.println(error.c_str());
   return;
 }

 String id_r = doc["id"];
 //Serial.printf("id_r: %s\n",id_r);
 
 if(id_r == id)
 {

   String comm = doc["command"];
   if(comm == "LIGHT_ON")
   {
    digitalWrite(ONBOARD_LED,HIGH);
    digitalWrite(15,HIGH);
    LIGHT = true;
   }
   else if(comm == "LIGHT_OFF")
   {
    digitalWrite(ONBOARD_LED,LOW);
    digitalWrite(15,LOW);
    LIGHT = false;
   }
   else if(comm == "TEMPERATURE_ON")
   {
    TEMPERATURE = true;
   }
   else if(comm == "TEMPERATURE_OFF")
   {
    TEMPERATURE = false;
   }
   else if(comm == "HUMIDITY_ON")
   {
    HUMIDITY = true;
   }
   else if(comm == "HUMIDITY_OFF")
   {
    HUMIDITY = false;
   }
 }
 
}

void connectWiFi_STA()
{
   delay(10);
   Serial.println("");
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid);
   while (WiFi.status() != WL_CONNECTED) 
   { 
     delay(100);  
     Serial.print('.'); 
   }
   Serial.println("");
   Serial.print("Iniciado STA:\t");
   Serial.println(ssid);
   Serial.print("IP address:\t");
   Serial.println(WiFi.localIP());
}

//Atondo estuvo aquí :) tqm
