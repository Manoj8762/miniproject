#define LED_PIN 10
// Pin connected to the LED light
#include "DHT.h"

#define DHTPIN 2 //it reading input data fron temperature sensor
#define DHTTYPE DHT11   // DHT 11 for temperature 
int fan1=5;
int fan2=6;
int fan3=4;
// It operates fan ON and OFF
int ldr =7; // It operatus the light pin
int x;
int led=11;
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  pinMode(LED_PIN, OUTPUT); //out pin
  pinMode(ldr,INPUT);//input pin of LDR
  pinMode(led,OUTPUT);//Output pin of Led
  pinMode(fan1,OUTPUT);//Output pin of Fan1
  pinMode(fan2,OUTPUT);//Output pin of Fan2
  pinMode(fan3,OUTPUT);//Output pin of Fan3
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop() {
  
  // temperature measuring
  
  //delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();//reads the humidity
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
  Serial.println();
  
  //
  x=digitalRead(ldr);//It storethe  it in the variable 'x'
  Serial.println(x);
  if (Serial.available() > 0)//It recieves the data from serial port
  {
    char signal = Serial.read();

    if (signal == '1') //human is found
    {
        if(x==HIGH)//checks the light intensity and turn ON the led
            {
                digitalWrite(led,HIGH);
            }
        if(x==LOW)//checks the light intensity and turn OFF the led
            {
                digitalWrite(led,LOW);
            }
        else
            {
                Serial.print("Now correct light intensity obtained that is :");
                Serial.println(x);
            }
        if(t>30)//checks the temperature and turn ON the fan
        {
           digitalWrite(fan1, HIGH);
           digitalWrite(fan2, HIGH);
           digitalWrite(fan3, HIGH);
        }
        if(t<29)//checks the temperature and turn OFF the fan
        {
            digitalWrite(fan1,LOW);
            digitalWrite(fan2,LOW);
            digitalWrite(fan3,LOW);
            
        }
        
        
      digitalWrite(LED_PIN, HIGH);  // Turn LED ON
   } 
     if (signal == '0')//human is not found
      {
        Serial.println(F("No human detected. Skipping sensor readings."));
        digitalWrite(fan1, LOW);
        digitalWrite(fan2, LOW);
        digitalWrite(fan3, LOW);// Ensure fan is OFF
        digitalWrite(led, LOW);  // Ensure LED is OFF
        digitalWrite(LED_PIN, LOW);   // Turn LED OFF
      }
  }
}
