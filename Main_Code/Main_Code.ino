/*
   The DHT 11 will be used to measure both Humidity and temperature
*/
#include <dht.h>
#define DHT11_PIN 7
dht DHT;

/*
 * PRESUURE AND ALTITUDE the bmp280 uses I2C connection to measure altitude and pressure 
 */

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C


/*we use the red and yellow wires to get sighnals from the rain gauge . we count the number of time the bucket tips to measure the amount of water colllected in a given period of time.
   Per manufatures spec on bucket being tested:
   "Average Switch closure time is 135 ms"
   "Bounce Settling Time: 0.75 ms"
*/
//
#define RAIN_PIN 3         // interrupt pin
#define CALC_INTERVAL 1000  // increment of measurements
#define DEBOUNCE_TIME 15    // time * 1000 in microseconds required to get through bounce noise
unsigned long nextCalc;
unsigned long timer;
volatile unsigned int rainTrigger = 0;
volatile unsigned long last_micros_rg;

void countingRain() {
  // ATTEMPTED: Check to see if time since last interrupt call is greater than
  // debounce time. If so, then the last interrupt call is through the
  // noisy period of the reed switch bouncing, so we can increment by one.
  if ((long)(micros() - last_micros_rg) >= DEBOUNCE_TIME * 1000) {
    rainTrigger += 1;
    last_micros_rg = micros();
  }
}


void setup() {
  Serial.begin(9600); // start serial communicatio

//rain gauge setup
  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), countingRain, RISING);
  pinMode(RAIN_PIN, INPUT);
  nextCalc = millis() + CALC_INTERVAL;

  // pressure sensor (BMP280) setup
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check the wiring!"));
    while (1);
  }
    /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}



void loop() {
  humidity();
  rainGuage();
}





void humidity() {
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(1000);

}

void rainGuage() {
  timer = millis();
  if (timer > nextCalc) {
    nextCalc = timer + CALC_INTERVAL;
    Serial.print("Total Tips: ");
    Serial.println((float) rainTrigger);
  }
}

void pressureSensor(){
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1018);
  Serial.println("pressure = ");
  Serial.print(pressure);
  Serial.println("Altitude = ");
  Serial.print(altitude);
  }
