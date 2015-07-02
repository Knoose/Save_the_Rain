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
//bool debug_Mode = false;
bool debug_Mode = true;

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
char* loc_4 = "1/23/15_TEST";

void setup() {
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
  if (RTC.setTime("15:07:02:05:11:00:00"))
    USB.println("DID NOT Set time Internally");
  else
    USB.println("Setting time Internally");
  //if (debug_Mode){
//  RainGauge.Init();
//if (xbeeDM.setRTCfromMeshlium(MAC_ADDRESS))
//    USB.println("Set Time From Meshlium");
//   else
//     USB.println("DID NOT Set Time From Meshlium");
//   USB.println(F("-------------------------------"));
}

void loop() {
  // Powering on the PCB Board
  RainGauge.set_Power(5);
  /////////////////////////////////////////////////////////////  
  // 0. Declarations
  /////////////////////////////////////////////////////////////
  //char convertFloat[10];
  float combVal[7];
  float temp[5];
  // char combVal2[7] = "000000";
  // float temp2[5];
  /* 3.3VDC Analog Variable */
  //double threeVolt = 3.3;
  /* 5VDC Analog Variable*/
  //double fiveVolt = 5;
  /////////////////////////////////////////////////////////////
  // 1. Getting Inputs
  /////////////////////////////////////////////////////////////
  //analog voltage between 0 - 3.3v (MAXBOTIX)
  //RainGauge.read_Analog(convertFloat, fiveVolt);
  //Digital Value
  //RainGauge.read_digital();
  RainGauge.read_Time();
  delay(500);
  // Read I2C Device
  RainGauge.read_Pressure(combVal, temp, I2C_ADDRESS2);
  delay(500);
  //RainGauge.read_Pressure(combVal2, temp2, I2C_ADDRESS3);
  /////////////////////////////////////////////////////////////
  // 2. Send to Gateway
  /////////////////////////////////////////////////////////////
  RainGauge.write_SD(combVal, loc_0, temp);
  delay(500);
  RainGauge.send_RG(combVal,loc_0,temp,MAC_ADDRESS);
  delay(500);
  /////////////////////////////////////////////////////////////
  // 3. Write to SD card 
  /////////////////////////////////////////////////////////////
  //RainGauge.write_SD(combVal, loc_1, temp);
  delay(500);
  /////////////////////////////////////////////////////////////
  // 4. Sleep For Fifteen Minutes
  /////////////////////////////////////////////////////////////
  // Days:Hours:Minutes:Seconds
 // PWR.deepSleep("00:00:00:05",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
  // RainGauge.set_Power(5);
  RainGauge.hibernate();
}



