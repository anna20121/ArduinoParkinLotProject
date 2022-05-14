#include <Wire.h>
#include <VL53L0X.h>
#include <ThingSpeak.h>
#include <WiFiNINA.h>

VL53L0X sensor;

int time;
const int PIN_RED = 11;
const int PIN_GREEN = 10;

int red;
int green;

int state;
int dist;

void set_colour(int r, int g)
{
  analogWrite(PIN_RED, r);
  analogWrite(PIN_GREEN, g);
}

void setup()
{
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("our lovely distance test");
  Wire.begin();

  sensor.init();
  sensor.setTimeout(500);

  sensor.startContinuous();
 }

void loop() 
{
  //Serial.print(sensor.readRangeContinuousMillimeters());// print distance
  int a = 150;
  dist = sensor.readRangeContinuousMillimeters();
  Serial.println(dist);
  if(dist<a)
  {
    state = 1;
    set_colour(255, 0);
  }
  else if(dist==a || dist>a)
  {
    state = 0;
    set_colour(0,255);
  }


  if (sensor.timeoutOccurred()) 
  { 
    Serial.print(" TIMEOUT"); 
  }
  
  Serial.println();
  delay(500);
}
