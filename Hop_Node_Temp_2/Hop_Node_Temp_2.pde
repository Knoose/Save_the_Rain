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
//char* MAC_ADDRESS="0013A20040788EE2"; // Kyle's usb gateway
//char* MAC_ADDRESS="0013A20040896768"; // Nick's USB gateway
//char* MAC_ADDRESS="0013A20040676BE0"; // LAB usb gateway

//Locations for our Nodes
char* loc_00 = "Test_HN";
char* loc_0 = "link+_HN";
char* loc_1 = "WP_Test";
char* loc_2 = "Bird_HN_S";
char* loc_3 = "Bird_HN_W";
char* loc_4 = "NH_HN_W";
char* loc_5 = "NH_HN_E";
char* loc_6 = "Whit_HN";
char* loc_7 = "Schine_HN";

packetXBee* packet; 
 
void setup()
{
  HopNode.set_Debug(debug_Mode);
  HopNode.Init();
  // Setting time [yy:mm:dd:dow:hh:mm:ss] 
    // Sunday = 01, Monday = 02, ...
    // hours are in a 1-24 format.
  RTC.setTime("14:08:08:06:15:10:00");
  if (debug_Mode)
    USB.println("Setting time");
}

void loop(){
  delay(500);
  HopNode.send_InTemp(MAC_ADDRESS, loc_7);
  delay(500);
  // frame.showFrame();
  // send the battery level
  HopNode.send_Batt(MAC_ADDRESS, loc_7);
  //frame.showFrame();
  delay(500);
   // Go to sleep for an hour disconnecting all the switches and modules
   // Format:   "Days:Hours:Minutes:Seconds"
   PWR.deepSleep("00:00:15:00",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
   delay(2000);
}
