/*! \def myLibrary_cpp
 \ This is a library to use with the save the rain project's rain gauges. 
 The functions below are apart of the myLibrary class. 

Save The Rain Project:
Waspmote Pro V1.2 
Rain Gauge Library v1.1
By: Kyle Nucera
kjnucera@syr.edu


# To Do:
Y  Add a sensor type to the inputs of the various send functions.
Y Move the send function into their respective functions.
Y Use RTC.data/ RTC.TIME for the data frames
N Figure out a a way to get the computer time and put it into the RTC function. 
N Also write code to allow OTA/ The meshlium to update the time of each of the waspmotes

 */

/******************************************************************************
 * Includes
 ******************************************************************************/

#ifndef __WPROGRAM_H__
#include <WaspClasses.h>
//#include "../XBeeDM/WaspXBeeDM.h"
#endif

#include "StRLib1.h"

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
  xbeeDM.setMeshNetworkRetries(0x06);
  dp.println(debug,"Rain Gauge Test:");
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
void Rain_Gauge::read_Time(){
  dp.print(debug, "Time day of week, YY/MM/DD, HH:MM:SS]:");
  dp.println(debug,RTC.getTime());
  dp.println(debug,"------------------------------------------");
}
void Rain_Gauge::set_Power(int val){
  if (val == 3)
    PWR.setSensorPower(SENS_3V3,SENS_ON);
  else if (val == 5)
    PWR.setSensorPower(SENS_5V,SENS_ON);
  else
    dp.println(debug,"Power not set.");
}
char Rain_Gauge::convert_Pressure(int firstVal, int secondVal, float * combVal){
   // Converts 2 int bytes to char * and then concatenates them
   char newVar[10];
   int totalVal = 0;
   totalVal = firstVal * 256 + secondVal; 
   //If you want pressure as a string value.
   //sprintf((char*)newVar, "%i\0",totalVal);
   //strcpy(combVal,newVar);
   *combVal = totalVal;
}
float Rain_Gauge::convert_Temp(int val, float * temp){
  float cTemp, fTemp;
  val = val << 3; // shift the temperature to the left by 3 bits
  cTemp = ((((float)val/2047)*200)-50);
  dp.print(debug,"Temperature in Celsius: ");
  dp.println_Float(debug, cTemp);
  fTemp = ((cTemp*9)/5) + 32;
  dp.print(debug,"Temperature in Fahrenheit: ");
  dp.println_Float(debug,fTemp);
  *temp = cTemp;
   //converts int to char *
   //float2string(cTemp,temp,3);
}
char Rain_Gauge::read_Pressure(float * combVal, float * temp, int I2C_ADDRESS){
  int val, firstVal, secondVal, thirdVal, Status;
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
  dp.println_Int(debug,*combVal);
  dp.println(debug,"-------------------------------");
  // convert totalval to a char variable named combVal
  dp.print(debug,"RAWTemp: ");
  dp.println_Int(debug,thirdVal);
  convert_Temp(thirdVal, temp); //gets the temperature in fahrenheit
  dp.println(debug,"-------------------------------");
  Status = check_I2C(temp, combVal);
  if (Status == 1)
    read_Pressure(combVal,temp,I2C_ADDRESS);
  //pressure2string(firstVal, secondVal, combVal); //gets RAW pressure value
}
char Rain_Gauge::average_Pressure(float * combVal, float * temp, int I2C_ADDRESS){
  //int j = 0;
  //float ave_Press, ave_Temp;
  //while (j < 10){
    //read_Pressure(combVal,temp,I2C_ADDRESS);
    //ave_Press = ave_Press + *combVal;
    //ave_Temp = ave_Temp + *temp;
    //j++;
  //}
  //*combVal = ave_Press/10;
  //*temp = ave_Temp/10;
}
int Rain_Gauge::send_RG(float* value, char* message, float* temp, char* MAC_ADDRESS){
  // Added to fix the only send once error
  xbeeDM.ON();
  // Create the packet that we will send wirelessly
  packetXBee* packet; 
  // Create new frame (ASCII)
  frame.createFrame(ASCII,message); 
  // add frame field (String message) writes pressure value to SD Card
  frame.addSensor(SENSOR_PA, *value);
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
    Utils.blinkGreenLED();
    dp.println(debug,"Packet sent successfully.");
    free(packet);
    packet=NULL;
    return 1;
  }
  else
  {
    Utils.blinkRedLED();
    dp.println(debug,"Didn't send value.");
    free(packet);
    packet=NULL;
    return 0;
  }
}
void Rain_Gauge::write_SD(float* value, char* message, float* temp){
  SD.ON();
  
  char* path="/data";
  char* filename="/data/log";
  // create path
  SD.mkdir(path);
    
  // Create files
  SD.create(filename);
  frame.setID(message);
  // Create new frame (ASCII)
  frame.createFrame(ASCII); 
  // add frame field (String message) that writes the date and time
  //frame.addSensor(SENSOR_DATE,RTC.year,RTC.month,RTC.date);
  //frame.addSensor(SENSOR_TIME,RTC.hour,RTC.minute,RTC.second);
  frame.addSensor(SENSOR_STR, RTC.getTime());
  //frame.addSensor(SENSOR_DATE,RTC.getTime());
  // add frame field (String message) writes pressure value to SD Card
   frame.addSensor(SENSOR_PA, *value);
  // add frame field (string message) writes temperature to SD card
  frame.addSensor(SENSOR_TCA, *temp);
  // // add frame field (Battery level)
  frame.addSensor(SENSOR_BAT, (uint8_t) PWR.getBatteryLevel());
  if(SD.appendln(filename, frame.buffer, frame.length)) 
    dp.println(debug,"Append successful");
  else 
    dp.println(debug,"Append failed");
  //delay(500);
  SD.OFF();
}
void Rain_Gauge::hibernate(){
  dp.println(debug,"------------------------------------------");
  dp.print(debug,"Battery Level: ");
  dp.print_Int(debug,PWR.getBatteryLevel());
  dp.println(debug,"%");
  RTC.ON();
}
int Rain_Gauge::check_I2C(float* temp, float* pressure)
  {
    float badTemp = -50.00;
    float badPress = 0.00;
    char* checkTime  = (char*) RTC.getTime();
    int reset = 0;
    if ((*temp == badTemp) && (*pressure == badPress)) 
      reset = reset_I2C();
    else 
      reset = 0;
    if (checkTime == "error")
        reset_PWR();
    dp.print(debug,"I2C Reset: ");
    dp.println_Int(debug,reset);
    return reset;
  }
   int Rain_Gauge::reset_I2C()
  {
    PWR.sleep(WTD_500MS, ALL_OFF);
    if( intFlag & WTD_INT )
    {
     intFlag &= ~(WTD_INT);
    }
    Wire.close();
    dp.println(debug,"Executing: Wire.begin()");
    Wire.begin();
    dp.println(debug,"Executing: RTC.ON()");
    set_Power(5);
    delay(500);
    RTC.ON();
    SD.ON();
    
    char* path="/error";
    char* filename="/error/log";
    // create path
    SD.mkdir(path);
      
    // Create files
    SD.create(filename);
    // Create new frame (ASCII)
    frame.createFrame(ASCII); 
    // add frame field (String message) that writes the date and time
    //frame.addSensor(SENSOR_DATE,RTC.year,RTC.month,RTC.date);
    //frame.addSensor(SENSOR_TIME,RTC.hour,RTC.minute,RTC.second);
    frame.addSensor(SENSOR_STR, (char *) "I2C Reset");
    if(SD.appendln(filename, frame.buffer, frame.length)) 
      dp.println(debug,"Append successful");
    else 
      dp.println(debug,"Append failed");
    //delay(500);
    SD.OFF();
    return 1;
  }
bool Rain_Gauge::set_Time(char* date){
  int address = 1030;
  if(Utils.readEEPROM(address) == 1){
     Utils.writeEEPROM(address,0);
  }
  else
  {
    if (RTC.setTime(date))
      dp.println(debug,"DID NOT Set time Internally");
    else
      dp.println(debug,"Setting time Internally");
  }
}
// void Rain_Gauge::power_SAVE(){
//   if (PWR.getBatteryLevel() < 10)
//     while ( PWR.getBatteryLevel() < 20)
//       PWR.deepSleep("00:01:00:00",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
// }
void Rain_Gauge::reset_PWR(){
    Utils.writeEEPROM(1030,1);
    SD.ON();
    dp.println(debug,"POWER RESET");
    char* path="/error";
    char* filename="/error/log";
    // create path
    SD.mkdir(path);
      
    // Create files
    SD.create(filename);
    // Create new frame (ASCII)
    frame.createFrame(ASCII); 
    frame.addSensor(SENSOR_STR, (char *) "POWER RESET");
    if(SD.appendln(filename, frame.buffer, frame.length)) 
      dp.println(debug,"Append successful");
    else 
      dp.println(debug,"Append failed");
    SD.OFF();
    PWR.reboot();
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
