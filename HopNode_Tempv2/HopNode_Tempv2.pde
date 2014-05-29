// Waspmote Pro V1.2 
// Save The Rain Project:
// Hop Node v2.0
// By: Kyle Nucera
// kjnucera@syr.edu

// Includes the Necessary Libraries 
#include <WaspXBeeDM.h>
#include <myLibrary.h>
#include <WaspFrame.h>

// comment out this line to remove the serial prints
//#define DEBUG 

//char* MAC_ADDRESS="0013A200408DE558"; // DA MESHLIUM first
char* MAC_ADDRESS="0013A20040794BAD"; // new meshlium
//char* MAC_ADDRESS="0013A20040788EE2"; // kyle's usb gateway
//char* MAC_ADDRESS="0013A20040896768"; // Nick's USB gateway


packetXBee* packet; 
 
void setup()
{
  myObject.Init();
  // Setting time [yy:mm:dd:dow:hh:mm:ss]
  RTC.setTime("14:05:29:05:12:11:00");
  #ifdef DEBUG
    USB.print(F("Setting time"));
  #endif
}

void loop(){
  int error = 0;
  error = myObject.send_InTemp(MAC_ADDRESS);
  if (error != 1)
    USB.println("Packet did not send");
  // send the battery level
  delay(1000);
  
   // Go to sleep for an hour disconnecting all the switches and modules
   PWR.deepSleep("00:00:00:15",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
}
