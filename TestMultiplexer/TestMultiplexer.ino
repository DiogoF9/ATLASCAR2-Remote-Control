
int readValue;
int inPin = A0;
int outPin = 4;
int outPin2 = 7;
int outPin3 = 8;
int outPin4 = 2;

void setup() {
  // put your setup code here, to run once:
pinMode(outPin,OUTPUT);
pinMode(outPin2,OUTPUT);
pinMode(outPin3,OUTPUT);
pinMode(outPin4,OUTPUT);
pinMode(inPin,INPUT);
Serial.begin(9600);
}

void loop() {

  digitalWrite(outPin,LOW);
  digitalWrite(outPin2,LOW);
  digitalWrite(outPin3,LOW);
  digitalWrite(outPin4,LOW);

  readValue = analogRead(A0);
  
  Serial.println(readValue);      //the first variable for plotting
}
