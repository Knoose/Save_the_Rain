/*! \def myLibrary_cpp
 \ This is a library to use with the save the rain project's rain gauges. 
 The functions below are apart of the myLibrary class. 

Save The Rain Project:
Waspmote Pro V1.2 
Rain Gauge Library v1.1
By: Kyle Nucera
kjnucera@syr.edu


# To Do:
Add a sensor type to the inputs of the various send functions.
Move the send function into their respective functions.
Use RTC.data/ RTC.TIME for the data frames
Figure out a a way to get the computer time and put it into the RTC function. 
Also write code to allow OTA/ The meshlium to update the time of each of the waspmotes

 */

/******************************************************************************
 * Includes
 ******************************************************************************/

#ifndef __WPROGRAM_H__
#include <WaspClasses.h>
//#include "../XBeeDM/WaspXBeeDM.h"
#endif

#include "StRLib.h"

//#define DEBUG 


/******************************************************************************
 *  RAIN GAUGE CLASS: PUBLIC FUNCTIONS
 ******************************************************************************/
Rain_Gauge::Rain_Gauge(){
}  
int Rain_Gauge::Init(){ 
  // Open the USB connection
  // PAN (Personal Area Network) Identifier
  uint8_t  PANID[2]={0x7F,0xFF}; 
  //USB.ON();
  // init XBee 
  //xbeeDM.ON();
  xbeeDM.setMeshNetworkRetries(0x06);
  dp.println(debug,"Rain Gauge Test:");
  // Set I2C ON
  //Wire.begin();
  // Powers RTC up, init I2C bus and read initial values
  //dp.println(debug,"Init RTC");
  //RTC.ON();
  
  //dp.println(debug,"Init SD");
  //SD.ON();
  //char* path="/data";
  //char* filename="/data/log";
  
  // create path
  //SD.mkdir(path);
    
  // // Create files
  //SD.create(filename);
  // define folder and file to store data ( SD CARD )
  delay(2000);
  xbeeDM.flush();
  
  ///////////////////////////////////
  //set PANID 
  /////////////////////////////////// 
  xbeeDM.setPAN(PANID);
  
  // check flag
    if( xbeeDM.error_AT == 0 ) 
    {
       dp.println(debug,"PANID set OK");
    }
    else 
    {
       dp.println(debug,"Error setting PANID");  
    }
  /////////////////////////////////////
  // write values to XBee module memory
  /////////////////////////////////////
  xbeeDM.writeValues();
 
  // check flag
    if( xbeeDM.error_AT == 0 ) 
    {
      dp.println(debug,"Write values OK");
    }
    else
    {
      dp.println(debug,"Error writing values");
    }
    dp.println(debug,"-------------------------------"); 
  }
