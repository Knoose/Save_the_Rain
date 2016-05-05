#ifndef myLibrary_h
#define myLibrary_h
/////////////////////////////////////////////////////////////////////////////
// StRLib.h - Save the Rain Library                                     //
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++                                                 //
// Platform:    Mid 2010 MacbookPro, Core i5, OSX 10.9.5                   //
// Application: Rain Gauge Project                                         //
// Author:      Kyle Nucera, 462 Link+, Syracuse University                //
//              (908) 906-9311, kjnucera@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
*
*
* Public Interface:
* =================
*
*
* Required Files:
* ===============
* Waspmote API
* StRLib1.cpp
* Display.h
*
* Build Command:
* ==============
* 
*
* Maintenance History:
* ====================
*
*

 *****************************************************************************
 * Includes
 ******************************************************************************/

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <WConstants.h>
#include "WaspXBeeCore.h"
#include <WaspFrame.h>
#include <Wire.h>
#include <WaspXBeeDM.h>
#include <inttypes.h>
#include "./Display/Display.h"

/******************************************************************************
 * Definitions & Declarations
 ******************************************************************************/

//#define DEBUG 

/******************************************************************************
 * Class
 ******************************************************************************/

// myLibrary Class
/*
  defines all the variables and functions used 
*/
class Rain_Gauge
{
  /// public methods and attributes ////////////
public:
    /*
    Class Constructor:
    Initialize the class attributes
    - param void
    - return void
    */ 
    Rain_Gauge();
    /*
    It initalizes all the necessary variables and Waspmote functionality.
    - Sets PANID.
    - Activates RTC, SD, XbeeDM, ACC, I2C.
    - Creates a path to write on the SD card.
    */   
    int Init();
    // A boolean value to let a user decide to debug or not. 
    Display dp;
    bool debug; 
    /*
    // This function sets the debugging mode for the main program.
    - value is a true/false value inputted by the user. 
    */
    bool set_Debug(bool value);  
    /*
    If Debug_mode is true, print out the time to the console. 
    */
    void read_Time(); 
    /*
    turns on the external power supply. 5vdc or 3vdc.
    */
    void set_Power(int val);
    /*
    This function takes two base 256 values and combines them into one decimal value. 
    After that, the value is converted into a string to be sent to the meshlium.
    - FirstVal is the first base 256 byte.
    - SecondVal is the second base 256 byte.
    - CombVal is the combined string value of firstVal & secondVal. 
    */   
    char convert_Pressure(int firstVal, int secondVal, float * combVal);
    /* ## NOTE: Add selection for celsius/Fahrenheit
    This function determines the outside temperature of the presure transducer in celsius.
    - val is the RAW temperature value.
    - val is computed and converted to a string.
    - temp is the final value.
    */   
    float convert_Temp( int val, float * temp);
    /* ## NOTE: Add selection for celsius/Fahrenheit
    This function converts floats to strings.
    - float f : a floating point value.
    - char* c : output string.
    - unint8_t prec : Number of decimal places.
    */   
    int float2string(float f, char* c, uint8_t prec);
    /*
    This function converts and combines pressure values in a string.
    - firstVal, secondVal are integers that represent base 256 numbers.
    - Combval is the Combined value of firstVal and secondVal.
    */
    char pressure2string( int firstVal, int secondVal, char * combVal);
    /*
    This function reads an analog value from ANALOG1.
    - The analog value is converted from float to string.
    - convertFLoat is the string output.
    */   
    char read_Analog(char * convertFloat, double volt_level);
    /*
    This function reads pressure from an I2C pressure transducer. 
    This function checks to see if the I2C bus is operating properly 
    and will call average_pressure if it is, otherwise it will call
    reset_I2C(). If check_I2C is true, recursively call read_Pressure().
    - FirstVal is the first base 256 byte.
    - SecondVal is the second base 256 byte.
    - ThirdVal is the third byte which represents temperature. 
    - CombVal is the combined string value of firstVal & secondVal. 
    - Temp is string value for the temperature in celsius. 
    - I2C_ADDRESS is the I2C address to read from.
    */   
    char read_Pressure(float * combVal, float * temp, int I2C_ADDRESS);
        /*
    This function reads pressure from an I2C pressure transducer but averages it.
    */   
    char average_Pressure(float * combVal, float * temp, int I2C_ADDRESS);
    /*
    This function sends the current battery level to a gateway.
    - MAC_ADDRESS is the Mac Address of the desired gateway. 
    */   
    int send_RG(float* value, char* message, float* temp, char* MAC_ADDRESS);
    /*
    This function writes a data frame to the SD card.  
    - ConvertFloat is the Maxboxtix analog value. 
    - CombVal is the the pressure value.
    - Temp is the temperature value.
    */   
    void write_SD(float* value, char* message, float* temp);
    /*
    Prints out the battery level is debug_mode is true and executes RTC.ON()
    */
    void hibernate();
    /*
    Checks to see if the I2C Bus is working properly by checking the value read from 
    an I2C sensor.
    */
    int check_I2C(float* temp, float* pressure);
    /*
    If the I2C bus is not working properly then:
        - Reset the I2C and return 1 
        - if getTime() returns error
            - Call reset_PWR
        - Write an error message to the SD card
    */
    int reset_I2C();
    /*
    Checks to see if we called reset_PWR
    If true:
        Do not set the time.
    Else:
        User updated time and set it as th new time.
    */
    bool set_Time(char* date);
    /*
        Resets the waspmote Device if RTC fails.
    */
    void reset_PWR();
    /*
    Checks the battery level to see if it's dropped below a certain threshold. 
    If it has, deepsleep until the battery level rises about a new threshold.
    */
    void power_SAVE();
 
};
extern Rain_Gauge   RainGauge;

class Hop_Node : public Rain_Gauge
{
    public:
    /*
    Class Constructor:
    Initialize the class attributes
    - param void
    - return void
    */ 
    Hop_Node();

    /*
    This function sends the internal temperature level to a gateway.
    - MAC_ADDRESS is the Mac Address of the desired gateway. 
    */   
    int send_InTemp(char* MAC_ADDRESS, char* message);
};
extern Hop_Node   HopNode;

#endif