/////////////////////////////////////////////////////////////////////////////
// RainGauge_v01 - Code for a Rain Gauge                                   //
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++/Wiring , Sublime Text 2 / G++ 4.2.1             //
// Platform:    2010 MacbookPro, Core i5, OSX 10.9.5                       //
// Application: Rain Gauge Code                                            //
// Author:      Kyle Nucera, 462 Link+, Syracuse University                //
//              (908) 906-9311, kjnucera@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
#include <vector>
#include "Smartwater.h"
// Variables:
std::vector<String> Output;
std::vector<String> Errors;
FuelGauge fuel;
// Address of I2C Pressure Transducer
int I2C_ADDRESS1 = 40; // Address 0x28
int I2C_ADDRESS2 = 72; // Address 0x48
int I2C_ADDRESS3 = 88; // Address 0x58
int I2C_ADDRESS4 = 104; //Address 0x68
long sleepTime = 900; //15 minutes

SYSTEM_MODE(SEMI_AUTOMATIC); // for photon right now

bool setDebug = false;

void setup(){
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  //WiFi.off();
  Smartwater SWI(setDebug);
  char batt[7];
  delay(5000);
  /*if (Particle.connected() == false) {
    Particle.connect();
    #ifdef DEBUG
      Serial.print("re-Connected");
    //Particle.publish("C","reconnect");
    #endif*/
  /*}
  else{
    #ifdef DEBUG
      Serial.print("Connected");
    //Particle.publish("C","connected");
    #endif
  }*/
  // collect data and publish
  Serial.println("TEST");
  SWI.rainGauge(I2C_ADDRESS2);
  Errors = SWI.getErrors();
  if (Errors.empty()){
    Output = SWI.getOutput();
    SWI.float2string(fuel.getSoC(),batt,3);
    Output.push_back(batt);
    //Particle.publish("RG", Output[0] + "," + Output[1] + "," + Output[2]);
    Output.clear();
  }
  //sleep
  delay(500);
  //System.sleep(SLEEP_MODE_DEEP,sleepTime,SLEEP_NETWORK_STANDBY);
  //System.sleep(D3, RISING, sleepTime, SLEEP_NETWORK_STANDBY);
  //Wire.begin();
  delay(500);
}
