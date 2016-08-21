// Sample RFM69 receiver/gateway sketch, with ACK and optional encryption
// Passes through any wireless received messages to the serial port & responds to ACKs
// It also looks for an onboard FLASH chip, if present
// Library and code by Felix Rusu - felix@lowpowerlab.com
// Get the RFM69 and SPIFlash library at: https://github.com/LowPowerLab/

#include <RFM69.h>    //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>

#define NODEID        1    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
#define FREQUENCY     RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define ACK_TIME      30 // max # of ms to wait for an ack
#define SERIAL_BAUD   115200
#define LED           9 // Moteinos have LEDs on D9

RFM69 radio;
char message[8];

void setup() {
  //Serial.begin(SERIAL_BAUD);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.setHighPower();
  radio.encrypt(ENCRYPTKEY);
  radio.promiscuous(true);
  /*char buff[50];
  sprintf(buff, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);*/
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5,OUTPUT);
}

byte ackCount=0;
uint32_t packetCount = 0;

void loop() {

  if (radio.receiveDone())
  {
    /*Serial.print("#[");
    Serial.print(++packetCount);
    Serial.print(']');
    Serial.print('[');Serial.print(radio.SENDERID, DEC);Serial.print("] ");*/

    for (byte i = 0; i < radio.DATALEN; i++){
      message[i] = (char)radio.DATA[i];
      //Serial.print(message[i]);
    }
    //Serial.print("   [RX_RSSI:");Serial.print(radio.RSSI);Serial.print("]");
    
    
    if (radio.ACKRequested())
    {
      byte theNodeID = radio.SENDERID;
      radio.sendACK();
      //Serial.print(" - ACK sent.");                
    }
    //Serial.println();
    
    /*Serial.print("Message[0]=");
    Serial.println(message[0]);*/
    
    if(radio.SENDERID == 2){
      if(message[0] == 'c') digitalWrite(3, HIGH);
      else if(message[0] == 'o') digitalWrite(3, LOW);
    }
    
    if(radio.SENDERID == 4){
      if(message[0] == 'c') digitalWrite(4, HIGH);
      else if(message[0] == 'o') digitalWrite(4, LOW);
    }
    
    if(radio.SENDERID == 6){
    if(message[0] == 'c') digitalWrite(5, HIGH);
    else if(message[0] == 'o') digitalWrite(5, LOW);
    }
  }
}
