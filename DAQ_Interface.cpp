#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <sstream>
#include <VTEXDsa.h>
#include "CSV_Output.h"


#import "IviDriverTypeLib.dll" no_namespace
#if _WIN64
	#import "VTEXDsa_64.dll" no_namespace
#else
	#import "VTEXDsa.dll" no_namespace
#endif

#include "DAQ_Interface.h"


//constructor
DAQ_Interface::DAQ_Interface(){
	::CoInitialize(NULL);

	try{
        //nothing right now, it was all moved to DAQ class
	}
	catch (...)
	{
		//Do something to intelligently deal with any other non-VTEX errors.

        //for now, just output an error message indicating where the error occured
        std::cout<<"Exception caught: DAQ Interface class, constructor."<<std::endl;
	}
}

//destructor
DAQ_Interface::~DAQ_Interface(){

    try{

    cout<<"Closing collection..."<<endl;

    //CM 6/4/15 - iterate through all the DAQ objects and free the memory

    close(); //CM 8/13/15 - close out the session for each DAQ - this releases any memory held by the DAQ(s)

    }
    catch(...){
        //for now, just output an error message indicating where the error occured
        std::cout<<"Exception caught: DAQ Interface class, deconstructor."<<std::endl;
    }
}

//take in all the newtwork IDs of DAQs, initialize them all, then start collecting
void DAQ_Interface::daqBootStrap(std::vector<std::string> ipAddresses){

	initialize(ipAddresses); //initialize all the DAQs

	//now start collecting
	collectDaqData(); //tell all daqs to collect data


}


//iterate through all the DAQ addresses (daq IDs which are hostnames, e.g., daq1.local)
//  and initialize them
int DAQ_Interface::initialize(std::vector<std::string> ipAddresses){

	std::vector<std::string>::iterator it;

	//since the first entry in the ip addresses is the executable name, skip it
	it = ipAddresses.begin();
	it++; //skip over executable name

	//go through each netowrk address in argv (number is argc) and create a DAQ object and call initialize for each
	//	Start at 1 since the first entry of argv is the name of the executable
	for (; it != ipAddresses.end(); it++){
        DAQ* temp = new DAQ(DEFAULT_BUFFER_SIZE); //create daq object with the default buffer size , i.e., number of records to grab before flushing for next collection sequence
		std::string networkID = *it; //grab network ID

		temp->initialize(networkID); //initialize with the given network ID (IP address)
		daqObjects.insert(std::pair<std::string, DAQ*>(networkID, temp)); //insert into map
		temp = NULL; //remove temp pointer to daq object

	}
	return 0;
}

//call data collection for all the DAQs
void DAQ_Interface::collectDaqData(){

	std::map<std::string, DAQ*>::iterator it;

    bool collectData = true;

    //CM 3/18/15 - adding wile loop for continuous data collection,
    //  collect until the buffer is full, then write to file and repeat
    while (collectData == true){

        startTimer(); //start timer for testing how long collection takes

        //iterate through all the daqs and tell each to collect data
        for (it = daqObjects.begin(); it != daqObjects.end(); it++){

            collectData = it->second->continuousRawData(); //CM 4/13/15 - testing writing all data unprocessed to file (i.e., not broken up into channels)

            //CM 8/18/15 - once collected data from the DAQ(s), write out to file =======================
            it->second->writeDAQDataToFile(); //CM 5/27/15 - change to each DAQ object using their own output object (this allows each
                                              //    DAQ to have a file open for writing its data)
            it->second->clearBuffer(); //CM 3/18/15 - clear the buffer for this channel after writing to file (or wherever the data is being sent)
            //===========================================================================================

            //CM 8/13/15 - if need to stop collecting data, then break from the for loop
            if(collectData == false){
                break;
            }

        }

    }

    //CM 8/13/15 - Collection is complete, now exit gracefully ===================
    //  When the above loop exits, then collection is over.
    delete this; //call the deconstructor
    //============================================================================

}


//Iterate through each DAQ and write the channel info from each to disk
void DAQ_Interface::writeDaqDataToFile(){

	std::map<std::string, DAQ*>::iterator it;

	//iterate through all the daqs and tell each to write data out to files
	for (it = daqObjects.begin(); it != daqObjects.end(); it++){

        it->second->writeDAQDataToFile(); //CM 5/27/15 - change to each DAQ object using their own output object (this allows each
                                          //    DAQ to have a file open for writing its data)
        it->second->clearBuffer(); //CM 3/18/15 - clear the buffer for this channel after writing to file (or wherever the data is being sent)
	}

}


int DAQ_Interface::close(){

	std::map<std::string, DAQ*>::iterator it;


    //iterate through all the daqs and tell each to close the session
	for (it = daqObjects.begin(); it != daqObjects.end(); it++){
		it->second->close();
	}

	return 0;
}
