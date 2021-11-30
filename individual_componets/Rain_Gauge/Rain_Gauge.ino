

// This is for the rain guage code for the weather station at Ndejje University 

// we use the red and yellow wires to get sighnals from the rain gauge . we count the number of time the bucket tips to measure the amount of water colllected in a given period of time. 
#define RAIN_PIN 3         // interrupt pin
#define CALC_INTERVAL 1000  // increment of measurements
#define DEBOUNCE_TIME 15    // time * 1000 in microseconds required to get through bounce noise

// Per manufatures spec on bucket being tested:

// "Average Switch closure time is 135 ms"
// "Bounce Settling Time: 0.75 ms" 

unsigned long nextCalc;
unsigned long timer;

volatile unsigned int rainTrigger = 0;
volatile unsigned long last_micros_rg;

void setup() {
  Serial.begin(9600); 
  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), countingRain, RISING); 
  
  pinMode(RAIN_PIN, INPUT);
  nextCalc = millis() + CALC_INTERVAL;
}

void loop() {
  timer = millis();
  if(timer > nextCalc) {
    nextCalc = timer + CALC_INTERVAL;
    Serial.print("Total Tips: ");
    Serial.println((float) rainTrigger);     
  }
}

void countingRain() {
  // ATTEMPTED: Check to see if time since last interrupt call is greater than 
  // debounce time. If so, then the last interrupt call is through the 
  // noisy period of the reed switch bouncing, so we can increment by one.   
  if((long)(micros() - last_micros_rg) >= DEBOUNCE_TIME * 1000) { 
   rainTrigger += 1;
   last_micros_rg = micros();
  }  
}
