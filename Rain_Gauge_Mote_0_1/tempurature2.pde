#define DEBUG
//Waspmote Pro V1.2 
//I2C and ANALOG Datalogger
//By: Kyle Nucera
//kjnucera@syr.edu

#include <WaspFrame.h>
#include <Wire.h>
#include <cstdlib> 
#include <sstream>
#include <stdio.h>

// define folder and file to store data
char* path="/data";
char* filename="/data/log";

void setup() {
     // Open the USB connection
  USB.ON();
  #ifdef DEBUG
  USB.println(F("Waspmote Test: #1"));
  #endif
  // Set I2C ON
  Wire.begin();
  
  // Powers RTC up, init I2C bus and read initial values
  #ifdef DEBUG
  USB.println(F("Init RTC"));
  #endif
  RTC.ON();
  
  // Setting time [yy:mm:dd:dow:hh:mm:ss]
  RTC.setTime("13:08:25:06:12:33:00");
  #ifdef DEBUG
  USB.print(F("Setting time: "));
  USB.println(F("13:08:05:06:12:33:00"));
  #endif
  // Set SD ON
  SD.ON();
  
  // create path
  SD.mkdir(path);
    
  // Create files
  SD.create(filename);

  // init Accelerometer
  ACC.ON();
  
  // 3.3VDC Power
  PWR.setSensorPower(SENS_3V3,SENS_ON); 
  //5VDC Power
  //PWR.setSensorPower(SENS_5V,SENS_ON); 
  delay(2000);

}


void loop() {
  
  /////////////////////////////////////////////////////////////  
  // 0. Declarations
  /////////////////////////////////////////////////////////////
  char convertFloat[10];
  char combVal[15];
  char temp[10];
  char maxbotix[3];
  /////////////////////////////////////////////////////////////
  // 1. Reading time & Getting Inputs
  /////////////////////////////////////////////////////////////
  #ifdef DEBUG
  USB.print(F("Time day of week, YY/MM/DD, HH:MM:SS]:"));
  USB.println(RTC.getTime());
  #endif
  //analog voltage between 0 - 3.3v
  readAnalog(convertFloat);
  // Read I2C Device
  readI2CPressure(combVal, temp);
  
  ////////////////////////////////////////
  // 2. Create Data frame using the Waspmote Frame class
  /////////////////////////////////////////////////////
  
  // Create new frame (ASCII)
  frame.createFrame(ASCII,"Datalog Test V1.0"); 
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
  
  /////////////////////////////////////////////////////   
  // 3. Append data to file
  /////////////////////////////////////////////////////  
  #ifdef DEBUG
  if(SD.appendln(filename, frame.buffer, frame.length)) USB.println(F("append ok"));
  else USB.println(F("append failed"));
  #endif
  /////////////////////////////////////////////////////
  // 4. Show last 'filename' line
  /////////////////////////////////////////////////////
  #ifdef DEBUG
  USB.println(F("Show last 'filename' line:"));
  USB.println(SD.catln( filename, SD.numln(filename)-1, SD.numln(filename)) );  
  USB.println(F("-----------------------------"));
  #endif
  // Delay for 2 seconds, in milliseconds
  delay(2000); 
}


  /////////////////////////////////////////////////////
  // FUNCTIONS
  /////////////////////////////////////////////////////

char readI2CPressure(char * combVal, char * temp){
  int val, firstVal, secondVal, thirdVal, Status;
  // Read I2C Device
  Wire.requestFrom(0x28, 3);
   while(Wire.available()){    // slave may send less than requested
    for (int i = 0; i < 3 ; i++){
      val = Wire.receive(); //reads the value from address 40 0x28
      if ( i == 0 ){// makes sure we only get the status bits for the first byte
        Status = val >> 14; // shifts the value over by 14 bits so we can get the status bits
        if (Status != 0){ // checks the the first two status bits to make sure there are no issues.
         #ifdef DEBUG
          USB.println(F("Error! Data not valid"));
         #endif
        }
        else{
          firstVal = val; // stores the first byte
           #ifdef DEBUG
           USB.print(F("FirstVal: "));
           USB.println(firstVal);
           #endif
        }   
      }
      else if (i == 1){
        secondVal = val; // stores the second byte
        #ifdef DEBUG
         USB.print(F("SecondVal: "));
         USB.println(secondVal);
        #endif
      }
      else if ( i == 2){
        thirdVal = val; // stores the third byte
        #ifdef DEBUG
         USB.print(F("temperature: "));
         USB.println(thirdVal);
        #endif
      }
    } 
   }
   I2Ctemp(thirdVal, temp); //gets the temperature in fahrenheit
   pressure2string(firstVal, secondVal, combVal); //gets RAW pressure value
   #ifdef DEBUG
    USB.print(F("combVal: "));
    USB.println(combVal);
    USB.print(F("temp: "));
    USB.println(temp);
   #endif
}

char readAnalog(char * convertFloat){
  float val1 = analogRead(ANALOG1);
  val1 = (val1/1023) *5;
  //function to convert floating point numbers to integers
  float2string(val1, convertFloat, 3);
}

int float2string(float f, char* c, uint8_t prec)
// float value, where you want to store it, how many decimal places you want
{
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

char I2Ctemp( int val, char * temp){
  float cTemp, fTemp;
   val = val << 3; // shift the temperature to the left by 3 bits
  // Determines the Temperature in celsius 
   cTemp = ((((float)val/2047)*200)-50);
   #ifdef DEBUG
    USB.print(F("Temperature in Celsius: "));
    USB.println(cTemp);
   #endif
   fTemp = ((cTemp*9)/5) + 32;
   #ifdef DEBUG
    USB.print(F("Temperature in Fahrenheit: "));
    USB.println(fTemp);
   #endif
   //converts int to char *
   float2string(cTemp,temp,3);
 }

char pressure2string( int firstVal, int secondVal, char * combVal){
  // Converts 2 int bytes to char * and then concatenates them
   char nfv[10];
   char nsv[10];
   sprintf((char*)nfv, "%i\0",firstVal);
   sprintf((char*)nsv, "%i\0",secondVal);
   strcpy(combVal,nfv);
   strcat(combVal,nsv);
}

