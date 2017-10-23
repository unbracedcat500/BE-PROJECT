//Include necessary Libraries
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

//Initialise Variables
int SensorPin = A0;
int Hum = 0;
int msg[1];
RF24 radio(7,8);
const uint64_t pipe = 0xE8E8F0F0E1LL;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
radio.begin();
pinMode(A0,INPUT);
radio.openWritingPipe(pipe);
}

void loop(void) {
  // put your main code here, to run repeatedly:
  Hum = analogRead(A0); //Read analog values from the sensor
  Hum = map(Hum,0,1024,0,100); //map the analog values in the range 0-100
  Hum = 100-Hum; // invert the values since the sensor gives higher reading at dry state & vice versa
  float HumPer = (float)((Hum-1)*100)/(99); // convert the reading into % form
  Serial.println(HumPer); //print on serial monitor
  String HumPerS = String(HumPer); // Convert % humidity into string
  int HumSize = HumPerS.length(); // Check the size of string
 //Send string character by character to the receiver
 for (int i = 0; i < HumSize; i++) {
 int charToSend[1];
 charToSend[0] = HumPerS.charAt(i);
 radio.write(charToSend,1);
}
//send termination string
 msg[0] = 2;
 radio.write(msg,1);

}
