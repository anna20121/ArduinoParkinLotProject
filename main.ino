#include <Wire.h>
#include <VL53L0X.h>
#include <ThingSpeak.h>
#include <WiFiNINA.h>

VL53L0X sensor;

int time;
const int PIN_RED = 11;
const int PIN_GREEN = 10;

const char ssid[] = "";
const char pass[] = "";
WiFiClient net; 

const char * writeAPIKey = "RY6E1GVFZ8ZBPXSC";
unsigned long myChannelNbr = 1656684;

int red;
int green;

int state;
int dist;

void TestWiFiConnection()
{
  char WiFiStatus = WiFi.status();
  if (WiFiStatus == WL_CONNECTION_LOST  WiFiStatus == WL_DISCONNECTED  WiFiStatus == WL_SCAN_COMPLETED)
  {
    WiFi.end();
    Serial.println("wait wifi fucked");
    delay(5000);
    WiFi.begin(ssid, pass);
    WiFiConnect();
  }
}

void WiFiConnect()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    if (WiFi.status() == WL_DISCONNECTED)
    {
      WiFi.end();
    }
    if (WiFi.status() == WL_IDLE_STATUS)
    {
      WiFi.begin(ssid, pass);
    }
  }
}

void set_colour(int r, int g)
{
  analogWrite(PIN_RED, r);
  analogWrite(PIN_GREEN, g);
}

void setup()
{
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  WiFi.begin(ssid, pass);
  WiFiConnect();
  ThingSpeak.begin(net);
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
  TestWiFiConnection();
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
  ThingSpeak.setField(1, dist);
  ThingSpeak.setField(2, state);
  ThingSpeak.writeFields(myChannelNbr,writeAPIKey);
  Serial.println();
  delay(500);
}