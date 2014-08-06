// Waspmote Pro V1.2 
// Save The Rain Project:
// Hop Node v2.0
// By: Kyle Nucera
// kjnucera@syr.edu

// Includes the Necessary Libraries 
#include <WaspXBeeDM.h>
#include <myLibrary.h>
#include <WaspFrame.h>

// Change to false to disable debugging.
bool debug_Mode = false;

//char* MAC_ADDRESS="0013A200408DE558"; // DA MESHLIUM first
char* MAC_ADDRESS="0013A20040794BAD"; // new meshlium
//char* MAC_ADDRESS="0013A20040788EE2"; // kyle's usb gateway
//char* MAC_ADDRESS="0013A20040896768"; // Nick's USB gateway

char* loc_0 = "link+_HN";
char* loc_1 = "NewHouse_HN";
char* loc_2 = "Bird_HN";


packetXBee* packet; 
 
void setup()
{
  HopNode.set_Debug(debug_Mode);
  HopNode.Init();
  // Setting time [yy:mm:dd:dow:hh:mm:ss]
  RTC.setTime("14:05:29:05:12:11:00");
  if (debug_Mode)
    USB.print(F("Setting time"));
}

void loop(){
  HopNode.send_InTemp(MAC_ADDRESS);
  // send the battery level
  delay(1000);
  
   // Go to sleep for an hour disconnecting all the switches and modules
   // Format:   "Days:Hours:Minutes:Seconds"
   PWR.deepSleep("00:00:15:00",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
}
