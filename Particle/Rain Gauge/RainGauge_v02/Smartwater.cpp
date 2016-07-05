/////////////////////////////////////////////////////////////////////////////
// Smartwater.cpp - Functions for Electron/Photon                          //
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Sublime Text 2 / G++ 4.2.1                     //
// Platform:    2010 MacbookPro, Core i5, OSX 10.9.5                       //
// Application: Smart Water Infastructure - Dave Chandler                  //
// Author:      Kyle Nucera, 462 Link+, Syracuse University                //
//              (908) 906-9311, kjnucera@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
#include "Smartwater.h"
//============================================
// Function: Smartwater()
// Purpose: Class Constructor
// Preconditions: A true/false value for debugging
//============================================
Smartwater::Smartwater(bool val) {
		debug = val;
}
//============================================
// Function: println()
// Purpose: prints a message to the serial monitor
// Preconditions: a message to print
//============================================
void Smartwater::printLine(char const* message) {
	if (debug != false)
		Serial.println(message);
}
void Smartwater::printLine(int message) {
	if (debug != false)
		Serial.println(message);
}
//============================================
// Function: RainGauge()
// Purpose: To measure the differential pressure
//       of an I2C pressure transducer.
// Preconditions: An address to read from
// Ouputs: Sets Output or Error vector Strings
//============================================
void Smartwater::rainGauge(int address) {
	int val, firstVal, secondVal, thirdVal, Status;
	char combVal[15];
	char temp[10];
	bool ReadValue = false;
	// Read I2C Device
	printLine("Attempting to read!");
	Wire.requestFrom(address, 3);  // read 3 bytes
	while (Wire.available()) {  // slave may send less than requested
		for (int i = 0; i < 3 ; i++) {
			val = Wire.read(); // reads the value from address 40 0x28
			if ( i == 0 ) { // makes sure we only get the status bits for the first byte
				Status = val >> 14; // shifts the value over by 14 bits so we can get the status bits
				if (Status != 0) { // checks the the first two status bits to make sure there are no issues.
					printLine("Error! Data Not Valid!");
					Errors.push_back("Error 01: Data Not Valid");
					return;
				}
				else {
					firstVal = val; // stores the first byte
					printLine("FirstVal: ");
					printLine(firstVal);
				}
			}
			else if (i == 1) {
				secondVal = val; // stores the second byte
				printLine("SecondVal: ");
				printLine(secondVal);
			}
			else if ( i == 2) {
				thirdVal = val; // stores the third byte
				printLine("Temperature: ");
				printLine(thirdVal);
			}
		}
		I2Ctemp(thirdVal, temp); //gets the temperature in fahrenheit
		press2string(firstVal, secondVal, combVal); //gets RAW pressure value
		Output.push_back(combVal);
		Output.push_back(temp);
		printLine("combVal: ");
		printLine(combVal);
		return;
	}
	printLine("Error! Sensor Not Available!");
	Errors.push_back("Error 02: Sensor Not Available");
}

void Smartwater::press2string( int firstVal, int secondVal, char * combVal) {
	// Converts 2 int bytes to char * and then concatenates them
	char nfv[10];
	char nsv[10];
	sprintf((char*)nfv, "%i", firstVal);
	sprintf((char*)nsv, "%i", secondVal);
	strcpy(combVal, nfv);
	strcat(combVal, nsv);
}
void Smartwater::I2Ctemp(int val, char * temp) {
	// shift the temperature to the left by 3 bits
	val = val << 3;
	printLine("Temperature: ");
	printLine(val);
	//converts int to string
	float2string(val, temp, 6);
}
void Smartwater::float2string(float f, char* c, uint8_t prec) {
	// float value, where you want to store it, how many decimal places you want
	int fix = 1;
	int p = f;
	f -= p;
	if ( f < 0.1 ) {
		fix = 0;
	}
	while (prec > 0) {
		f *= 10;
		prec--;
	}
	int q = f;
	if ( fix == 0) {
		sprintf(c, "%i.%i%i", p, fix, q);
	}
	else
		sprintf(c, "%i.%i", p, q);
}
std::vector<String> Smartwater::getErrors() {
	std::vector<String> temp;
	temp = Errors;
	Errors.clear();
	return temp;
}
std::vector<String> Smartwater::getOutput() {
	std::vector<String> temp;
	temp = Output;
	Output.clear();
	return temp;
}