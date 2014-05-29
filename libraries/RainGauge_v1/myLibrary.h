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
class myLibrary
{
  /// public methods and attributes ////////////
public:
    /*
    Class Constructor:
  	Initialize the class attributes
  	- param void
  	- return void
    */ 
    myLibrary();
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
    /*
    This function determines the outside temperature of the presure transducer in celsius.
    - val is the RAW temperature value.
    - val is computed and converted to a string.
    - temp is the final value.
    ####### Need to add selection for Fahrenheit and celsius.
     */   
    char I2Ctemp( int val, char * temp);
    /*
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
    char readAnalog(char * convertFloat);
    /*
    This function reads pressure from an I2C pressure transducer. 
    - FirstVal is the first base 256 byte.
    - SecondVal is the second base 256 byte.
    - ThirdVal is the third byte which represents temperature. 
    - CombVal is the combined string value of firstVal & secondVal. 
    - Temp is string value for the temperature in celsius. 
    - I2C_ADDRESS is the I2C address to read from.
     */   
    char readI2CPressure(char * combVal, char * temp, int I2C_ADDRESS);
    /*
    This function sends a data frame to the Meshlium.  
    - ConvertFloat is the Maxboxtix analog value. 
    - CombVal is the the pressure value.
    - Temp is the temperature value.
    - MAC_ADDRESS is the Mac Address of the desired gateway. 
     */   
    int send2mesh(char* convertFloat, char* combVal, char* temp, char* MAC_ADDRESS);
    /*
    This function writes a data frame to the SD card.  
    - ConvertFloat is the Maxboxtix analog value. 
    - CombVal is the the pressure value.
    - Temp is the temperature value.
     */   
    void sdWrite(char* convertFloat, char* combVal, char* temp);

    char totalpressure2string(int firstVal, int secondVal, char * combVal);
};
extern myLibrary	myObject;

#endif