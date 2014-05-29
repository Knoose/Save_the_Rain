// Waspmote Pro V1.2 
// Save The Rain Project:
// Rain Gauge v1.1
// By: Kyle Nucera
// kjnucera@syr.edu

// Includes the Necessary Libraries 
#include <WaspXBeeDM.h>
#include <myLibrary.h>
#include <WaspFrame.h>

// comment out this line to remove the serial prints
//#define DEBUG 

// Gateway Mac Addresses ( Wireless Communication )
char* MAC_ADDRESS="0013A20040794BAD"; // new meshlium
//char* MAC_ADDRESS="0013A20040788EE2"; // kyle's usb gateway
//char* MAC_ADDRESS="0013A20040676BE0"; // Nick's USB gateway

// Address of I2C Pressure Transducer
int I2C_ADDRESS1 = 40; // Address 0x28
int I2C_ADDRESS2 = 72; // Address 0x48
int I2C_ADDRESS3 = 88; // Address 0x58
int I2C_ADDRESS4 = 104; //Address 0x68

//Global Names for the values we're sending
char* pres_0 = "RG"; // Rain Gauge Pressure
char* pres_1 = "P1"; //
char* pres_2 = "P2"; // 
char* pres_3 = "P3"; // 
char* tempurature = "Temp"; // Rain Guage Temperature reading
char* Dist = "Dist_Sens";

void setup() {
  myObject.Init();
  // Setting time [yy:mm:dd:dow:hh:mm:ss]
  RTC.setTime("14:05:29:05:12:11:00");
  #ifdef DEBUG
    USB.print(F("Setting time"));
  #endif
}

void loop() {
  // 3.3VDC Power
  PWR.setSensorPower(SENS_3V3,SENS_ON); 
  //5VDC Power
  //PWR.setSensorPower(SENS_5V,SENS_ON); 
  delay(1000);
  /////////////////////////////////////////////////////////////  
  // 0. Declarations
  /////////////////////////////////////////////////////////////
  char convertFloat[10];
  char combVal[7];
  char temp[5];
  int error = 0;
  /////////////////////////////////////////////////////////////
  // 1. Getting Inputs
  /////////////////////////////////////////////////////////////
  //analog voltage between 0 - 3.3v (MAXBOTIX)
  myObject.read_Analog(convertFloat);
  // Read I2C Device
  myObject.read_Pressure(combVal, temp, I2C_ADDRESS2);
  /////////////////////////////////////////////////////////////
  // 2. Send to Gateway
  /////////////////////////////////////////////////////////////
  // frame.setFrameSize(125);
  // myObject.send2mesh(convertFloat,combVal, temp, MAC_ADDRESS);
  error = myObject.send_Frame(combVal,pres_0, MAC_ADDRESS);
  if (error != 1)
    USB.println("Packet did not send");
  delay(500);
  error = myObject.send_Frame(temp,tempurature, MAC_ADDRESS);
  if (error != 1)
    USB.println("Packet did not send");
  error = myObject.send_Batt(MAC_ADDRESS);
  if (error != 1)
    USB.println("Packet did not send");
  /////////////////////////////////////////////////////////////
  // 3. Write to SD card only if the gateway is unavailable
  /////////////////////////////////////////////////////////////
  myObject.write_SD(convertFloat,combVal, temp);
  /////////////////////////////////////////////////////////////
  // 4. Sleep For Fifteen Minutes
  /////////////////////////////////////////////////////////////
  delay(5000);
  //PWR.deepSleep("00:00:15:00",RTC_OFFSET,RTC_ALM1_MODE1,SENS_OFF);
}


