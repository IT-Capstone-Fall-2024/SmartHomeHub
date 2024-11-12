#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>

// Pins
const int led = 2; // pin for onboard LED
const int flash = 0; // pin for "FLASH" button
#define PIN_WS2812B 16  // The ESP32 pin GPIO16 connected to WS2812B
#define NUM_PIXELS 60   // The number of LEDs (pixels) on WS2812B LED strip

//MQTT Config
const char* mqtt_server = "192.168.90.234";
const char* user = "Cam_Button";
const char* mqttpass = "password";
const char* mainTopic = "camera/1";
const char* client = "Cam_Button";

// WiFi Config
const char* ssid = "Smart-Home-AP";
const char* wifiPassword = "checkout";
WiFiClient espClient;
PubSubClient mqtt(mqtt_server, 1883, 0, espClient);


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
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(flash, INPUT);
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
  }
}

void loop() {
    //Initial Connection to MQTT server
  if (mqtt_server!="") {
    if(!mqtt.connected()) {
      Serial.println("MQTT Reconnecting");
      reconnect();
    }
    if (digitalRead(flash)) {
      digitalWrite(led, LOW);
    } else {
     digitalWrite(led, HIGH);
     mqtt.publish(mainTopic, "camera");
     delay(1000);
   }
  }
}
