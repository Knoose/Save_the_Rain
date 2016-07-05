#ifndef __SMARTWATER_H__
#define __SMARTWATER_H__
/////////////////////////////////////////////////////////////////////////////
// Smartwater.h - Functions for Electron/Photon                            //
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Sublime Text 2 / G++ 4.2.1                     //
// Platform:    2010 MacbookPro, Core i5, OSX 10.9.5                       //
// Application: Smart Water Infastructure - Dave Chandler                  //
// Author:      Kyle Nucera, 462 Link+, Syracuse University                //
//              (908) 906-9311, kjnucera@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
*
* Module Operations:
* ==================
* 
*
* Public Interface:
* =================
*
* Required Files:
* ===============
* Smartwater.cpp
*
* Build Command:
* ==============
* g++ -o SWI_Test Smartwater.cpp
*
* Maintenance History:
* ====================
* ver 1.1 : 05 Jul 16
* - Added RainGauge function
* - Added println function
* ver 1.0 : 08 Apr 16
* - first release
*
* Sublime Notes:
* AStyleFormatter - Ctrl + alt + f
*/

#include <fstream>
#include <vector>
// For Particle functions
#include "application.h" 
class Smartwater
{
private:
	bool debug = false;
	std::vector<String> Output;
	std::vector<String> Errors;
public:
	Smartwater(bool val);
	void rainGauge(int address);
	void printLine(char const* message);
	void printLine(int message);
	void press2string( int firstVal, int secondVal, char * combVal);
	void I2Ctemp(int val, char * temp);
	void float2string(float f, char* c, uint8_t prec);
	std::vector<String> getErrors();
	std::vector<String> getOutput();
};
#endif