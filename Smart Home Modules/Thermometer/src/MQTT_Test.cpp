// #include <WiFi.h>
// #include <Arduino.h>
// #include <PubSubClient.h>

// //MQTT Config
// const char* mqtt_server = "192.168.50.234";
// const char* user = "tester";
// const char* mqttpass = "password";
// const char* mainTopic = "test";
// const char* confirmTopic = "test/confirm";
// const char* client = "test";

// // WiFi Config
// const char* ssid = "Smart-Home-AP";
// const char* wifiPassword = "checkout";
// WiFiClient espClient;
// PubSubClient mqtt(mqtt_server, 1883, 0, espClient);

// // Test pins
// const int led = 2; // pin for onboard LED
// const int flash = 0; // pin for "FLASH" button


// // Reconnects the WiFi and MQTT if disconnected
// void reconnect() {
//   // Loop until we're reconnected
//   while (!mqtt.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     // Attempt to connect
//     if (mqtt.connect(client, user, mqttpass)) {
//       Serial.println(F("connected"));
//       // ... and resubscribe
//       // mqtt.subscribe(topicStatus);
//     } else {
//       Serial.print(F("failed, rc="));
//       Serial.print(mqtt.state());
//       Serial.println(F(" try again in 5 seconds"));
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }

// String get_wifi_status(int status){
//     switch(status){
//         case WL_IDLE_STATUS:
//         return "WL_IDLE_STATUS";
//         case WL_SCAN_COMPLETED:
//         return "WL_SCAN_COMPLETED";
//         case WL_NO_SSID_AVAIL:
//         return "WL_NO_SSID_AVAIL";
//         case WL_CONNECT_FAILED:
//         return "WL_CONNECT_FAILED";
//         case WL_CONNECTION_LOST:
//         return "WL_CONNECTION_LOST";
//         case WL_CONNECTED:
//         return "WL_CONNECTED";
//         case WL_DISCONNECTED:
//         return "WL_DISCONNECTED";
//     }
//     return "";
// }


// void setup() {   
//   // initialize inbuilt LED pin as an output.
//   Serial.begin(9600);
//   pinMode(led, OUTPUT);
//   pinMode(flash, INPUT);
//   int status = WL_IDLE_STATUS;
//   Serial.println(get_wifi_status(status));
//   Serial.begin(9600);
//   Serial.println("Booting");
//   Serial.println(F("Connecting to Wifi"));
//   // Connects to WiFi
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, wifiPassword);
//   while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//     Serial.println("Connection Failed! Rebooting...");
//     delay(5000);
//     ESP.restart();
//   }
//   if (mqtt_server!="") {
//     mqtt.setServer(mqtt_server, 1883);
//     mqtt.setBufferSize(512);
//   }

// }


// // loop function runs over and over  again forever
// void loop() {
//   //Initial Connection to MQTT server
//   if (mqtt_server!="") {
//     if(!mqtt.connected()) {
//       Serial.println("MQTT Reconnecting");
//       reconnect();
//     }
//     mqtt.loop(); // Looks at messages constantly
//     mqtt.subscribe(mainTopic); // Subscribes to prementioned topic
//   }
//   if (digitalRead(flash)) {
//     digitalWrite(led, HIGH);
//   } else {
//     digitalWrite(led, LOW);
//     mqtt.publish(mainTopic, "test positive");
//     delay(1000);
//   }
// }