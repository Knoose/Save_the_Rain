// Waspmote Pro V1.2 
// Save The Rain Project:
// Rain Gauge v1.2
// By: Kyle Nucera
// kjnucera@syr.edu

// Includes the Necessary Libraries 
#include <WaspXBeeDM.h>
#include <myLibrary.h>
#include <WaspFrame.h>

// Change to false to disable debugging. Vise-Versa is true.
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
  RainGauge.Init();
  // Setting time [yy:mm:dd:dow:hh:mm:ss]
   // Sunday = 01, Monday = 02, ...
    // hours are in a 1-24 format.
  RTC.setTime("15:06:15:04:12:30:00");
  if (debug_Mode){
    USB.println(F("Setting time"));
    USB.println(F("-------------------------------"));
  }
}

void loop() {
  //setRTCfromMeshlium(MAC_ADDRESS);
  // 3.3VDC Power
  //double threeVolt = 3.3;
  //PWR.setSensorPower(SENS_3V3,SENS_ON); 
  //5VDC Power
  double fiveVolt = 5;
  PWR.setSensorPower(SENS_5V,SENS_ON); 
  /////////////////////////////////////////////////////////////  
  // 0. Declarations
  /////////////////////////////////////////////////////////////
  char convertFloat[10];
  char combVal[7] = "000000";
  float temp[5];
  int error = 0;
  /////////////////////////////////////////////////////////////
  // 1. Getting Inputs
  /////////////////////////////////////////////////////////////
  //analog voltage between 0 - 3.3v (MAXBOTIX)
  //RainGauge.read_Analog(convertFloat, fiveVolt);
  delay(500);
  // Read I2C Device
  RainGauge.read_Pressure(combVal, temp, I2C_ADDRESS2);
  /////////////////////////////////////////////////////////////
  // 2. Send to Gateway
  /////////////////////////////////////////////////////////////
  // RainGauge.send_Frame(convertFloat,loc_1, MAC_ADDRESS);
  delay(500);
  RainGauge.send_RG(combVal,loc_1,temp,MAC_ADDRESS);
  delay(500);
  //RainGauge.send_Batt(MAC_ADDRESS, loc_1);
  //delay(500);
  /////////////////////////////////////////////////////////////
  // 3. Write to SD card only if the gateway is unavailable
  /////////////////////////////////////////////////////////////
  RainGauge.write_SD(combVal, loc_1, temp);
  delay(500);
  //frame.showFrame();
  /////////////////////////////////////////////////////////////
  // 4. Sleep For Fifteen Minutes
  /////////////////////////////////////////////////////////////
  // Days:Hours:Minutes:Seconds
  //RainGauge.hibernate();
  //PWR.deepSleep("00:00:00:10",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
  delay(2000);
  xbeeDM.ON();
}


