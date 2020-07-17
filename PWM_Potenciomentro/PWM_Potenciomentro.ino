int pin = 9;      
int pin2 = 10;
int analogPin = 0;   // potentiometer connected to analog pin 3
int val;         // variable to store the read value
int val1;
int val2;
int pot=52;

void setup() {
  pinMode(pin, OUTPUT);  // sets the pin as output
  pinMode(pin2, OUTPUT);
}

void loop() {
  val = analogRead(analogPin);  // read the input pin
  val1 = val/4;
  //val1 = val1/2+107; //112.2

  //if (val1 > 170) //147.9
  //{val1 = 170;}

  //val2 = 127.5 + (127.5-val1);

  val2=val1/2;
  analogWrite(pin, pot); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  analogWrite(pin2, val2); //main
  delay(1000);
  pot=pot+10;
}
