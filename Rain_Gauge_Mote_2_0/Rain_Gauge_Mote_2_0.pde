// Waspmote Pro V1.2 
// Save The Rain Project:
// Rain Gauge v1.5
// By: Kyle Nucera
// kjnucera@syr.edu

// Includes the Necessary Libraries 
#include <WaspXBeeDM.h>
#include <StRLib.h>
#include <WaspFrame.h>

// Change to false to disable debugging. Vise-Versa is true.
bool debug_Mode = true;
//bool debug_Mode = false;

// Gateway Mac Addresses ( Wireless Communication )
char* MAC_ADDRESS="0013A20040794BAD"; // new meshlium
//char* MAC_ADDRESS="0013A20040788EE2"; // kyle's usb gateway
//char* MAC_ADDRESS="0013A20040676BE0"; // Nick's USB gateway

// Address of I2C Pressure Transducer
int I2C_ADDRESS1 = 40; // Address 0x28
int I2C_ADDRESS2 = 72; // Address 0x48
int I2C_ADDRESS3 = 88; // Address 0x58
int I2C_ADDRESS4 = 104; //Address 0x68 -- Doesn't work with waspmote :(

//Global Names for the location we're sending from
char* loc_0 = "Link+";
char* loc_1 = "Waverly_RG";
char* loc_3 = "Bird_HN_W";
char* loc_4 = "LINK+_TEST4";
char* loc_5 = "LINK+_TEST1";
char* loc_6 = "LINK+_TEST2";
char* loc_7 = "LINK+_TEST3";
char* loc_8 = "LINK+_TEST5";


void setup() {
  Display dp;
  RainGauge.set_Debug(debug_Mode);
 // RainGauge.Init();
  USB.ON();
  xbeeDM.ON();
  xbeeDM.setMeshNetworkRetries(0x06);
  Wire.begin();
  //PCB needs power to have RTC work
  RainGauge.set_Power(5);
  RTC.ON();
  // Setting time [yy:mm:dd:dow:hh:mm:ss]
  // Sunday = 01, Monday = 02, ...
  // hours are in a 1-24 format.
  RainGauge.set_Time("15:07:24:06:09:48:00");
}

void loop() {
  /////////////////////////////////////////////////////////////  
  // Powering on the PCB Board
  /////////////////////////////////////////////////////////////  
  RainGauge.set_Power(5);
  /////////////////////////////////////////////////////////////  
  // Variable Declarations
  /////////////////////////////////////////////////////////////
  //char convertFloat[10];
  float combVal[10];
  float temp[5];
  // char combVal2[7] = "000000";
  // float temp2[5];
  /* 3.3VDC Analog Variable */
  //double threeVolt = 3.3;
  /* 5VDC Analog Variable*/
  //double fiveVolt = 5;
  /////////////////////////////////////////////////////////////
  // Getting Input Values
  /////////////////////////////////////////////////////////////
  //analog voltage between 0 - 3.3v (MAXBOTIX)
  //RainGauge.read_Analog(convertFloat, fiveVolt);
  //Digital Value
  //RainGauge.read_digital();
  //
  RainGauge.read_Time();
  delay(500);
  /////////////////////////////////////////////////////////////  
  // Read I2C Device
  /////////////////////////////////////////////////////////////  
  RainGauge.read_Pressure(combVal, temp, I2C_ADDRESS2);
  delay(500);
  //RainGauge.read_Pressure(combVal2, temp2, I2C_ADDRESS3);
  /////////////////////////////////////////////////////////////
  // Write to SD card 
  /////////////////////////////////////////////////////////////
  RainGauge.write_SD(combVal, loc_5, temp);
  delay(500);
  /////////////////////////////////////////////////////////////
  // Send to Gateway
  /////////////////////////////////////////////////////////////
  RainGauge.send_RG(combVal,loc_5,temp,MAC_ADDRESS);
  delay(500);
  /////////////////////////////////////////////////////////////  
  // Saving time in case of Malfunction
  /////////////////////////////////////////////////////////////  
  /////////////////////////////////////////////////////////////
  // Sleep For Fifteen Minutes - Days:Hours:Minutes:Seconds
  /////////////////////////////////////////////////////////////
  PWR.deepSleep("00:00:00:05",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
  RainGauge.hibernate();
}



