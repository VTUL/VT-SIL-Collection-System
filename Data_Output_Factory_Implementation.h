/*
*  Data_Output_Factory.h
*  DAQ Interface
*
*  Created by Chreston Miller on 1/13/15.
*  Copyright 2015 Virginia Tech. All rights reserved.
*
*/

#ifndef PocketFactory_Implementation_H
#define PocketFactory_Implementation_H


#include "Data_Output_Factory.h"
#include "CSV_Output.h"

////////////////////////////////////////////////////////////////////////////////////Data_Output_Factory()
//Default constructor for Data_Output_Factory
//
//Parameters:
//		none
//		
//Returns:				Nothing
//
//Pre:					All the parameters have not been initialized or set to values
//Post:					All the parameters have been initialized
//Called by:			Class Data_Output_Factory
//Calls:				none
//
Data_Output_Factory::Data_Output_Factory(){

}

////////////////////////////////////////////////////////////////////////////////////CreateDataOutput()
//Creates a specified output format for the DAQ sensor data
//
//Parameters:
//		outputType		The ourput format for the DAQ sensor data
//		
//Returns:				A base pointer to the newly created process
//
//Pre:					All the parameters have not been initialized or set to values
//Post:					A process has been created
//Called by:			Class Data_Output_Factory
//Calls:				none
//
Data_Output* Data_Output_Factory::CreateDataOutput(OutputType type){
	Data_Output* BasePtr = NULL;  //base pointer 
    CSV_Output* newCSVOutput = NULL;

	switch (type){
        case CSV:
            newCSVOutput = new CSV_Output();
            BasePtr = &*newCSVOutput;
            break;

        default:
            break;
	}

	return BasePtr; //return the base pointer

}

#endif
