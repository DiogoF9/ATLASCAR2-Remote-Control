// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13


#include <SPI.h>
#include "mcp_can.h"

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

int outPin = 5;
int outPin2 = 6;
int dirD;
int dirA;
float val1 = 255/2;
float val2 = 255/2;
float main_signal=2.5;
int kp = 0.01;

int readValue1;
int readValue2;
int readValue3;

void setup() {

  pinMode(outPin,OUTPUT);
  pinMode(outPin2,OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(8,OUTPUT);
pinMode(7,OUTPUT);
pinMode(4,OUTPUT);
    
  SERIAL.begin(115200);

  while (CAN_OK != CAN.begin(CAN_500KBPS)) {            // init can bus : baudrate = 500k
      SERIAL.println("CAN BUS Shield init fail");
      SERIAL.println(" Init CAN BUS Shield again");
      delay(100);
  }
  SERIAL.println("CAN BUS Shield init ok!");
}


void loop() {

    //compatibilizacao dos modos de conducao
    readValue1 = analogRead(A0); // modo atual de condução - manual ou automático
    readValue2 = analogRead(A1); // sinal torque sensor
    readValue3 = analogRead(A2); // sinal torque sensor
  
    //if (readValue3 > 650 || readValue3 < 350){ 
    //digitalWrite(8,LOW);
    //delay(5000);}
    //else{digitalWrite(8,HIGH);}

    //comunicacao CAN
    unsigned char len = 0;
    unsigned char buf[8];
    
analogWrite(outPin,102);
            analogWrite(outPin2,51);
            
    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();
        if (canId == 0x231)
        {
          SERIAL.println("-----------------------------");
          SERIAL.print("Get data from ID: 0x");
          SERIAL.println(canId, HEX);

          for (int i = 0; i < len; i++) { // print the data
            SERIAL.print(buf[i], HEX);
            SERIAL.print("\t");
          }


          //desencriptação da mensagem para ter angulos desejados e atual
          if (canId == 0x231  && buf[4] == 0x02 ){
             
  digitalWrite(8,LOW);
  delay(5000);}
  else{digitalWrite(8,HIGH);}
  //digitalWrite(8,HIGH);
  digitalWrite(7,LOW);
            analogWrite(outPin,102);
            analogWrite(outPin2,51);
            if (buf[1] == 0x01)
              dirD = -dirD;
            if (buf[3] == 0x01)
              dirA = -dirA;}
    
          
          SERIAL.print(dirD,DEC);
          SERIAL.print("\t");
        
          //if (canId == 0x236){
            //dirA = (buf[0]*256+buf[1]-4096)/2;}

          SERIAL.print(dirD-dirA,DEC); // VALORES NEGATIVOS PARA A DIREITA E POSITIVOS PARA A ESQUERDA
          SERIAL.print("\t");

        
        
         main_signal = main_signal+0.05;
        //val1 = val1 + 1;
        //val2 = val2 - 1;
        val1=main_signal*255/5;
        val2=255/2-(val1-255/2);
       // analogWrite(outPin,val1);
        //analogWrite(outPin2,val2);


        analogWrite(outPin,102);
            analogWrite(outPin2,51);
        SERIAL.print(val1  );
        SERIAL.print(val2);      
        SERIAL.println();
        }
        else
        {
          //if (readValue3 > 650 || readValue3 < 350){ 
          //digitalWrite(8,LOW);
          //delay(5000);}
          //else{digitalWrite(8,HIGH);}
         analogWrite(outPin,102);
         analogWrite(outPin2,51);
        }
    }
