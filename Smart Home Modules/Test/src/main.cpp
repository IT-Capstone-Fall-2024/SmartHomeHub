#include <WiFiUdp.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>

const int led = 2;
const int flash = 0;
const char* ssid = "rpi-hub";
const char* password = "checkout";
const char* mqtt_server = "10.42.0.1";
const char* user = "tester";
const char* mqttpass = "password";
const char* topic = "testTopic";
const char* client = "TEST";
WiFiClient espClient;
PubSubClient mqtt(mqtt_server, 1883, 0, espClient);

void reconnect() {
  //String mytopic;
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

String messageReceived(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i=0; i < length; i++) {
    message = message + (char)payload[i];
  }

  Serial.println(message);
  if (message = "led") {
    digitalWrite(led, LOW);
    delay(1000);
  }
  return message;
}


void setup() {   
  // initialize inbuilt LED pin as an output.
  pinMode(led, OUTPUT);
  pinMode(flash, INPUT);
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed");
    delay(5000);
    ESP.restart();
  }
  if (mqtt_server!="") {
    mqtt.setServer(mqtt_server, 1883);
    mqtt.setBufferSize(512);
    mqtt.setCallback(messageReceived);
  }

}


// loop function runs over and over  again forever
void loop() {
  if (mqtt_server!="") {
    if(!mqtt.connected()) {
      Serial.println("MQTT Reconnecting");
      reconnect();
    }
    mqtt.loop();
    mqtt.subscribe(topic);
  }
  if (digitalRead(flash)) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
    mqtt.publish(topic, "test positive");
    delay(1000);
  }

}