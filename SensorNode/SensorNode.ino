/* This code was modified on 24/10/17. 
 * Delay of 1 sec was added between each transmission using the millis() function.
 * Packet lost problem solved 
 * Power problem exists, Receiver hangs sometimes.
 */


//Include necessary Libraries
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <DHT_U.h>
#include <DHT.h>
//Initialise Constants
#define  moistSensor  A0
//#define ONE_WIRE_BUS 3
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


int msg[1];
RF24 radio(7,8);
const uint64_t pipe = 0xE8E8F0F0E1LL;
unsigned long prevMillis = 0;
const long interval = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  pinMode(moistSensor,INPUT);
  radio.openWritingPipe(pipe);
  radio.setPALevel(RF24_PA_MAX);
}

void loop(void) {
  
  // put your main code here, to run repeatedly:
  radio.powerUp();
  unsigned long currentMillis = millis();
  if(currentMillis - prevMillis >= interval)
  {
    prevMillis = currentMillis;
    
    float moistPer = moisture();
    String moistPerS = String(moistPer);                              // Convert % moisture into string
    int moistSize = moistPerS.length();                               // Check the size of moisture string 
    float hum = dht.readHumidity();                                   //Get humidity values
    float temp = dht.readTemperature();                               //Get temperature readings
    String humS = String(hum);                                        //Convert humidity into string
    String tempS = String(temp);                                      //Convert temperature into string
    int humSize = humS.length();                                      //Check the size of humidity string
    int tempSize = tempS.length();                                    //Check the size of temperature string
    
    //Send string character by character to the receiver
    for (int i = 0; i < moistSize; i++) 
    {
      int charToSend[1];
      charToSend[0] = moistPerS.charAt(i);
      radio.write(charToSend,1);
    }
     for (int i = 0; i < humSize; i++) 
    {
      int charToSend[1];
      charToSend[0] = humS.charAt(i);
      radio.write(charToSend,1);
    }
   
    for (int i = 0; i < tempSize; i++) 
    {
      int charToSend[1];
      charToSend[0] = tempS.charAt(i);
      radio.write(charToSend,1);
    }
  
  //send termination string
  
  msg[0] = 2;
  radio.write(msg,1);
    

    }  

    
   
}
float moisture(){
    int moist = 0;
    moist = analogRead(moistSensor);                                         //Read analog values from the sensor
    moist = map(moist,0,1024,0,100);                                  //map the analog values in the range 0-100
    moist = 100-moist;                                                // invert the values since the sensor gives higher reading at dry state & vice versa
    float moistPer = (float)((moist-1)*100)/(99);                     // convert the reading into % form
    Serial.println(moistPer);                                       //print on serial monitor
    return moistPer;
}

//float temperature(){
//   // Send the command to get temperatures
//  sensors.requestTemperatures();  
//  Serial.print("Temperature is: ");
//  Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
//  //Update value every 1 sec.
//  float temp = sensors.getTempCByIndex(0);
//  return temp;
//  
// }



