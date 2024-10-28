/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-ws2812b-led-strip
 */

#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>

// Pins
const int led = 2; // pin for onboard LED
const int flash = 0; // pin for "FLASH" button
const int testIO = 5;
#define PIN_WS2812B 16  // The ESP32 pin GPIO16 connected to WS2812B
#define NUM_PIXELS 60   // The number of LEDs (pixels) on WS2812B LED strip

// WiFi Config
const char* ssid = "Smart-Home-AP";
const char* wifiPassword = "checkout";
WiFiClient espClient;
PubSubClient mqtt(mqtt_server, 1883, 0, espClient);

//MQTT Config
const char* mqtt_server = "192.168.50.234";
const char* user = "tester";
const char* mqttpass = "password";
const char* topic = "light1";
const char* client = "TEST";

// Stored Variables
const char* status = "";
const char* color = "";


Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);


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


uint32_t getColor(String color) {
  if (color == "white") {
    return 16777216;
  }
  if (color == "red") {
    return 16711680;
  }
  if (color == "green") {
    return 65280;
  }
  if (color == "blue") {
    return 255;
  }
  if (color == "orange") {
    return 16747520;
  }
  if (color == "purple") {
    return 16711935;
  }
  return 0;
}

void Lights(int state, String color) {
  if (state == 0) {
    ws2812b.clear();
    ws2812b.show();
  }
  if (state == 1) {
    ws2812b.clear();
    ws2812b.fill(getColor(color), 0, 0);
    ws2812b.show();
  }
}

//
String messageReceived(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i=0; i < length; i++) {
    message = message + (char)payload[i];
  }
  // Setup for if message comes, write to output pin
  Serial.println(message);
  if (message == "on") {
    Lights(1, "red");
  }
  if (message == "off") {
    Lights(0, "");
  }
  return message;
}

void setup() {
  Serial.begin(9600);
  ws2812b.begin();
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
    mqtt.subscribe(topic); // Subscribes to prementioned topic
  }
  if (digitalRead(flash)) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
    mqtt.publish(topic, "test positive");
    delay(1000);
  }
}
