// #include <Arduino.h>

// //source
// #define ADC_VREF_mV    3300.0 // in millivolt
// #define ADC_RESOLUTION 4096.0
// #define PIN_LM35       12 // ESP32 pin GIOP34 (ADC1) connected to LM35

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   // read the ADC value from the temperature sensor
//   int adcVal = analogRead(PIN_LM35);
//   // convert the ADC value to voltage in millivolt
//   float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
//   // convert the voltage to the temperature in °C
//   float tempC = milliVolt / 10;
//   // convert the °C to °F
//   float tempF = tempC * 9 / 5 + 32;

//   // print the temperature in the Serial Monitor:
//   Serial.print("Temperature: ");
//   Serial.print(tempC);   // print the temperature in °C
//   Serial.print("°C");
//   Serial.print("  ~  "); // separator between °C and °F
//   Serial.print(tempF);   // print the temperature in °F
//   Serial.println("°F");

//   delay(500);
// }