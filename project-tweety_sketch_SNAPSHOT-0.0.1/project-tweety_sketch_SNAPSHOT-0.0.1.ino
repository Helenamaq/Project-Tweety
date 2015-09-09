#include <SPI.h> // needed in Arduino 0019 or later
#include <Ethernet.h>
#include <Twitter.h>
#include "DHT.h"
//DHT22 humidity and temperature sensor liberaries and initialisation definations. 
#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define fan 13

DHT dht(DHTPIN, DHTTYPE);

//trigger event static variables
int maxHum = 75;
int maxTemp = 45;
int trig_h = 10;
int trig_t = 2;
int msg_count = 100;
//static utility variables
long picker;
float prev_h=0.0,prev_t=0.0;


// The includion of EthernetDNS is not needed in Arduino IDE 1.0 or later.
// Please uncomment below in Arduino IDE 0022 or earlier.
//#include <EthernetDNS.h>


// Ethernet Shield Settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// If you don't specify the IP address, DHCP is used(only in Arduino 1.0 or later).
byte ip[] = { 10, 0, 0, 15 };

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
Twitter twitter("3491000954-JoQ3TcuAGX3C6vFHQoAiWZyO0QUECz2EbO2CWrd");

// Message to post
char msg[] = "Hello, World! I'm Speakuino!";


void setup()
{ //physical pinMode configuration for directly stacking the sensor on the board.
  pinMode(9,OUTPUT);
  pinMode(8,INPUT_PULLUP);
  pinMode(7,OUTPUT);
  digitalWrite(9,HIGH);
  digitalWrite(7,LOW);
  pinMode(fan, OUTPUT);
  Serial.begin(9600); 
  dht.begin();

  
 // or you can use DHCP for autoomatic IP address configuration.
 // Ethernet.begin(mac);
  Ethernet.begin(mac,ip);
  Serial.println(Ethernet.localIP());

  
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

}

void loop() // functions to execute repeatively here...
{ 
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  //logic for triggering the twitter post if the change in humidity or temperature is considerable here...

  float change_h = h - prev_h;
  float change_t = t - prev_t;
  
  if(h > maxHum || t > maxTemp) {
      digitalWrite(fan, HIGH);
  } else {
     digitalWrite(fan, LOW); 
  }
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.println(" *C ");

    if(change_h >= (trig_h)){
    // statement for considerable increase in humidity.
    picker = random(msg_count);
    
    }  
    else if(change_h < -(trig_h)){
    // statement for considerable decrease in humidity.
    picker = random(msg_count);
    
    }  
    else if(change_t >= (trig_t)){
    // statement for considerable increase in Temperature.
    picker = random(msg_count);
    
    }  
    else if(change_t < -(trig_t)){
    // statement for considerable decrease in Temperature.
    picker = random(msg_count);
    
    }  
    else return;


  // Twitter message posting implementation code here... 
  Serial.println("connecting ...");
  if (twitter.post(msg)) {
    // Specify &Serial to output received response to Serial.
    // If no output is required, you can just omit the argument, e.g.
    // int status = twitter.wait();
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    }
    else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
    
  }
  else {
    Serial.println("connection failed.");
  }

    // Wait an hour between measurements.
    delay(1000*60*60);
}
