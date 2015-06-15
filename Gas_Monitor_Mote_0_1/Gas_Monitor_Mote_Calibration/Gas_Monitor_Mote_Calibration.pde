#define DEBUG
//Waspmote Pro V1.2 
// Gas Monitoring 
//By: Kyle Nucera
//kjnucera@syr.edu

#include <WaspFrame.h>
#include <Wire.h>
#include <cstdlib> 
#include <sstream>
#include <stdio.h>
#include <WaspSensorGas_v20.h>

 bool CALCO2 = false;
 bool CALCH4 = false;
 bool CALNO2 = true;

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
  RTC.setTime("15:05:21:05:18:30:00");
  #ifdef DEBUG
  USB.print(F("Setting time: "));
  USB.println(F("15:05:21:05:18:30:00"));
  #endif
  // Set SD ON
  SD.ON();
  
  // create path
  SD.mkdir(path);
    
  // Create files
  SD.create(filename);

  // 3.3VDC Power
  //PWR.setSensorPower(SENS_3V3,SENS_ON); 
  //5VDC Power
  //PWR.setSensorPower(SENS_5V,SENS_ON); 
  delay(2000);

}


void loop() {
  SensorGasv20.ON();
  PWR.setSensorPower(SENS_5V,SENS_ON); 

  /////////////////////////////////////////////////////////////  
  // 0. Declarations
  /////////////////////////////////////////////////////////////
  float humidity,pressure,CO_two,NH_three,CH_four, VOC, CO, NO_two;
  /////////////////////////////////////////////////////////////
  // 1. Reading time & Getting Inputs
  /////////////////////////////////////////////////////////////
  if ( CALCO2 == true )
  {
    CO_two = readCO2();
    frame.createFrame(ASCII,"Calibrate CO2"); 
    // add frame field (String message) that writes the date and time
    frame.addSensor(SENSOR_STR, RTC.getTime());
    frame.addSensor(SENSOR_CO2,CO_two);
  }
  if ( CALCH4 == true )
  {
     CH_four = readCH4();
     frame.createFrame(ASCII,"Calibrate CH4"); 
     // add frame field (String message) that writes the date and time
     frame.addSensor(SENSOR_STR, RTC.getTime());
     frame.addSensor(SENSOR_CH4,CH_four);
  }
  if ( CALNO2 == true )
  {
    NO_two = readNO2();
    frame.createFrame(ASCII,"Calibrate NO2"); 
    // add frame field (String message) that writes the date and time
    frame.addSensor(SENSOR_STR, RTC.getTime());
    frame.addSensor(SENSOR_NO2, NO_two);
  }
  //humidity = readHumidity();
 // pressure = readPressure();
 
  //NH_three = readNH3();
 
  //VOC = readVOC();
  //CO = readCO();
  /////////////////////////////////////////////////////////////
  // 2. Create Data frame using the Waspmote Frame class
  /////////////////////////////////////////////////////////////
  // Create new frame (ASCII)
//  frame.createFrame(ASCII,"Gas Monitoring"); 
//  // add frame field (String message) that writes the date and time
//  frame.addSensor(SENSOR_STR, RTC.getTime());
//  
//  frame.addSensor(SENSOR_PA,pressure);
//  frame.addSensor(SENSOR_HUMA,humidity);
//  frame.addSensor(SENSOR_CO2,CO_two);
//  frame.addSensor(SENSOR_NH3,NH_three);
//  frame.addSensor(SENSOR_CH4,CH_four);
//  frame.addSensor(SENSOR_VOC,VOC);
//  frame.addSensor(SENSOR_CO,CO);
  /////////////////////////////////////////////////////   
  // 3. Append data to file
  /////////////////////////////////////////////////////  
  #ifdef DEBUG
  if(SD.appendln(filename, frame.buffer, frame.length)) USB.println(F("append ok"));
  else USB.println(F("append failed"));
  #endif
  USB.println(RTC.getTime());
  /////////////////////////////////////////////////////
  // 4. Show last 'filename' line
  /////////////////////////////////////////////////////
  #ifdef DEBUG
  USB.println(F("Show last 'filename' line:"));
  USB.println(SD.catln( filename, SD.numln(filename)-1, SD.numln(filename)) );  
  USB.println(F("-----------------------------"));
  #endif
  USB.println(RTC.getTime());
  // Delay for 2 seconds, in milliseconds
  delay(2000); 
}
/*
The variable GAIN represents the gain chosen for the amplification stage which is accessed from the sensor’s output. Integer
values can be set between 1 and 101.
The variable RESISTOR determines the load resistance on which the sensor acts before the amplification stage, expressed in
kiloohms. Values between 0 and 100kΩ can be taken (provided that the minimum load resistance shown in the specifications
of each sensor

*/

