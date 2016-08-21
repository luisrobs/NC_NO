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

//#define DEBUG

RFM69 radio;
char message[8];

void setup() {
  
  //Radio Settings
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.setHighPower();
  radio.encrypt(ENCRYPTKEY);
  radio.promiscuous(true);
  
  //Output Settings
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5,OUTPUT);
  
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("\nListening...");  
  #endif
}

void loop() {

  if (radio.receiveDone())
  {
    #ifdef DEBUG
    Serial.print("ID:[");Serial.print(radio.SENDERID, DEC);Serial.print("]-MSG:[");
    #endif

    for (byte i = 0; i < radio.DATALEN; i++){
      message[i] = (char)radio.DATA[i];
      
      #ifdef DEBUG
      Serial.print(message[i]);
      #endif
    }
    
    #ifdef DEBUG
    Serial.print("]-[RX_RSSI:");Serial.print(radio.RSSI);Serial.println("]");
    #endif
    
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
