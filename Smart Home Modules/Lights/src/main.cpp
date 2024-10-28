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

//MQTT Config
const char* mqtt_server = "192.168.50.234";
const char* user = "tester";
const char* mqttpass = "password";
const char* mainTopic = "light/1";
const char* confirmTopic = "light/confirm";
const char* client = "light1";

// WiFi Config
const char* ssid = "Smart-Home-AP";
const char* wifiPassword = "checkout";
WiFiClient espClient;
PubSubClient mqtt(mqtt_server, 1883, 0, espClient);

// Stored Variables // String array to overcome returning char array
int state = 0; // universal state 
String color = ""; // universal color


// Initializes the lights as an object
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

// Splits given message into array (strs[2]), sets the  and returns the color in a String variable
String split(String s) {
  int StringCount = 0;
  String strs[2];
  while (s.length() > 0)
  {
    int index = s.indexOf(' ');
    if (index == -1) // No space found
    {
      strs[StringCount++] = s;
      break;
    }
    else
    {
      strs[StringCount++] = s.substring(0, index);
      s = s.substring(index+1);
    }
  }
  if (strs[0] == String("on")) {
    state = 1;
  }
    if (strs[0] == String("off")) {
    state = 0;
  }
  return strs[1];
}

// Method for translating string to color variables
uint32_t getColor(String color) {
  if (color == "white") {
    return 16777215;
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

// Lights up or disables the LED strip based on input color and state
void Lights(int state, String color) {
  if (state == 0) {
    ws2812b.clear();
    ws2812b.show();
    color = "";
  }
  if (state == 1) {
    ws2812b.clear();
    ws2812b.fill(getColor(color), 0, 0);
    ws2812b.show();
  }
}

// Callback function for MQTT; This runs when any message is pushed to subscribed topic
String messageReceived(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i=0; i < length; i++) {
    message = message + (char)payload[i];
  }
  // Setup for if message comes, write to output pin
  Serial.println(message);
  color = split(message);
  String stateHelp = "";
  String confirm = "";
  if (state == 1) {
    Lights(1, color);
    stateHelp = "On";
    confirm = "Light 1 is " + stateHelp + " and " + color;
  }
  if (state == 0) {
    Lights(0, "");
    stateHelp = "Off";
    confirm = "Light 1 is " + stateHelp;
  }

  mqtt.publish(confirmTopic, confirm.c_str());
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
    mqtt.subscribe(mainTopic); // Subscribes to prementioned topic
  }
  if (digitalRead(flash)) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
    mqtt.publish(mainTopic, "test positive");
    delay(1000);
  }
}
