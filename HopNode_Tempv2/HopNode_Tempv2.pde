//char* MAC_ADDRESS="0013A200408DE558"; // DA MESHLIUM first
char* MAC_ADDRESS="0013A20040794BAD"; // new meshlium
//char* MAC_ADDRESS="0013A20040788EE2"; // kyle's usb gateway
//char* MAC_ADDRESS="0013A20040896768"; // Nick's USB gateway
float temperature = 0.0;

//#define DEBUG // comment out this line to remove the serial prints
#include <WaspFrame.h>
#include <WaspXBeeDM.h>
#include <Wire.h>
#include <cstdlib> 
#include <sstream>
#include <stdio.h>
packetXBee* packet; 
 
// PAN (Personal Area Network) Identifier
uint8_t  PANID[2]={0x77,0x7F}; 

// 16-byte Encryption Key
//char*  KEY="WaspmoteLinkKey!"; 
 
 
void setup()
{
  
  USB.ON();  
  // init XBee 
  xbeeDM.ON();
  // Init RTC
  RTC.ON();
  // Setting time [yy:mm:dd:dow:hh:mm:ss]
  RTC.setTime("14:01:31:06:01:00:00");
  // 1/31/2014 ; Friday 10:00:00
  // wait for a second
  delay(1000);
  xbeeDM.flush();
 
  
  /////////////////////////////////////
  // set PANID 
  ///////////////////////////////////// 
  xbeeDM.setPAN(PANID);
  
  // check flag
  if( xbeeDM.error_AT == 0 ) 
  {
    #ifdef DEBUG
    USB.println(F("PANID set OK"));
    #endif
  }
  else 
  {
    #ifdef DEBUG
    USB.println(F("Error setting PANID"));  
    #endif
  }
  
  
  /////////////////////////////////////
  // 3. set encryption mode (1:enable; 0:disable)
  /////////////////////////////////////
  xbeeDM.setEncryptionMode(0);

  // check flag  
  #ifdef DEBUG
  if( xbeeDM.error_AT == 0 ) 
  {
    USB.println(F("encryption set OK"));
  }
  else 
  {
    USB.println(F("Error setting security"));  
  }
  #endif
  
  
  /////////////////////////////////////
  // 4. set encryption key
  /////////////////////////////////////
  //xbeeDM.setLinkKey(KEY);
  
  // check flag
  if( xbeeDM.error_AT == 0 )
  {
    #ifdef DEBUG
    USB.println(F("encryption key set OK"));
    #endif
  }
  else
  {
    #ifdef DEBUG
    USB.println(F("Error setting encryption key")); 
    #endif 
  }
  
 
  /////////////////////////////////////
  // 5. write values to XBee module memory
  /////////////////////////////////////
  xbeeDM.writeValues();
 
  // check flag
  if( xbeeDM.error_AT == 0 ) 
  {
    #ifdef DEBUG
    USB.println(F("write values OK"));
    #endif
  }
  else
  {
    #ifdef DEBUG
    USB.println(F("Error writing values"));  
    #endif
  }
  #ifdef DEBUG
  USB.println(F("-------------------------------")); 
  #endif
}

void loop(){
  //int i =  PWR.getBatteryLevel();
  //USB.println( i);
   delay(2000);
   // Create new frame (ASCII)
   frame.createFrame(ASCII,"Hop_Node_2"); 
   // add frame field (String message) that writes the date and time
   frame.addSensor(SENSOR_STR, RTC.getTime());
   // get onboard temperature
   frame.addSensor(SENSOR_IN_TEMP,(float) RTC.getTemperature());
   // Get battery level 
   //frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());
   // Memory allocation
   packet=(packetXBee*) calloc(1,sizeof(packetXBee)); 
   // Choose transmission mode: UNICAST or BROADCAST
   packet->mode=UNICAST; 
   // set destination XBee parameters to packet
   xbeeDM.setDestinationParams(packet, MAC_ADDRESS, frame.buffer, frame.length); 
   // send XBee packet
   xbeeDM.sendXBee(packet);
   // check TX flag
   #ifdef DEBUG
   if( xbeeDM.error_TX == 0 )
     USB.println(F("ok"));
   else
     USB.println(F("error"));
   frame.showFrame();
   USB.println(F("enter deep sleep"));
   USB.print("Temp: ");
   USB.println(RTC.getTemperature());
   USB.print("Time: ");
   USB.println(RTC.getTime());
   #endif
   // free memory
   delay(2000);
   free(packet);
   packet=NULL;
   // send the battery level
   delay(1000);
   frame.createFrame(ASCII,"Hop_Node_2"); 
   frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());
   // Memory allocation
   packet=(packetXBee*) calloc(1,sizeof(packetXBee)); 
   // Choose transmission mode: UNICAST or BROADCAST
   packet->mode=UNICAST; 
   // set destination XBee parameters to packet
   xbeeDM.setDestinationParams(packet, MAC_ADDRESS, frame.buffer, frame.length); 
   // send XBee packet
   xbeeDM.sendXBee(packet);
    // free memory
   delay(2000);
   free(packet);
   packet=NULL;
   
   
   // Go to sleep for an hour disconnecting all the switches and modules
   PWR.deepSleep("00:00:00:15",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
}
