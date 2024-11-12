#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>


//MQTT Config
const char* mqtt_server = "192.168.90.234";
const char* user = "light1";
const char* mqttpass = "password";
const char* mainTopic = "light/1";
const char* confirmTopic = "light/confirm";
const char* client = "light1";

// WiFi Config
const char* ssid = "Smart-Home-AP";
const char* wifiPassword = "checkout";
WiFiClient espClient;
PubSubClient mqtt(mqtt_server, 1883, 0, espClient);

// Pins
const int led = 2; // pin for onboard LED

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

// Callback function for MQTT; This runs when any message is pushed to subscribed topic
String messageReceived(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i=0; i < length; i++) {
    message = message + (char)payload[i];
  }

  return message;
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifiPassword);
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

void loop() {
    //Initial Connection to MQTT server
  if (mqtt_server!="") {
    if(!mqtt.connected()) {
      Serial.println("MQTT Reconnecting");
      reconnect();
    }
    mqtt.loop(); // Looks at messages constantly
    mqtt.subscribe(mainTopic); // Subscribes to prementioned topic
  }
}
