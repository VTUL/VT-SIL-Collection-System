#include "library.h"
#include <VTEXDsa.h>
#include "CSV_Output.h"
#include "DAQ.h"
#include <map>


#import "IviDriverTypeLib.dll" no_namespace
#if _WIN64
	#import "VTEXDsa_64.dll" no_namespace
#else
	#import "VTEXDsa.dll" no_namespace
#endif

#ifndef __DAQ_INTERFACE_H
#define __DAQ_INTERFACE_H

//This class controlls the action of the DAQ(s) the system is connected to.
//  This includes:
//      - Initializing them based on the hostnames of the daq(s) provided
//      - controlling the collection/writing cycle of all daq(s)
class DAQ_Interface
{

public:
    DAQ_Interface(); //constructor, takes the max to read in before writing to disk
	~DAQ_Interface(); //desctutor

	void daqBootStrap(std::vector<std::string> ipAddresses); //take in all the newtwork IDs of DAQs, initialize them all, then start collecting

    int initialize(std::string ipAddress); //initialize a single daq

    int initialize(std::vector<std::string> ipAddresses); //initialize a set of daqs

	void collectDaqData(); //call data collection for all the DAQs

    int close(); //close out the session for all daq(s)

    void writeDaqDataToFile(); //write the current stored data in each daq to file

private:

    std::map<std::string, DAQ*> daqObjects; //map to store all the daq objects, value: daq ID string, value: pointer to daq object

};

#endif
