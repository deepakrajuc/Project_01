#include <Arduino.h>


#define APP_DEBUG
#define RX_pin 16
#define TX_pin 17
#define BaudRate 115200
#define SerialMode SERIAL_8N1

void sep_node(int n,int a);

#include "painlessMesh.h"
#include <Arduino_JSON.h>



// MESH Details
#define   MESH_PREFIX     "RNTMESH" //name for your MESH
#define   MESH_PASSWORD   "MESHpassword" //password for your MESH
#define   MESH_PORT       5555 //default port


int nodeNumber = 2;
int node1data, node2data, node3data; //variables for 3 node data

//String to send to other nodes with sensor readings
String readings;

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
String getReadings(); // Prototype for sending sensor readings

//Create tasks: to send messages and get readings;
Task taskSendMessage(TASK_SECOND * 5 , TASK_FOREVER, &sendMessage);

String getReadings () {                       //function to seperate data from json file
  JSONVar jsonReadings;
  jsonReadings["node"] = nodeNumber;
  jsonReadings["analog"] = analogRead(A0);
  readings = JSON.stringify(jsonReadings);
  return readings;
}

void sendMessage () {
  String msg = getReadings();
  mesh.sendBroadcast(msg);
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  JSONVar myObject = JSON.parse(msg.c_str());
  
    int node = myObject["node"];
    int analog_data = myObject["analog"];

    sep_node(node,analog_data);
 
  Serial.print("Node: ");
  Serial.println(node);  
  Serial.print("Analog_data:");
  Serial.println(analog_data);

}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void sep_node(int n,int a)
{                                                 //This function to separate 3 diferent data using the node number
  if(n==1){node1data = a; }
  if(n==2){node2data = a; }
  if(n==3){node3data = a; } 
}


void setup() {
  Serial.begin(115200);                                //just begin the serial monitor
  Serial2.begin(BaudRate, SerialMode, RX_pin, TX_pin); //just begin the UART2 serial communication

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  //userScheduler.addTask(taskSendMessage);      // I disable the task to send message 
  //taskSendMessage.enable();

  

}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  Serial.println("=================");
  Serial.println(node1data);
  Serial.println(node2data);
  Serial.println(node3data);
  Serial.println("=================");
 
  Serial2.print(node1data); Serial2.print("A");                      //sending the 3 data to ESp32 using UART2
  Serial2.print(node2data); Serial2.print("B");
  Serial2.print(node3data); Serial2.print("C");

  Serial2.print("\n");

  delay(100);
  
}