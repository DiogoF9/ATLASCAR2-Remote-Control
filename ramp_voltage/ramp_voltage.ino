int outPin = 9;
int outPin2 = 10;
int pot = 511;
int pot2 = 511;
int readValue;

void setup() {
  // put your setup code here, to run once:
pinMode(outPin,OUTPUT);
pinMode(outPin2,OUTPUT);
pinMode(A3,INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (pot < 1023) {
  analogWrite(outPin,pot);
  analogWrite(outPin2,pot2);
  delay(250);
  pot = pot + 5;
  pot2 = pot2 + 5;
}
  
}
