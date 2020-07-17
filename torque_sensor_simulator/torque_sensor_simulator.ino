#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>

int pin = 9;

void setup() {
  Serial.begin(9600); // For debug use
  Serial.println("CAN Read - Testing receival of CAN Bus message");  
  delay(1000);
  
  if(Canbus.init(CANSPEED_500))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");
    
  delay(1000);

  pinMode(pin,OUTPUT);
}

void loop(){

  tCAN message;
if (mcp2515_check_message()) 
  {
    if (mcp2515_get_message(&message)) 
  {
        if(message.id == 0x236)
             {
               
               Serial.print("ID: ");
               Serial.print(message.id,HEX);
               Serial.print(", ");
               Serial.print("Data: ");
               Serial.print(message.header.length,DEC);
               for(int i=0;i<message.header.length;i++) 
                { 
                  Serial.print(message.data[i],HEX);
                  Serial.print(" ");
                }
               Serial.println("");

             // adjust the PWM output value corresponding to the data of the CAN message
             //int volt = message.data[0]; //some kind of correlation needs to be done here
             //analogWrite(pin,volt);
             }

             
           }}

}
