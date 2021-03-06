// Sample RFM69 sender/node sketch, with ACK and optional encryption
// Sends periodic messages of increasing length to gateway (id=1)
// It also looks for an onboard FLASH chip, if present
// Library and code by Felix Rusu - felix@lowpowerlab.com
// Get the RFM69 and SPIFlash library at: https://github.com/LowPowerLab/

#include <RFM69.h>    //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>
#include "LowPower.h"

#define NODEID        4    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
#define GATEWAYID     1
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY   RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define ACK_TIME      30 // max # of ms to wait for an ack
#define LED           9 // Moteinos have LEDs on D9
#define SERIAL_BAUD   115200


// Use pin 2 as wake up pin
const int wakeUpPin = 3;
int pin = 0;
int door=0;

char paquete;
byte sendSize=1;
RFM69 radio;

void wakeUp()
{
  // Just a handler for the pin interrupt.
//  Serial.println("INT");
  //Serial.flush();
  
  //digitalWrite(LED, pin);
  
  //if(pin) pin=0;
  //else pin = 1;
  
  //door = digitalRead(7);
  if(digitalRead(3)){
    paquete= 'c';
    //digitalWrite(LED, HIGH);
  }
  else{
    paquete = 'o';
    //digitalWrite(LED, LOW);
  }
  
  radio.send(GATEWAYID, &paquete, sizeof(paquete), false);
  radio.sleep();
  //radio.sendWithRetry(GATEWAYID, &paquete, sizeof(paquete), 3, 30);
  
  /*if (radio.sendWithRetry(GATEWAYID, &paquete, sizeof(paquete)))
    Serial.print(" ok!");
  else Serial.print(" nothing...");
  
  Serial.print("Door: ");
  Serial.println(paquete);*/

}

void setup(){
  
  //pinMode(LED, OUTPUT);
  pinMode(wakeUpPin, INPUT); 
  
  //Serial.begin(SERIAL_BAUD);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.setHighPower();
  radio.encrypt(ENCRYPTKEY);
  radio.sleep();
    
  //Serial.println("Starting...");
  //Serial.flush();
}

void loop() {
  
  // Allow wake up pin to trigger interrupt on change.
    attachInterrupt(1, wakeUp, CHANGE);
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is changing.
    //Serial.flush();
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(1);
}
