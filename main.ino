#include <Wire.h>
#include <VL53L0X.h>
#include <ThingSpeak.h>
#include <WiFiNINA.h>
#define TIME_MSG_LEN 11 // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER 'T' // Header tag for serial time sync message
#define TIME_REQUEST 7 // ASCII bell character requests a time sync message

//declaring varaibles
VL53L0X sensor;
unsigned long timeIn;
unsigned long timeOut;
long price_calc;

const int PIN_BLUE = 9;
const int PIN_RED = 11;
const int PIN_GREEN = 10;

const char ssid[] = "enter_wifi_ssid_here";
const char pass[] = "enter_password_here";
WiFiClient net; 

const char * writeAPIKey = " "; // ThingSpeak APIkey
unsigned long myChannelNbr = ; //chanel number

int red;
int green;
int blue;

int state;
int dist;
long time_spent;
int count;


void set_colour(int r, int g, int b) //function to set colour of RGB light
{
  analogWrite(PIN_RED, r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE, b);
}

void TestWiFiConnection() // function to check wi-fi connection
{
  char WiFiStatus = WiFi.status();
  if(WiFiStatus == WL_CONNECTION_LOST   WiFiStatus == WL_DISCONNECTED  WiFiStatus == WL_SCAN_COMPLETED) //incase wifi not connected
  {
    WiFi.end();
    Serial.println("wifi not working");
    set_colour(0,0,255); //blue light when wi-fi problemes occures
    delay(5000);
    WiFi.begin(ssid, pass);
    WiFiConnect();
  }
}

void WiFiConnect()//function to connect to wifi
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

void setup()
{
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
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
  int a = 82; // dist varible when lot is empty
  dist = sensor.readRangeContinuousMillimeters(); //getting current distance
  Serial.println(dist);
  
  
  if(dist<a && state == 0)//red when car enters and sensor at state 0
  {
    timeIn = millis(); //getting current time (when car entered)
    state = 1;
    set_colour(255,0,0); //setting red
    time_spent = 0;
  }
  
  if(dist>=a && state == 1)//green when car leaves and sensor at state 1
  {
    timeOut = millis(); //getting current time (when car leaves)
    set_colour(0,255,0); //setting green
    time_spent = (timeOut - timeIn)/60000; //substracting time when car entered from time when car left
    price_calc =  time_spent*0.75 + 2; //calculating price
    count = count + 1;
    state = 0;
  }

  
  
  if (sensor.timeoutOccurred()) 
  { 
    Serial.print(" TIMEOUT"); 
  }

//setting ThingSpeak fields for varibles
  ThingSpeak.setField(1, dist);
  ThingSpeak.setField(2, state);
  ThingSpeak.setField(3, time_spent);
  ThingSpeak.setField(4, price_calc);
  ThingSpeak.setField(5, count);
  

  ThingSpeak.writeFields(myChannelNbr,writeAPIKey); //sending values to ThingSpeak channel
  Serial.println();
  delay(500);
  
  
  
}