// Sample RFM69 sender/node sketch, with ACK and optional encryption
// Sends periodic messages of increasing length to gateway (id=1)
// It also looks for an onboard FLASH chip, if present
// Library and code by Felix Rusu - felix@lowpowerlab.com
// Get the RFM69 and SPIFlash library at: https://github.com/LowPowerLab/

#include <RFM69.h>    //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>
#include <SPIFlash.h> //get it here: https://www.github.com/lowpowerlab/spiflash
#include "LowPower.h"



#define NODEID        2    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
#define GATEWAYID     1
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY   RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
#define ACK_TIME      30 // max # of ms to wait for an ack
#define LED           9 // Moteinos have LEDs on D9
#define FLASH_SS      8 // and FLASH SS on D8

#define SERIAL_BAUD   115200


// Use pin 2 as wake up pin
const int wakeUpPin = 2;


int TRANSMITPERIOD = 500; //transmit a packet to gateway so often (in ms)

char paquete;
char buff[20];
byte sendSize=1;
boolean requestACK = false;
RFM69 radio;
int door=0;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("0");

  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.setHighPower(); //uncomment only for RFM69HW!
  radio.encrypt(ENCRYPTKEY);
  Serial.println("0.1");
  pinMode(7,INPUT);
  pinMode(wakeUpPin, INPUT);   
  delay(100);

}

void loop() {
  
  Serial.println("1");
  // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(wakeUpPin, wakeUp, CHANGE);
  Serial.println("2");  
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    Serial.println("3");
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(wakeUpPin); 
    Serial.println("4");
    // Do something here
    // Example: Read sensor, data logging, data transmission.

    sendPackage();
    Blink(LED,3);
  
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}

void sendPackage(){
  
  door = digitalRead(7);
    
    if(door) paquete= 'o';
    else paquete = 'c';
    
    
    Serial.print("Door: ");
    Serial.println(door);
      
    Serial.print("Sending[");
    Serial.print(sendSize);
    Serial.print("]: ");


    if (radio.sendWithRetry(GATEWAYID, &paquete, sizeof(paquete)))
     Serial.print(" ok!");
    else Serial.print(" nothing...");
    
    Serial.println();
  
}
