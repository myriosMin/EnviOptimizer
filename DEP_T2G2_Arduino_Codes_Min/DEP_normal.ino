//***temp, hum, air***

//air quality sensor
#include "Air_Quality_Sensor.h"
AirQualitySensor sensor2(A0);

//temp-hum sensor
#include "Seeed_SHT35.h"
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SDAPIN  20
  #define SCLPIN  21
  #define RSTPIN  7
  #define SERIAL SerialUSB
#else
  #define SDAPIN  A4
  #define SCLPIN  A5
  #define RSTPIN  2
  #define SERIAL Serial
#endif
SHT35 sensor(SCLPIN);

//wifi & mqtt
#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_NICLA_VISION) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA)
  #include <WiFi.h>
#elif defined(ARDUINO_PORTENTA_C33)
  #include <WiFiC3.h>
#elif defined(ARDUINO_UNOR4_WIFI)
  #include <WiFiS3.h>
#endif

//wifi credentials
#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;    
char pass[] = SECRET_PASS;    

//broker details & mqtt topic
WiFiClient wifiClient; //create wifi client
MqttClient mqttClient(wifiClient); //connect wifi client to mqtt client
const char broker[] = "192.168.0.101"; //broker ip
int        port     = 1883;
const char topic[]  = "t2g2";  //unique topic
const char clientID[] = "clientid-dsd";  // temporary initialisation; it can be any string

//variables
unsigned int counter = 0;
const long interval = 300000;  // interval between seach MQTT publish (300,000 = 5 min)
unsigned long previousMillis = 0;
float temp = 0.0;
String clientidstr = "";
 
//user-defined functions 
void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

//set up
void setup() {
  Serial.begin(9600); //Initialize serial and wait for port to open
  SERIAL.println("serial start!!");
  //Initialize the sensors 
    //temp-hum
    if(sensor.init())
    {
      SERIAL.println("sensor init failed!!!");
    }
    //air quality
    if (sensor2.init()) {
        Serial.println("Sensor ready.");
    } else {
        Serial.println("Sensor ERROR!");
    }
    delay(1000);

  //print mqtt details
  Serial.println("Simple MQTT publisher");
  Serial.print("Broker: ");
  Serial.println(broker);
  while (!Serial); //wait for serial port to connect. Needed for native USB port only
  
  //attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(2000);
 
  }
  //print mac & ip address
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
  IPAddress ip = WiFi.localIP();

  //successful connection
  Serial.println("You're connected to the network");
  Serial.println();

  //connect wifi client to mqtt client
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  //unique client id derived from topic and ip address, t2g2:192
  clientidstr = ":" + String(ip[3]); 
  clientidstr = topic +  clientidstr;
  Serial.print("Client ID:");Serial.println(clientidstr);
  mqttClient.setId(clientidstr);
  // this username and passwowrd was set in the mosquitto configuration file
  // in the mosquitto zip file downloaded from Brightspace
  mqttClient.setUsernamePassword("egt209", "egt209");  
  while (!mqttClient.connect(broker,port)){
    //failed, retry
    Serial.println("Connecting to MQTT Broker..");
    delay(1000);
  }
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  mqttClient.poll(); //keep the WiFi-MQTT connection alive, avoid being disconnected by the broker
  //BlinkWithoutDelay technique to not disrupt the program flow with delay statements
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // save the last time a message was sent
    
    //read air quality
    int quality = sensor2.slope();
    float air = sensor2.getValue();

    //temp-hum
    u16 value=0;
    u8 data[6]={0};
    float temp,hum;
    if(NO_ERROR!=sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH,&temp,&hum))
    {
      //temp sensor failed
      SERIAL.println("read temp failed!!");
      SERIAL.println("   ");
      SERIAL.println("   ");
      SERIAL.println("   ");
    }
    else
    {
      //read temp & hum

      //print sensor values in terminal to match with transmitted data
      Serial.print("Sending message to topic: ");  
      Serial.println(topic);
      Serial.print("temperature: ");
      Serial.println(temp);
      Serial.print("humidity: ");
      Serial.println(hum);
      Serial.print("air quality: ");
      Serial.println(air);
      //determine and print the air quality
      if (quality == AirQualitySensor::FORCE_SIGNAL) {
          Serial.println("High pollution! Force signal active.");
      } else if (quality == AirQualitySensor::HIGH_POLLUTION) {
          Serial.println("High pollution!");
      } else if (quality == AirQualitySensor::LOW_POLLUTION) {
          Serial.println("Low pollution!");
      } else if (quality == AirQualitySensor::FRESH_AIR) {
          Serial.println("Fresh air.");
      }

      //sending message to mqtt broker
      //message content must be in this format - clientid,sensor_reading;sensor_reading;sensor_reading;
      mqttClient.beginMessage(topic); //create a new message to be published
      
      //send data
      mqttClient.print(clientidstr); 
      mqttClient.print(",");   
      mqttClient.print(temp);
      mqttClient.print(";");
      mqttClient.print(hum);
      mqttClient.print(";");
      mqttClient.print(air);
      mqttClient.print(";");
      mqttClient.print("Min");
      mqttClient.print(";");

      mqttClient.endMessage();  //end the message and send the data
      Serial.println();
    }
    delay(1000);
  }
}

