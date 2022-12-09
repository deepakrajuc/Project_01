#include <Arduino.h>


#define RX_pin 16                  //define the RX pin 16 in esp32 UART2 communication
#define TX_pin 17                  //define the TX pin 17 in esp32 UART2 communication
#define BaudRate 115200            //define the baudRate for UART2 communication
#define SerialMode SERIAL_8N1      //define Serial mode
void print_data();
void parse_the_data();
char c;                            
String dataIn;                                                     //init.variables for seperate the string data to 3 different node data
int8_t indexofA, indexofB, indexofC;
String node1data, node2data, node3data;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);                                           //just init. the 2 serial uart communication 1 for serial monitor
  Serial2.begin(BaudRate, SerialMode, RX_pin, TX_pin);            //Another one for serial UART communication

}

void loop() {
  // put your main code here, to run repeatedly:
while(Serial2.available()>0)                                       //In while condition just check serial data is available
{                                     
  c = Serial2.read();                                              //Store the serial data in c variable
  if(c=='\n')                                                      //If c = next line Its come out form the loop
  {
    break;
  } 
  else                                                             //else the data is added to dataIn variable
  {
    dataIn += c;
  }
}
  if(c=='\n')                                                      //If c = nextLine c and dataIn variable clears
  {                                                                 // those two functions calls for to seperate data from string and print data
    parse_the_data();                                                
    print_data();
    c = 0;
    dataIn = "";
  }
}

void parse_the_data()
{
  indexofA = dataIn.indexOf("A");                                  // data is sperated by using the index of the string
  indexofB = dataIn.indexOf("B");
  indexofC = dataIn.indexOf("C");
  node1data = dataIn.substring(0,indexofA); 
  node2data = dataIn.substring(indexofA+1, indexofB);
  node3data = dataIn.substring(indexofB+1, indexofC); 
}

void print_data()
{
  Serial.println("node1data = " + node1data);
  Serial.println("node2data = " + node2data);
  Serial.println("node3data = " + node3data);
  Serial.println("=======================");
}