float readHumidity()
{ 
  // humidity read
  delay(15000);
  float value;
  value = SensorGasv20.readValue(SENS_HUMIDITY);
  // print to the user
  USB.print("Humidity: ");
  USB.println(value);
  return value;
}
float readPressure()
{
  SensorGasv20.setSensorMode(SENS_ON, SENS_PRESSURE);
  delay(30);
  float value;
  value = SensorGasv20.readValue(SENS_PRESSURE);
  USB.print("PRESSURE: ");
  USB.println(value);
  return value;
}
float readCO2()
{
    //co2
    // GAIN = 7 and 10
    // TIME MINIMMUM = 30000
    int GAIN = 7, TIME = 30000;
    SensorGasv20.configureSensor(SENS_CO2, GAIN);
    SensorGasv20.setSensorMode(SENS_ON, SENS_CO2);
    delay(TIME);
    float value;
    value = SensorGasv20.readValue(SENS_CO2);
    // print to the user
    USB.print("CO2: ");
    USB.println(value);
    return value;
}
float readNH3()
  {
    //NH3
    uint16_t SENSOR = SENS_SOCKET4NH3;
    int GAIN = 1, RESISTOR = 10;
    SensorGasv20.configureSensor(SENSOR, GAIN, RESISTOR);
    float value;
    value = SensorGasv20.readValue(SENSOR);
     // print to the user
    USB.print("NH3: ");
    USB.println(value);
    return value;
  }
  
 float readCH4()
 {
    //CH4
    uint16_t SENSOR = SENS_SOCKET2A;
    int GAIN = 1, RESISTOR = 1, TIME = 30000;
    SensorGasv20.configureSensor(SENSOR, GAIN, RESISTOR);
    SensorGasv20.setSensorMode(SENS_ON, SENSOR);
    delay(TIME);
    float value;
    value = SensorGasv20.readValue(SENSOR);
    // print to the user
    USB.print("CH4: ");
    USB.println(value);
    return value;
 }
 
 float readVOC()
 {
     //VOC
     int GAIN = 1, RESISTOR = 1,TIME = 30000;
     SensorGasv20.configureSensor(SENS_SOCKET2B, GAIN, RESISTOR);
     SensorGasv20.setSensorMode(SENS_ON, SENS_SOCKET2B);
     delay(TIME);
     float value;
     value = SensorGasv20.readValue(SENS_SOCKET2B);
     // print to the user
     USB.print("VOC: ");
     USB.println(value);
     return value;
 }
 float readCO()
 {
    int GAIN = 1, RESISTOR = 100;
    float coVal;
    SensorGasv20.configureSensor(SENS_SOCKET3A, GAIN, RESISTOR);
    coVal = SensorGasv20.readValue(SENS_SOCKET3A);
    coVal = SensorGasv20.calculateResistance(SENS_SOCKET3A, coVal, GAIN, RESISTOR);
    // print to the user
    USB.print("CO: ");
    USB.println(coVal);
    return coVal;
 }
 
 float readNO2()
 {
   int GAIN = 1, RESISTOR = 3;
   float value;
   // Configure the 3B sensor socket
   SensorGasv20.configureSensor(SENS_SOCKET3B, GAIN, RESISTOR);
   // Turn on the sensor on socket 3B and wait for stabilization and
   // sensor response time
   SensorGasv20.setSensorMode(SENS_ON, SENS_SOCKET3B);
   delay(40000);
   value = SensorGasv20.readValue(SENS_SOCKET3B);
   value = SensorGasv20.calculateResistance(SENS_SOCKET3B, value, GAIN, RESISTOR);
   USB.print("NO2: ");
   USB.println(value);
   return value;
 }
 
 
