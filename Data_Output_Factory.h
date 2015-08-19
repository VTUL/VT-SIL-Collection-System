/*
*  Data_Output_Factory.h
*  DAQ Interface
*
*  Created by Chreston Miller on 1/13/15.
*  Copyright 2015 Virginia Tech. All rights reserved.
*
*/

#ifndef __DATAOUTPUTFACTORY_H
#define __DATAOUTPUTFACTORY_H

#include "Data_Output.h"
#include "library.h"

using namespace std;

class Data_Output_Factory{
public:
	Data_Output_Factory();							//Default constructor
	static Data_Output* CreateDataOutput(OutputType type);	//Creates a specific pocket

private:

};

#include "Data_Output_Factory_Implementation.h"

#endif
