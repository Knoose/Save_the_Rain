#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////////////
// Display.h - Save the Rain Library                                     //
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
* Public Interface:
* =================
*
* Required Files:
* ===============
*
* Build Command:
* ==============
* 
*
* Maintenance History:
* ====================
*
*
*/

class Display
{
public:
	Display(){}
	
	void println(bool debug, char* message)
	{
		if (debug)
			USB.println(message);
	}
	void print(bool debug, char* message)
	{
		if (debug)
			USB.print(message);
	}
	void println_Int(bool debug, int message)
	{
		if (debug)
			USB.println(message);
	}
	void print_Int(bool debug, int message)
	{
		if (debug)
			USB.print(message);
	}
		void println_Hex(bool debug, int message)
	{
		if (debug)
			USB.printf("%x\n",message);
	}

private:

};
#endif