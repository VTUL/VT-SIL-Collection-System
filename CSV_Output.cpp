#include <map>
#include <iostream>
#include <ctime>
#include <VTEXDsa.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <time.h>

#include "library.h"
#include "CSV_Output.h"



#include "C:\Program Files (x86)\IVI Foundation\IVI\Include\VTEXDsa.h"
//to provide support for safearrays
#import "IviDriverTypeLib.dll" no_namespace
#if _WIN64
#import "VTEXDsa_64.dll" no_namespace
#else
#import "VTEXDsa.dll" no_namespace
#endif

using namespace std;

CSV_Output::CSV_Output()
{
    outputCount = 0; //CM 3/18/15
    lastWriteTime = int(currentTimeSeconds()); //CM 4/14/15 - record the current time for the time of the first file to be written

    string dateTime = currentDateTime(); //CM 8/10/15 - change to use std string, CM 4/16/15 - string format of date and time


    currentFileName = dateTime+"_output.txt"; //CM 4/14/15 - create the first output file based on the current time

    moveDir = ""; //initialize move path so if one is not set, created files will not be moved

    headerWritten = false; //CM 5/19/15
    initializeFileName = false; //CM 5/27/15

    fileLength = 60; //CM 4/14/15 - this is in seconds, start creating a new file every 1 minute (1 minute = 60 seconds)
    numPoints = 0;

    outputStdStr = "";
    headerStdStr = "";

    recordsCollected = 0;
}

//deconstructor
CSV_Output::~CSV_Output()
    //nothing to do at this time
{
}

//get the move directory
std::string CSV_Output::getMoveDir(){
    return moveDir;
}

//set the move directory
void CSV_Output::setMoveDir(std::string mvDir){
    moveDir = mvDir;
}

       //get the file length (in seconds)
int CSV_Output::getfileLength(){
    return fileLength;
}

 //set the file length (in seconds)
void CSV_Output::setfileLength(int fl){
    fileLength = fl;
}



//Write sensor data to file
//CM 8/20/15 - use Sensor_Data class to store data and unpack here for writing to file
/*Parameters:
 *  sensorData:         Data returned from the DAQ. This is a vector of Sensor_Data object pointers.
 *  recordSize:         The number of samples in each record
 *  sampleRate:         The number of samples per second
 *  channelIDs:         Vector of IDs for each channel, format: 2!CH7 --> card in slot 2, channel 7 on that card
 *  numPhysicalChannels The number of physical channels on the DAQ
 *  activeChannels:     List of channels that have sensors connected to them. First parameter is the index of the channel in the list of channels on the DAQ.
 *                          The second parameter is the channel ID
 *  daqID                The unique identifier for the daq that the passed in sensor data originated - NOTE: I am not sure this is still used.
 */
