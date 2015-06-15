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

#include "myLibrary.h"

//#define DEBUG 


/******************************************************************************
 *  RAIN GAUGE CLASS: PUBLIC FUNCTIONS
 ******************************************************************************/
Rain_Gauge::Rain_Gauge(){
}  
int Rain_Gauge::Init(){ 
  // Open the USB connection
  // define folder and file to store data ( SD CARD )
  char* path="/data";
  char* filename="/data/log";
  // PAN (Personal Area Network) Identifier
  uint8_t  PANID[2]={0x7F,0xFF}; 
  USB.ON();
  // init XBee 
  xbeeDM.ON();
  xbeeDM.setMeshNetworkRetries(0x06);
  if (debug) 
    USB.println(F("Rain Gauge Test: #1"));
  // Set I2C ON
  Wire.begin();
  
  // Powers RTC up, init I2C bus and read initial values
  if (debug) 
    USB.println(F("Init RTC"));
  RTC.ON();
  // Set SD ON
  SD.ON();
  
  // create path
  SD.mkdir(path);
    
  // Create files
  SD.create(filename);

  // init Accelerometer
  ACC.ON();
  
  delay(2000);
  xbeeDM.flush();
  
  ///////////////////////////////////
  //set PANID 
  /////////////////////////////////// 
  xbeeDM.setPAN(PANID);
  
  // check flag
  if (debug) {
    if( xbeeDM.error_AT == 0 ) 
    {
       USB.println(F("PANID set OK"));
    }
    else 
    {
       USB.println(F("Error setting PANID"));  
    }
  }
  /////////////////////////////////////
  // write values to XBee module memory
  /////////////////////////////////////
  xbeeDM.writeValues();
 
  // check flag
  if (debug) {
    if( xbeeDM.error_AT == 0 ) 
    {
        USB.println(F("write values OK"));
    }
    else
    {
        USB.println(F("Error writing values"));  
    }
      USB.println(F("-------------------------------")); 
  }
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
char Rain_Gauge::convert_Temp(int val, char * temp){
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
   float2string(cTemp,temp,3);
}
int Rain_Gauge::float2string(float f, char* c, uint8_t prec){
  // float value, where you want to store it, how many decimal places you want
  int fix = 1;
  int p = f;
  f -= p;
  if ( f < 0.1 ){
    fix = 0;
  }
  while (prec > 0) {
    f *= 10;
    prec--;
  }
  int q = f;
  if ( fix == 0){
     sprintf(c, "%i.%i%i\0",p,fix,q);
  }
  else
    sprintf(c, "%i.%i\0",p,q);
}
char Rain_Gauge::pressure2string(int firstVal, int secondVal, char * combVal){
  // Converts 2 int bytes to char * and then concatenates them
   char nfv[10];
   char nsv[10];
   sprintf((char*)nfv, "%i\0",firstVal);
   sprintf((char*)nsv, "%i\0",secondVal);
   strcpy(combVal,nfv);
   strcat(combVal,nsv);
}
char Rain_Gauge::read_Analog(char * convertFloat, double volt_level){
  float val1 = analogRead(ANALOG1);
  val1 = (val1/1023) * volt_level;
  //function to convert floating point numbers to integers
  float2string(val1, convertFloat, 3);
}
char Rain_Gauge::read_Pressure(char * combVal, char * temp, int I2C_ADDRESS){
  int val, firstVal, secondVal, thirdVal, Status, totalVal=0;
  // Read I2C Device
  Wire.requestFrom(I2C_ADDRESS, 3);
   while(Wire.available()){    // slave may send less than requested
    for (int i = 0; i < 3 ; i++){
      val = Wire.receive(); //reads the value from the I2C address 
      if ( i == 0 ){// makes sure we only get the status bits for the first byte
        Status = val >> 14; // shifts the value over by 14 bits so we can get the status bits
        if (Status != 0){ // checks the the first two status bits to make sure there are no issues.
          if (debug) 
            USB.println(F("Error! Data not valid"));
        }
        else{
          firstVal = val; // stores the first byte
          if (debug){
            USB.print(F("FirstVal: "));
            USB.println(firstVal);
          }
        }   
      }
      else if (i == 1){
        secondVal = val; // stores the second byte
        if (debug){
          USB.print(F("SecondVal: "));
          USB.println(secondVal);
        }
      }
      else if ( i == 2){
        thirdVal = val; // stores the third byte
        if (debug){
          USB.print(F("temperature: "));
          USB.println(thirdVal);
        }
      }
    } 
  }
  if (debug)
    USB.println(F("-------------------------------"));
  // Calculate the the total based on base 256
  // convert totalval to a char variable named combVal
  convert_Temp(thirdVal, temp); //gets the temperature in fahrenheit
  convert_Pressure(firstVal, secondVal, combVal);
  if (debug)
    USB.println(F("-------------------------------"));
  //pressure2string(firstVal, secondVal, combVal); //gets RAW pressure value
  if (debug){
    USB.print(F("Decimal Pressure: "));
    USB.println(combVal);
    //USB.print(F("Celsius Temp: "));
    //USB.println(temp);
  }
}
int Rain_Gauge::send_Batt(char* MAC_ADDRESS, char* message){
  // Added to fix the only send once error
  xbeeDM.ON();
  // Creates a packet to send
  packetXBee* packet; 
  // Create new frame (ASCII)
  frame.createFrame(ASCII, message); 
  frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel()); 
  packet=(packetXBee*) calloc(1,sizeof(packetXBee)); 
  // Choose transmission mode: UNICAST or BROADCAST
  packet->mode=UNICAST; 
  // set destination XBee parameters to packet
  xbeeDM.setDestinationParams(packet, MAC_ADDRESS, frame.buffer, frame.length); 
  // send XBee packet
  xbeeDM.sendXBee(packet);
  // check TX flag
  delay(500);
  if( xbeeDM.error_TX == 0 )
  {
    if (debug)
      USB.println("Packet sent successfully.");
    free(packet);
    packet=NULL;
    return 1;
  }
  else
  {
    if (debug)
      USB.println("Didn't send value.");
    free(packet);
    packet=NULL;
    return 0;
  }
}
int Rain_Gauge::send_Frame(char* value, char* message, char* MAC_ADDRESS){
  // Added to fix the only send once error
  xbeeDM.ON();
  // Create the packet that we will send wirelessly
  packetXBee* packet; 
  // Create new frame (ASCII)
  frame.createFrame(ASCII,message); 
  // add frame field (DATE) that writes the current date
  frame.addSensor(SENSOR_DATE, RTC.month, RTC.day , RTC.year);
  // add frame field (Time) that write the current time
  frame.addSensor(SENSOR_TIME, RTC.hour, RTC.minute, RTC.second);
  // add frame field (String message) writes the analog voltage to the SD card
  frame.addSensor(SENSOR_STR, (char *) value);
  // Send To Gateway ( I.E. Meshlium ) 
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
  delay(500);
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
int Rain_Gauge::send_RG(char* value, char* message, char* temp, char* MAC_ADDRESS)
{
// Added to fix the only send once error
  xbeeDM.ON();
  // Create the packet that we will send wirelessly
  packetXBee* packet; 
  // Create new frame (ASCII)
  frame.createFrame(ASCII,"Message"); 
  // add frame field (String message) writes pressure value to SD Card
  frame.addSensor(SENSOR_STR, (char *) value);
  // add frame field (string message) writes temperature to SD card
  frame.addSensor(SENSOR_TCA, temp);
  // add frame field (Battery level)
  frame.addSensor(SENSOR_BAT, (uint8_t) PWR.getBatteryLevel());
  // add frame field (Accelerometer axis)
  //frame.addSensor(SENSOR_ACC, ACC.getX(), ACC.getY(), ACC.getZ() );
  //USB.println(frame.getFrameSize(),DEC);  
  // Send To Gateway ( I.E. Meshlium ) 
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
    USB.println("Packet sent successfully.");
    return 1;
  }
  else
  {
    USB.println("Didn't send value.");
    return 0;
  }
}
int Rain_Gauge::send_RG_old(char* convertFloat, char* combVal, char* temp, char* MAC_ADDRESS){
  // Added to fix the only send once error
  xbeeDM.ON();
  // Create the packet that we will send wirelessly
  packetXBee* packet; 
  // Create new frame (ASCII)
  frame.createFrame(ASCII,"Datalogging V1.1"); 
  // add frame field (String message) that writes the date and time
  frame.addSensor(SENSOR_STR, RTC.getTime());
  // add frame field (String message) writes the analog voltage to the SD card
  frame.addSensor(SENSOR_STR, (char *) convertFloat);
  // add frame field (String message) writes pressure value to SD Card
  frame.addSensor(SENSOR_STR, (char *) combVal);
  // add frame field (string message) writes temperature to SD card
  frame.addSensor(SENSOR_STR, (char *) temp);
  // add frame field (Battery level)
  frame.addSensor(SENSOR_BAT, (uint8_t) PWR.getBatteryLevel());
  // add frame field (Accelerometer axis)
  //frame.addSensor(SENSOR_ACC, ACC.getX(), ACC.getY(), ACC.getZ() );
  //USB.println(frame.getFrameSize(),DEC);  
  // Send To Gateway ( I.E. Meshlium ) 
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
    USB.println("Packet sent successfully.");
    return 1;
  }
  else
  {
    USB.println("Didn't send value.");
    return 0;
  }
}
void Rain_Gauge::write_SD(char* convertFloat, char* combVal, char* temp, char* message){
  char* path="/data";
  char* filename="/data/log";
  SD.ON();
  
  // create path
  SD.mkdir(path);
    
  // Create files
  SD.create(filename);
  // define folder and file to store data ( SD CARD )

  // Create new frame (ASCII)
  frame.createFrame(ASCII,message); 
  // add frame field (String message) that writes the date and time
  frame.addSensor(SENSOR_STR, RTC.getTime());
  // add frame field (String message) writes the analog voltage to the SD card
  frame.addSensor(SENSOR_STR, (char *) convertFloat);
  // add frame field (String message) writes pressure value to SD Card
  frame.addSensor(SENSOR_STR, (char *) combVal);
  // add frame field (string message) writes temperature to SD card
  frame.addSensor(SENSOR_STR, (char *) temp);
  // add frame field (Battery level)
  frame.addSensor(SENSOR_BAT, (uint8_t) PWR.getBatteryLevel());
  if(SD.appendln(filename, frame.buffer, frame.length)) USB.println(F("append ok"));
    else USB.println(F("append failed"));
    delay(500);
  SD.OFF();
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
    frame.addSensor(SENSOR_STR, RTC.getTime());
    // get onboard temperature
    frame.addSensor(SENSOR_IN_TEMP,(float) RTC.getTemperature());
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
