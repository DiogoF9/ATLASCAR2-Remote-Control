

int outPin = 5;
int outPin2 = 6;

void setup() {
  // put your setup code here, to run once:
pinMode(outPin,OUTPUT);
pinMode(outPin2,OUTPUT);
}

void loop() {

  analogWrite(outPin,255);
  analogWrite(outPin2,255/2);
}
