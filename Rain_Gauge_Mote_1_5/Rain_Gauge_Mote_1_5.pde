// Waspmote Pro V1.2 
// Save The Rain Project:
// Rain Gauge v1.2
// By: Kyle Nucera
// kjnucera@syr.edu

// Includes the Necessary Libraries 
//#include <myLibrary.h>
#include <WaspXBeeDM.h>
#include <StRLib.h>
#include <WaspFrame.h>
//#include <myLibrary.h>

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
int I2C_ADDRESS4 = 104; //Address 0x68

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
  RTC.ON();
  SD.ON();
  if (RTC.setTime("15:06:22:04:12:30:00"))
    USB.println("DID NOT Set time Internally");
  else
    USB.println("Setting time Internally");
  // Setting time [yy:mm:dd:dow:hh:mm:ss]
   // Sunday = 01, Monday = 02, ...
    // hours are in a 1-24 format.
   // RTC.setTime("15:06:15:04:12:30:00");
  //if (debug_Mode){
//  RainGauge.Init();
//  if (xbeeDM.setRTCfromMeshlium(MAC_ADDRESS))
//    USB.println("Set Time From Meshlium");
//   else
//     USB.println("DID NOT Set Time From Meshlium");
//   USB.println(F("-------------------------------"));
}

void loop() {
  RTC.ON();
  // 3.3VDC Power
  //double threeVolt = 3.3;
  // PWR.setSensorPower(SENS_3V3,SENS_ON); 
  //5VDC Power
  //double fiveVolt = 5;
  PWR.setSensorPower(SENS_5V,SENS_ON); 
  /////////////////////////////////////////////////////////////  
  // 0. Declarations
  /////////////////////////////////////////////////////////////
  char convertFloat[10];
  char combVal[7] = "000000";
  float temp[5];
  char combVal2[7] = "000000";
  float temp2[5];
  int error = 0;
  /////////////////////////////////////////////////////////////
  // 1. Getting Inputs
  /////////////////////////////////////////////////////////////
  //analog voltage between 0 - 3.3v (MAXBOTIX)
  //RainGauge.read_Analog(convertFloat, fiveVolt);
  if (RTC.setTime("15:06:22:04:12:30:00"))
    USB.println("DID NOT Set time Internally");
  else
    USB.println("Setting time Internally");
  USB.print(F("Time day of week, YY/MM/DD, HH:MM:SS]:"));
  USB.println(RTC.getTime());
  USB.println("------------------------------------------");
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
  RainGauge.send_RG(combVal,loc_1,temp,MAC_ADDRESS);
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
  PWR.deepSleep("00:00:00:10",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
  if( intFlag & RTC_INT )
  { 
    Utils.blinkLEDs(300);
    intFlag &= ~(RTC_INT);
    freeMemory();
  }
  USB.println("------------------------------------------");
  USB.print(F("Battery Level: "));
  USB.print(PWR.getBatteryLevel(),DEC);
  USB.println(F(" %"));
}