void CSV_Output::writeDaqDataToFile(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, list< pair<int,std::string> > activeChannels, std::string daqID){
    std::string fileName = "";


    int currentWriteTime = int(currentTimeSeconds()); //CM 4/14/15 - get the current (now) time for comparison to the last write time

   std::vector<Sensor_Data*>::iterator sensorIt; //iterator through the list of sensor objects representing samples returned from the DAQ

    try{

        //CM 5/27/15 - update current file name to include the DAQ ID when this function is called for the very first time
        //      This is analogous to detecting the beginning of a write to the file so the channel ID header can be written
        //      only once to the file.
        if(initializeFileName == false){
            initializeFileName = true;
        }


        if (DEBUG_MODE == 1){
            std::cout << "Writing data..." << std::endl;
        }

        //initialize loop count variables for debugging. They  need to be declared outside of a check for debug mode is on/off
        //=======================
        int sensorItLoopCount = 0;
        int recordEntryLoopCount = 0;
        int recordSizeLoopCount = 0;
        int channelLoopCount = 0;
        //=======================


        double *currentSensorDataDouble = NULL; //CM 8/10/15 - move definition outside of sensor loop

        //Iterate through each sensor object and pull out the data
        for(sensorIt = sensorData->begin(); sensorIt != sensorData->end();sensorIt++){

            //keep track of the number of sensor loops
            if(DEBUG_MODE == 1){
                sensorItLoopCount++;
            }

            Sensor_Data* currentSensorData = NULL;
            currentSensorData = *sensorIt; //grab the current sensor data object

            //===============================================================
            //CM 8/12/15 - record the timestamp of the first record
            //If at the first sensor data entry then grab the timestamp
            if(sensorIt == sensorData->begin()){

                //CM 5/27/15 - since the timestamp is the same for each channel for each record (and only processing one record at a time),
                //      Then just grab the timestamp of the first entry, i.e., index = 0
                int timeIndex = 0;
                ::SafeArrayGetElement(currentSensorData->getTimeSeconds(), (LONG *)&timeIndex, &firstTimeSecond);
                ::SafeArrayGetElement(currentSensorData->getTimeFraction(), (LONG *)&timeIndex, &firstTimeFraction);
            }
            //===============================================================


            //CM 8/7/15 - update to use safe array access ===========================================================
            currentSensorDataDouble = NULL;
            HRESULT hr;

            // Get a pointer to the elements of the array.
            hr = ::SafeArrayAccessData(currentSensorData->getData(), (void**)&currentSensorDataDouble);

            if (FAILED(hr)){
                cout<<"Error geting access to SAFEARRAY of sensor data. Aborting..."<<endl;
                return;
            }
            //=======================================================================================================

            //get the number of samples in the sample data
            double ts = 0.0;//CM 5/26/15 - store the time second timestamp for the sample
            double tf = 0.0; //CM 5/26/15 - store the time fraction timestamp for the sample

            //CM 5/27/15 - since the timestamp is the same for each channel for each record (and only processing one record at a time),
            //      Then just grab the timestamp of the first entry, i.e., index = 0
            //CM 8/20/15 - I do not think I need getting the timestamp here, but leave here for now just in case
            //int timeIndex = 0;
            //::SafeArrayGetElement(currentSensorData->getTimeSeconds(), (LONG *)&timeIndex, &ts);
            //::SafeArrayGetElement(currentSensorData->getTimeFraction(), (LONG *)&timeIndex, &tf);


            double d = 0.0; //hold each sample as the sensor data is iterated, once saved, this variable is
                            //  set to the next sample from the sensor

            //Now to iterate through the sensor data. This is completed by "jumping" through the linear stream of data
            //  provided by the DAQ for each channel for each record. Even though I am not accessing it in linear fashion,
            //  I only see one sample once, which leads to linear performance.
            //  Format of linear stream:
            //      For record of size 4 and 3 channels:
            //              ------------------------------ record 1 ------------------------------ ----- record 2----------
            //              [[channel 1 ; 4 samples][channel 2 ; 4 samples][channel 3 ; 4 samples][channel 1 ; 4 samples]...]

           //iterate over all the available records
            for (int record = 0; record < currentSensorData->getNumRecords(); record++)
            {
                recordsCollected++; //CM 8/6/15 - record each record written out

                if(DEBUG_MODE == 1){
                    recordEntryLoopCount++;
                }

                //iterate through each record, i.e., recordSize is the number of samples in a record,
                //  so if the record size is 20, then 20 samples are read for each channel for that specific record
                for (size_t i = 0; i < recordSize; i++)
                {
                    if(DEBUG_MODE == 1){
                        recordSizeLoopCount++;
                    }

                     //Only pull daata from channels with sensors
                     if(ALL_CHANNEL_DATA == 0){
                        int channel = 0; //specify which channel data to get
                        //iterate through all the active channels

                        //=================================================================================================================================
                        //CM 8/7/15 - since changing to using std::list and std::pair, I need to iterate through the list through using an iterator
                        std::list< std::pair<int,std::string> >::iterator it;
                        //=================================================================================================================================

                        //for(int specificChannel = 0; specificChannel < activeChannels.size();specificChannel++){
                        for(it = activeChannels.begin(); it != activeChannels.end();it++){
                            //channel = activeChannels.at(specificChannel).first;
                            channel = (*it).first;

                            if(DEBUG_MODE == 1){
                                channelLoopCount++;
                            }

                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
                            if(headerWritten == false){
                                std::string currentChannelID = channelIDs->at(channel);
                                //if the last channel, leave off the comma
                                //CM 8/7/15 - if the last channel, then leave off the comma - check the next iterator to see if at the end
                                if(std::next(it,1) == activeChannels.end()){
                                    std::string temp = "Data_"+currentChannelID+'\n'; //CM 8/7/15 - remove the timestamp from the header
                                    headerStdStr.append(temp);
                                    temp.clear();


                                    headerWritten = true; //header is written, do not write one again until the next CSV file
                                }
                                //otherwise, put the comma on since more channels are to come
                                else{
                                    headerStdStr.append("Data_"+currentChannelID+','); //CM 8/7/15 - remove the timestamp from the header
                                }
                            }

                            //index calculation: (the current channel * the size of a record) + the current index in the record
                            size_t index = (channel*recordSize)+i;

                            d = (double)currentSensorDataDouble[index]; //CM 8/7/15 - update to use safe access

                            //add current sample to string to be written to file
                            //CM 5/19/15 - if at the last channel, do not add a comma
                            //CM 8/7/15 - if the last channel, then leave off the comma - check the next iterator to see if at the end
                            if(std::next(it,1) == activeChannels.end()){
                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
                                if(DETAILED_OUPUT == 1){
                                    numPoints++;
                                }
                            }
                            //Otherwise, still more channels to go, so add a comma
                            else{
                                outputStdStr.append(std::to_string(d)+','); //CM 7/10/15 - using std::string
                                if(DETAILED_OUPUT == 1){
                                    numPoints++;
                                }
                            }
                        }
                     }
                     //otherwise, pull samples from all channels, despite if they have sensors connected to them
                     else{
                        //iterate through all channels (active and inactive) and pull the data
                         for(int channel = 0; channel < numPhysicalChannels; channel++)
                        {
                            if(DEBUG_MODE == 1){
                                channelLoopCount++;
                            }

                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
                            if(headerWritten == false){
                                //if the last channel, leave off the comma
                                std::string currentChannelID = channelIDs->at(channel); //CM 7/10/15 - using std::string
                                if(channel == numPhysicalChannels-1){
                                    headerStdStr.append("Data_"+currentChannelID+'\n'); //CM 8/7/15 - remove the timestamp from the header
                                    headerWritten = true; //header is written, do not write one again until the next CSV file
                                }
                                //otherwise, put the comma on since more channels are to come
                                else{
                                    headerStdStr.append("Data_"+currentChannelID+','); //CM 8/7/15 - remove the timestamp from the header
                                }
                            }
                            //================================================================================================================
                            //index calculation: (the current channel * the size of a record) + the current index in the record
                            size_t index = (channel*recordSize)+i;//+(record*data_count);

                            d = (double)currentSensorDataDouble[index]; //CM 8/7/15 - update to use safe access

                            //CM 5/19/15 - if at the last channel, do not add a comma
                            if(channel == numPhysicalChannels-1){

                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
                                if(DETAILED_OUPUT == 1){
                                    numPoints++;
                                }
                            }
                            //Otherwise, still more channels to go, so add a comma
                            else{
                                outputStdStr.append(std::to_string(d)+','); //CM 7/10/15 - using std::string
                                if(DETAILED_OUPUT == 1){
                                    numPoints++;
                                }
                            }
                        }
                     }


                    outputStdStr.append("\n"); //signal end of line once an entry in a record for all channels is recorded
                }

            }
            ::SafeArrayUnaccessData(currentSensorData->getData()); //CM 8/10/15 - close access to current safe array
            //===========================================================================================================


            //debugging - check to see how many times the loops above are actually being called
            if(DEBUG_MODE == 1){
                 std::cout<<"Safe loop count: "<<sensorItLoopCount<<" Record Entry Loop Count: "<<recordEntryLoopCount<< " Record Size Loop Count: "<<recordSizeLoopCount<<" Channel Loop Count: "<<channelLoopCount<<std::endl;
                 recordEntryLoopCount = 0; //reset
                 channelLoopCount = 0;
                 recordSizeLoopCount = 0;
            }

        }

        outputCount++; //CM 3/18/15 - increment for next write to file
    }
    catch (...){
       //for now, just output an error message indicating where the error occured
       std::cout<<"Exception caught: CSV_Output class, wrtieDaqDataToFile."<<std::endl;
    }

    channelIDs = NULL; //CM 7/24/15 - cut association with pointer to channel IDs
    sensorData = NULL; //CM 7/23/15 - cut association with pointer to sensor data

    //========================================================================================
    // CM 8/18/15 - writing to file after iterating over data so writing can occur after parsing data since I changed my logic to collect all
    //  the records for one file in one collection sequence in DAQ.cpp
    //  There "should" not be a problem with this since I collect the same number of records in one collection sequence
    //  (before coming to this function) as the fileLength, hence recordsCollected "should" always equal fileLength
    //  when control reaches here.

       recordsCollected = 0; //reset

       //CM 8/12/15 - converting timestamp from DAQs to current date and time =============
       time_t t = firstTimeSecond;

       // convert now to string form
       char* dt = ctime(&t);
       struct tm * timeinfo;
       timeinfo = localtime(&t);

       //CM 8/20/15 - keep here so testing date/time can be done
       //debugging =================
       //cout << "The local date and time is: " << dt << endl;
       //===========================

       char buf[80];

       // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
       // for more information about date/time format
       strftime(buf, sizeof(buf), "%Y-%m-%d_%H_%M_%S", timeinfo); //format timestamp of first data point in the file to a date/time

       //CM 8/20/15 - keep here so testing date/time can be done
       //debugging =================
       //cout<<buf<<endl;
       //==========================

       //==================================================================================

       currentFileName = string(buf);       //create a string out of the formated timstamp
       currentFileName = currentFileName + ".csv"; //add file extention
       std::ofstream Out(currentFileName, std::ios::app); //CM 8/12/15 - Now open a file with the name being the timestampe of the first data point

       //====================================================================================================================
       //CM 8/12/15 - add header to store information necessary to calculate the exact time of each sample in the file

       //NOTE: use to_string() to retain the accuracy of the timestamp info, i.e., all significant figures are retained

       //header for file info: Time info for first sample in the file, sample rate, and length of the file in seconds
       Out << "Time Integer, Time Fraction, Sample Rate (samples per second), Length of file in seconds\n";

       //output the file info
       Out <<to_string(firstTimeSecond)<<','<<to_string(firstTimeFraction)<<','<<sampleRate<<','<<fileLength<<'\n';

       //header to describe what the data in the file is
       Out <<"Sensor Data organized by CSV columns. Each column represents the data from one channel on a specific DAQ\n";

       //====================================================================================================================


       Out << headerStdStr;
       Out << outputStdStr; //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)

       headerStdStr.clear();
       outputStdStr.clear();
       Out.flush(); //CM 7/23/15
       Out.close();

       //CM 4/16/15 - move completed file to another directory
       std::string moveCommand = "move " + currentFileName + " " + moveDir;

       system(moveCommand.c_str()); //move recent finished file

       //perform time benchmark testing for processing each file
       double timeForCollection = 0.0;
       timeForCollection = endTimer();
       std::cout<<"-------------------------------------------------"<<std::endl;
       std::cout<<"Collection/Write cycle time lapse: "<<timeForCollection<<std::endl;
       std::cout<<"-------------------------------------------------"<<std::endl;

       currentFileName = "daq_data_"+std::to_string(currentWriteTime)+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
       headerWritten = false; //CM 5/19/15 - with the new file, signal the header needs to be written

       //only print info when wanting more details about what the collection system is doint
       if(DETAILED_OUPUT == 1){
           std::cout<<"Number of samples written: "<<numPoints<<" over "<<fileLength<<" seconds"<<std::endl
                    <<'\t'<<"Number of samples written = sample rate * time slice for collecting (in seconds) * number of channels ==>"<<std::endl
                    <<"\t \t"<<numPoints<<" = "<<sampleRate<<" * "<<fileLength<<" * "<<numPhysicalChannels<<std::endl
                    <<std::endl;
       }
       numPoints = 0;
}

