// #include <WiFi.h>

// #define CONNECTION_TIMEOUT 10

// const char* ssid = "rpi-hub";
// const char* password = "checkout";

// void setup(){
//     Serial.begin(9600);
//     delay(1000);

//     WiFi.mode(WIFI_STA); //Optional
//     WiFi.disconnect();
//     WiFi.begin(ssid, password);
//     Serial.println("\nConnecting");
//     int timeout_counter = 0;

//     while(WiFi.status() != WL_CONNECTED){
//         Serial.print(".");
//         delay(200);
//         timeout_counter++;
//         if(timeout_counter >= CONNECTION_TIMEOUT*5){
//         ESP.restart();
//         }
//     }

//     Serial.println("\nConnected to the WiFi network");
//     Serial.print("Local ESP32 IP: ");
//     Serial.println(WiFi.localIP());
// }

// void loop(){}