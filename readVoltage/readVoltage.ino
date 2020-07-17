int readValue1;
int readValue2;
void setup() {
  pinMode(A3,INPUT);
  pinMode(A0,INPUT);
  Serial.begin(9600);
}

void loop() {
  readValue1 = analogRead(A3);
  readValue2 = analogRead(A0);
  
  Serial.print(readValue1);      //the first variable for plotting
  Serial.print(",");              //seperator
  Serial.println(readValue2);          //the second variable for plotting including line break
    
  delay(250);
}
