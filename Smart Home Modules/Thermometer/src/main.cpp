#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include "soc/sens_reg.h"

//MQTT Config
const char* mqtt_server = "192.168.50.233";
const char* user = "temp";
const char* mqttpass = "password";
const char* mainTopic = "temp/1";
const char* client = "temp";

// WiFi Config
const char* ssid = "Smart-Home-AP";
const char* wifiPassword = "checkout";
WiFiClient espClient;
PubSubClient mqtt(mqtt_server, 1883, 0, espClient);

// Test pins
const int led = 2; // pin for onboard LED
const int flash = 0; // pin for "FLASH" button
#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0
#define PIN_LM35       33 // ESP32 pin GIOP34 (ADC1) connected to LM35

// Reconnects the WiFi and MQTT if disconnected
void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt.connect(client, user, mqttpass)) {
      Serial.println(F("connected"));
      // ... and resubscribe
      // mqtt.subscribe(topicStatus);
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(mqtt.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {  
  // initialize inbuilt LED pin as an output.
  Serial.begin(9600);
  Serial.println("Booting");
  Serial.println(F("Connecting to Wifi"));
  // Connects to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifiPassword);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  if (mqtt_server!="") {
    mqtt.setServer(mqtt_server, 1883);
    mqtt.setBufferSize(512);
  }

}

void loop() {
  //Initial Connection to MQTT server
  if (mqtt_server!="") {
    if(!mqtt.connected()) {
      Serial.println("MQTT Reconnecting");
      reconnect();
    }
  }
    // read the ADC value from the temperature sensor
  int adcVal = analogRead(PIN_LM35);
  // convert the ADC value to voltage in millivolt
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  // convert the voltage to the temperature in °C
  float tempC = milliVolt / 10;
  // convert the °C to °F
  float tempF = tempC * 9 / 5 + 32;

  char temp[20];
  dtostrf(tempF,2,2,temp);
  String stringTemp = String(temp) + "°F";
  Serial.println(stringTemp);
  mqtt.publish(mainTopic, stringTemp.c_str());
  delay(5000);
}