bool Rain_Gauge::set_Debug(bool value){
  debug = value;
}
char Rain_Gauge::convert_Pressure(int firstVal, int secondVal, char * combVal){
   // Converts 2 int bytes to char * and then concatenates them
   char newVar[10];
   int totalVal = 0;
   totalVal = firstVal * 256 + secondVal; 
   sprintf((char*)newVar, "%i\0",totalVal);
   strcpy(combVal,newVar);
}
float Rain_Gauge::convert_Temp(int val, float * temp){
  float cTemp, fTemp;
   val = val << 3; // shift the temperature to the left by 3 bits
  // Determines the Temperature in celsius 
   cTemp = ((((float)val/2047)*200)-50);
   if (debug){
    USB.print(F("Temperature in Celsius: "));
    USB.println(cTemp);
   }
   fTemp = ((cTemp*9)/5) + 32;
   if (debug){
    USB.print(F("Temperature in Fahrenheit: "));
    USB.println(fTemp);
   }
   //converts int to char *
   //float2string(cTemp,temp,3);
}
char Rain_Gauge::read_Pressure(char * combVal, float * temp, int I2C_ADDRESS){
  int val, firstVal, secondVal, thirdVal, Status;
  // Read I2C Device
  dp.print(debug,"I2C Address: ");
  dp.println_Hex(debug,I2C_ADDRESS);
  Wire.begin();
  Wire.requestFrom(I2C_ADDRESS, 3);
   while(Wire.available()){    // slave may send less than requested
    for (int i = 0; i < 3 ; i++){
      val = Wire.receive(); //reads the value from the I2C address 
      if ( i == 0 ){// makes sure we only get the status bits for the first byte
        Status = val >> 14; // shifts the value over by 14 bits so we can get the status bits
        if (Status != 0){ // checks the the first two status bits to make sure there are no issues.
          dp.println(debug,"Error! Data not valid.");
        }
        else{
          firstVal = val; // stores the first byte
          dp.print(debug,"FirstVal: ");
          dp.println_Int(debug,firstVal);
        }   
      }
      else if (i == 1){
        secondVal = val; // stores the second byte
        dp.print(debug,"SecondVal: ");
        dp.println_Int(debug,secondVal);
      }
      else if ( i == 2){
        thirdVal = val; // stores the third byte
      }
    } 
  }
  // Calculate the the total based on base 256
  convert_Pressure(firstVal, secondVal, combVal);
  dp.print(debug,"Decimal Pressure: ");
  dp.println(debug,combVal);
  dp.println(debug,"-------------------------------");
  // convert totalval to a char variable named combVal
  dp.print(debug,"RAWTemp: ");
  dp.println_Int(debug,thirdVal);
  convert_Temp(thirdVal, temp); //gets the temperature in fahrenheit
  dp.println(debug,"-------------------------------");
  //pressure2string(firstVal, secondVal, combVal); //gets RAW pressure value
}
int Rain_Gauge::send_RG(char* value, char* message, float* temp, char* MAC_ADDRESS)
{
  // Added to fix the only send once error
  xbeeDM.ON();
  // Create the packet that we will send wirelessly
  packetXBee* packet; 
  // Create new frame (ASCII)
  frame.createFrame(ASCII,message); 
  // add frame field (String message) writes pressure value to SD Card
  frame.addSensor(SENSOR_STR, (char *) value);
  // add frame field (string message) writes temperature to SD card
  frame.addSensor(SENSOR_TCA, *temp);
  // add frame field (Battery level)
  frame.addSensor(SENSOR_BAT, (uint8_t) PWR.getBatteryLevel());
  // Send To Gateway ( I.E. Meshlium ) 
  packet=(packetXBee*) calloc(1,sizeof(packetXBee)); 
  // Choose transmission mode: UNICAST or BROADCAST
  packet->mode=UNICAST; 
  // set destination XBee parameters to packet
  xbeeDM.setDestinationParams(packet, MAC_ADDRESS, frame.buffer, frame.length); 
  // send XBee packet
  xbeeDM.sendXBee(packet);
  delay(1000);
  // check TX flag
  if( xbeeDM.error_TX == 0 )
  {
    dp.println(debug,"Packet sent successfully.");
    free(packet);
    packet=NULL;
    return 1;
  }
  else
  {
    dp.println(debug,"Didn't send value.");
    free(packet);
    packet=NULL;
    return 0;
  }
}
void Rain_Gauge::write_SD(char* value, char* message, float* temp){
  SD.ON();
  
  char* path="/data";
  char* filename="/data/log";
  
  // create path
  //SD.mkdir(path);
    
  // // Create files
  //SD.create(filename);
  //define folder and file to store data ( SD CARD )

  // Create new frame (ASCII)
  frame.createFrame(ASCII,message); 
  // add frame field (String message) that writes the date and time
  frame.addSensor(SENSOR_STR, RTC.getTime());
  // add frame field (String message) writes pressure value to SD Card
  frame.addSensor(SENSOR_STR, (char *) value);
  // add frame field (string message) writes temperature to SD card
  frame.addSensor(SENSOR_TCA, *temp);
  // // add frame field (Battery level)
  frame.addSensor(SENSOR_BAT, (uint8_t) PWR.getBatteryLevel());
  if(SD.appendln(filename, frame.buffer, frame.length)) 
    dp.println(debug,"Append successful");
  else 
    dp.println(debug,"Append failed");
  delay(500);
}
void Rain_Gauge::hibernate(){
  PWR.deepSleep("00:00:00:01",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
}

/******************************************************************************
 * HOP NODE CLASS: PUBLIC FUNCTIONS
 ******************************************************************************/
Hop_Node::Hop_Node(){
}
int Hop_Node::send_InTemp(char* MAC_ADDRESS, char* message){
     // Added to fix the only send once error
    xbeeDM.ON();
    // Creates a packet to send
    packetXBee* packet; 
    // Create new frame (ASCII)
    frame.createFrame(ASCII,message); 
    // add frame field (String message) that writes the date and time
    //frame.addSensor(SENSOR_STR, RTC.getTime());
    // get onboard temperature
    frame.addSensor(SENSOR_IN_TEMP,(float) RTC.getTemperature());
    // add frame field (Battery level)
    frame.addSensor(SENSOR_BAT, (uint8_t) PWR.getBatteryLevel());
    // Memory allocation
    packet=(packetXBee*) calloc(1,sizeof(packetXBee)); 
    // Choose transmission mode: UNICAST or BROADCAST
    packet->mode=UNICAST; 
    // set destination XBee parameters to packet
    xbeeDM.setDestinationParams(packet, MAC_ADDRESS, frame.buffer, frame.length); 
    // send XBee packet
    xbeeDM.sendXBee(packet);
    delay(1000);
    //clear data packet
    free(packet);
    packet=NULL;
    // check TX flag
    if( xbeeDM.error_TX == 0 )
    {
      if (debug)
      USB.println("Packet sent successfully.");
      return 1;
    }
    else
    {
      if (debug)
      USB.println("Didn't send value.");
      return 0;
   }
}
/// Preinstantiate Objects /////////////////////////////////////////////////////
Rain_Gauge RainGauge = Rain_Gauge();
Hop_Node HopNode = Hop_Node();
