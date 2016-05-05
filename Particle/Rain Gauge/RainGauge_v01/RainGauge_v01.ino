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
// Comment this out when not testing
//#define DEBUG
// Variables:
std::vector<String> Output;
FuelGauge fuel;
// Address of I2C Pressure Transducer
int I2C_ADDRESS1 = 40; // Address 0x28
int I2C_ADDRESS2 = 72; // Address 0x48
int I2C_ADDRESS3 = 88; // Address 0x58
int I2C_ADDRESS4 = 104; //Address 0x68
long sleepTime = 900; //15 minutes

void setup(){
  Wire.begin();        // join i2c bus (address optional for master)
  #ifdef DEBUG
    Serial.begin(9600);  // start serial for output
  #endif
}

void loop()
{
  char batt[7];
  delay(5000);
  if (Particle.connected() == false) {
    Particle.connect();
    #ifdef DEBUG
      Serial.print("re-Connected");
    //Particle.publish("C","reconnect");
    #endif
  }
  else{
    #ifdef DEBUG
      Serial.print("Connected");
    //Particle.publish("C","connected");
    #endif
  }
  // collect data and publish
  if (collectData(I2C_ADDRESS2)){
    float2string(fuel.getSoC(),batt,3);
    Output.push_back(batt);
    Particle.publish("RG", Output[0] + "," + Output[1] + "," + Output[2]);
    Output.clear();
  }
  //sleep
  delay(500);
  //System.sleep(SLEEP_MODE_DEEP,sleepTime,SLEEP_NETWORK_STANDBY);
  System.sleep(D3, RISING, sleepTime, SLEEP_NETWORK_STANDBY);
  Wire.begin();
  delay(500);
}
bool collectData(int address){
  int val, firstVal, secondVal, thirdVal, Status;
  char combVal[15];
  char temp[10];
  bool ReadValue = false;
  // Read I2C Device
  #ifdef DEBUG
    Serial.println("Attempting to read!");
  #endif
  Wire.requestFrom(address, 3);
   while(Wire.available()){    // slave may send less than requested
    for (int i = 0; i < 3 ; i++){
      val = Wire.read(); //reads the value from address 40 0x28
      if ( i == 0 ){// makes sure we only get the status bits for the first byte
        Status = val >> 14; // shifts the value over by 14 bits so we can get the status bits
        if (Status != 0){ // checks the the first two status bits to make sure there are no issues.
          Serial.println(F("Error! Data not valid"));
        }
        else{
          firstVal = val; // stores the first byte
           #ifdef DEBUG
            Serial.print(F("FirstVal: "));
            Serial.println(firstVal);
           #endif
        }
      }
      else if (i == 1){
        secondVal = val; // stores the second byte
        #ifdef DEBUG
            Serial.print(F("SecondVal: "));
            Serial.println(secondVal);
        #endif
      }
      else if ( i == 2){
        thirdVal = val; // stores the third byte
        #ifdef DEBUG
         Serial.print(F("temperature: "));
         Serial.println(thirdVal);
         ReadValue = true;
       #endif
      }
    }
   }
   if (ReadValue == true){
    I2Ctemp(thirdVal, temp); //gets the temperature in fahrenheit
    pressure2string(firstVal, secondVal, combVal); //gets RAW pressure value
    Output.push_back(combVal);
    Output.push_back(temp);
    #ifdef DEBUG
      Serial.print(F("combVal: "));
      Serial.println(combVal);
    #endif
    return true;
  }
  else
    return false;
}

char I2Ctemp( int val, char * temp){
  float cTemp, fTemp;
   val = val << 3; // shift the temperature to the left by 3 bits
  // Determines the Temperature in celsius
   cTemp = ((((float)val/2047)*200)-50);
   #ifdef DEBUG
    Serial.print(F("Temperature in Celsius: "));
    Serial.println(cTemp);
    #endif
   fTemp = ((cTemp*9)/5) + 32;
   #ifdef DEBUG
    Serial.print(F("Temperature in Fahrenheit: "));
    Serial.println(fTemp);
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
