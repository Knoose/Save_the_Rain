/*! \def myLibrary_h
 \ This is a library to use with the save the rain project's rain gauges. 
 This header containts a list of functions that belong to the mylibrary class.

Save The Rain Project:
Waspmote Pro V1.2 
Rain Gauge Library v1.1
By: Kyle Nucera
kjnucera@syr.edu

 */
#ifndef myLibrary_h
#define myLibrary_h

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <WConstants.h>
#include "WaspXBeeCore.h"
#include <WaspFrame.h>
#include <Wire.h>
#include "../XBeeDM/WaspXBeeDM.h"
#include <inttypes.h>

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
    /*
    This function converts and combines pressure values in a string.
    - firstVal, secondVal are integers that represent base 256 numbers.
    - Combval is the Combined value of firstVal and secondVal.
    */   
    char pressure2string( int firstVal, int secondVal, char * combVal);
    /* ## NOTE: Add selection for celsius/Fahrenheit
    This function determines the outside temperature of the presure transducer in celsius.
    - val is the RAW temperature value.
    - val is computed and converted to a string.
    - temp is the final value.
    */   
    char convert_Temp( int val, char * temp);
    /* ## NOTE: Add selection for celsius/Fahrenheit
    This function converts floats to strings.
    - float f : a floating point value.
    - char* c : output string.
    - unint8_t prec : Number of decimal places.
    */   
    int float2string(float f, char* c, uint8_t prec);
    /*
    This function reads an analog value from ANALOG1.
    - The analog value is converted from float to string.
    - convertFLoat is the string output.
    */   
    char read_Analog(char * convertFloat);
    /*
    This function reads pressure from an I2C pressure transducer. 
    - FirstVal is the first base 256 byte.
    - SecondVal is the second base 256 byte.
    - ThirdVal is the third byte which represents temperature. 
    - CombVal is the combined string value of firstVal & secondVal. 
    - Temp is string value for the temperature in celsius. 
    - I2C_ADDRESS is the I2C address to read from.
    */   
    char read_Pressure(char * combVal, char * temp, int I2C_ADDRESS);
    /* ## NOTE: This function currently is not compatible with the meshlium.
    This function sends a data frame to the Meshlium. 
    - ConvertFloat is the Maxboxtix analog value. 
    - CombVal is the the pressure value.
    - Temp is the temperature value.
    - MAC_ADDRESS is the Mac Address of the desired gateway. 
    */   
    int rg_Send(char* convertFloat, char* combVal, char* temp, char* MAC_ADDRESS);
    /*
    This function writes a data frame to the SD card.  
    - ConvertFloat is the Maxboxtix analog value. 
    - CombVal is the the pressure value.
    - Temp is the temperature value.
    */   
    void write_SD(char* convertFloat, char* combVal, char* temp);
    /*
    This function takes two base 256 values and combines them into one decimal value. 
    After that, the value is converted into a string to be sent to the meshlium.
    - FirstVal is the first base 256 byte.
    - SecondVal is the second base 256 byte.
    - CombVal is the combined string value of firstVal & secondVal. 
    */   
    char convert_Pressure(int firstVal, int secondVal, char * combVal);
    /*
    This function sends one string value to a gateway.
    - Message is a way to define what you're sending.
    - Value is the actual string that is being sent.
    - MAC_ADDRESS is the Mac Address of the desired gateway. 
     */   
    int send_Frame(char* value, char* message, char* MAC_ADDRESS);
    /*
    This function sends the current battery level to a gateway.
    - MAC_ADDRESS is the Mac Address of the desired gateway. 
    */   
    int send_Batt(char* MAC_ADDRESS);
    /*
    This function sends the internal temperature level to a gateway.
    - MAC_ADDRESS is the Mac Address of the desired gateway. 
    */   
    int send_InTemp(char* MAC_ADDRESS);
    
};
extern Rain_Gauge	RainGauge;

#endif