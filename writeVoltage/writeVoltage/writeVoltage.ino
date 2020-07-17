
/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

int outPin = 5;
int outPin2 = 6;

float pot = 51; //140 110

float dif = 15;
float pot1 = 255/2;
// combinação de potência principal e secundária, respetivamente que faz o volante rodar para o lado esquerdo

float pot2 = 255/2-dif;
// qual a combinação que permite rodar para o lado direito?
int pot3 = 105;
int pot4 = 200;

float val = 0.25;
float val1;
float val2;

float bin1;
float bin2;

int readValue1;
int readValue2;
int readValue3;

void setup() {
  // put your setup code here, to run once:
pinMode(outPin,OUTPUT);
pinMode(outPin2,OUTPUT);
pinMode(8,OUTPUT);
pinMode(7,OUTPUT);
pinMode(4,OUTPUT);
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);
SERIAL.begin(9600);
val1 = 2.5+val;
val2 = 2.5-val;
}

void loop() {
  // loop para rodar tudo para um lado e depois tudo para o outro começando parado:
  pot=51;
  //analogWrite(outPin,pot3);
  //analogWrite(outPin2,pot4);
  //delay(5000);
  readValue1 = analogRead(A0); // modo atual de condução - manual ou automático
  readValue2 = analogRead(A1); // sinal torque sensor
  readValue3 = analogRead(A2); // sinal torque sensor
  
  // teste simples de tensões
 // analogWrite(outPin,pot);
 // analogWrite(outPin2,255/2);

  //incremento controlado direita
 // if(pot > 110){
 // pot = pot -0.5;}
  
  //incremento controlado esquerda
  //if(pot < 140){
  //pot = pot +0.5;}
  //delay(20000);

  if (readValue3 > 650 || readValue3 < 350){ 
  digitalWrite(8,LOW);
  delay(5000);}
  else{digitalWrite(8,HIGH);}
  //digitalWrite(8,HIGH);
  digitalWrite(7,LOW);

val1=val1+0.01;
val2=val2-0.01;


   bin1 = val1*255/5;
   bin2 = val2*255/5;
   
pot2 = pot2-0.1;
 //val1 = 25.5;
 //val2 = 51;
  analogWrite(outPin,255/2+15);
  analogWrite(outPin2,255/2-15);
  delay(1000);
  
//val1 = 51;
//val2 = 102;
  //analogWrite(outPin,val1);
  //analogWrite(outPin2,val2);
//delay(5000);

   Serial.print(pot1);      //the first variable for plotting
  Serial.print(",");              //seperator
  Serial.print(pot2);      //the first variable for plotting
  Serial.print(",");              //seperator
  Serial.println(readValue3);          //the second variable for plotting including line break
    
  //val2 = val2-0.1;
    //    SERIAL.println();
  //SERIAL.print(pot);
  //SERIAL.print("\t");
  //SERIAL.print(pot1);
  
    //    SERIAL.println();
}
