// demo: CAN-BUS Shield, send data
// loovee@seeed.cc

#include <mcp_can.h>
#include <SPI.h>

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
int readValue1;
int readValue2;
int readValue3;
int outPin=5;
int outPin2=6;
float pot1=102;
float pot2=51;
float val1=255/2;
float val2=255/2;
float poten1 = 255/2;
float poten2 = 255/2;
int i = 0;
int t=0;
int travao =0;

void setup() {
    SERIAL.begin(115200);
    
    pinMode(A3,INPUT);
    pinMode(A2,INPUT);
  pinMode(A0,INPUT);
  pinMode(A4,INPUT);
  pinMode(outPin,OUTPUT);
  pinMode(outPin2,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  
    while (CAN_OK != CAN.begin(CAN_500KBPS)) {            // init can bus : baudrate = 500k
        SERIAL.println("CAN BUS Shield init fail");
        SERIAL.println(" Init CAN BUS Shield again");
        delay(100);
    }
    SERIAL.println("CAN BUS Shield init ok!");
}

unsigned char len = 0;
unsigned char buf[8];
unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void loop() {
  travao=0;
readValue3 = analogRead(A2);

//----------------------------------------------
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
          if (canId == 0x231 && buf[4] == 0x02){
            travao = 1;}
            else {travao=0;}
        }
    }

//------------------------------------------------------------------
  
  i=i+1;
  
  //potenciometro
  poten1 = analogRead(A4);
  if (poten1 > 230) //147.9
  {poten1 = 230;}
  //if (poten1 < 102) //147.9
  //{poten1 = 102;}
  poten2 = poten1/2;

  if(readValue3 > 210){ 
  digitalWrite(8,LOW);
  delay(5000);}
  else{digitalWrite(8,HIGH);}
  //digitalWrite(8,HIGH);
  digitalWrite(7,LOW);

  if (i%20==0 && i/20>0)
  {pot1=pot1-0.5;
  pot2=pot2+0.5;}

  analogWrite(outPin,51);
  analogWrite(outPin2,102);
  
  readValue1 = analogRead(A3);
  readValue2 = analogRead(A0);
  
  Serial.print(readValue3);      //the first variable for plotting
  Serial.print(",");              //seperator
  Serial.println(travao);          //the second variable for plotting including line break
    
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    //stmp[7] = stmp[7] + 1;
    //if (stmp[7] == 100) {
      //  stmp[7] = 0;
       // stmp[6] = stmp[6] + 1;

        //if (stmp[6] == 100) {
          //  stmp[6] = 0;
           // stmp[5] = stmp[6] + 1;
        //}
    //}

    stmp[0]=readValue1/100;
    
    stmp[1]=readValue1%100;
    
    stmp[2]=readValue2/100;
    
    stmp[3]=readValue2%100;

    stmp[4]=t/1000;
    stmp[5]=t%1000;
    CAN.sendMsgBuf(0x500, 0, 8, stmp);
    delay(25);                       // send data per 100ms
    t=t+25;
}

// END FILE
