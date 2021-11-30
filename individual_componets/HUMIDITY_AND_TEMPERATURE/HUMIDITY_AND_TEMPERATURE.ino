/*
 * NDEJJE UNIVERSITY WEATHER STATION PROJECT 
 * WATER RESEARCH LABARATORY 
This is to send the temperature,humidity to the weather station from a DHT11 sensor 

*/
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#include <Wire.h>
#include "DHT.h"
#define DHTPIN 14    
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11


// Replace with your network credentials
const char* ssid     = "Raffsoft air";
const char* password = "raffsoft123$";
const char* serverName = "http://192.168.0.103/waterlab/send-weather-data.php";


DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
   WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


 if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
//    // Prepare your HTTP POST request data
    String httpRequestData = "&temperature=" + String(t)
                           + "&humidity=" + String(h) +"";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
   //  then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
   // String httpRequestData = "value1="+ (String)t + "&value2= "+ (String)h + "&value3= "+(String)hic;

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
//   http.addHeader("Content-Type", "application/json");
//   int httpResponseCode = http.POST("{\"value1\":\"t\",\"value2\":\"h\",\"value3\":\"hic\"}");
//   
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(30000); 
  
}
