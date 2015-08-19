#include "library.h"
#include <VTEXDsa.h>
#include "C:\Program Files (x86)\IVI Foundation\IVI\Include\VTEXDsa.h"
#include "Data_Output.h"
#include "CSV_Output.h"
#include "sensor_data.h"
#include "config_parser.h"

#include <map>
#include <string>
#include <list>
//#include <QStringList>
//#include <QPair>

//to provide support for safearrays
#import "IviDriverTypeLib.dll" no_namespace
#if _WIN64
#import "VTEXDsa_64.dll" no_namespace
#else
#import "VTEXDsa.dll" no_namespace
#endif

#ifndef __DAQ_H
#define __DAQ_H

#pragma once
class DAQ
{
public:
	DAQ(long int buffer = DEFAULT_BUFFER_SIZE);
	~DAQ();

	int initialize(std::string ipAddress); //initialize the driver session, i.e., connect to the DAQ specified by the ip address

    bool continuousRawData();  //CM 4/13/15 - test just gathing the raw, unprocessed linear stream of sensor data,
                                                                           //   e.g., do not seperate which data goes to which sensor, but keep linear raw data
    std::string getDaqID();

    void appendToData(Sensor_Data* sd); //CM 5/18/15 - storing the safearray from the DAQ directly
    bool getBufferFull(); //CM 5/18/15 - see if the buffer is full, True: it is full; False; not full
    double getSampleRate(); //CM 5/21/15 - get the sample rate of the DAQ

    void writeDAQDataToFile(); //write the data in each channel to the specified output object

	int close(); //Close the DAQ session gracefully

    bool clearBuffer(); //CM 3/18/15 - clear the buffer for each channel

private:
    IVTEXDsaPtr driver;     //driver object to interact with the DAQ
    long actual_records;    //the number of actual records return from the DAQ per each call to the DAQ for records
    BSTR channels;          //the channel names (identifiers) corresponding to the data returned
    SAFEARRAY *data;        //the sensor data returned
    SAFEARRAY *ts_sec;      //the seconds of the timestamp of the latest returned sensor data
    SAFEARRAY *ts_frac;     //the fractions of a second for the timestampe (goes with above)
    BSTR addl_data;         //any additional data available

	std::string daqID; //the unique ID of the DAQ, i.e., the network name for the daq
    long numPhysicalChannels; //number of physical channels

    long int bufferSize; //the amount of data that is being store before a write to disk occcurs
    bool bufferFull; //CM 5/11/15 - flag to signal if the buffer is full so the buffer can be saved and the buffer flushed

    DataFormat dataFormat; //CM 4/14/15 - enum to specify the format of the data (after collected from the DAQ)see library.h for full explanation)

    bool parsedChannelNames; //CM 3/19/15 - testing: see if I can only parse and create the channel names/objects once

    //QStringList channelIDs; //CM 5/18/15 - store the channel IDs of the channels in the DAQ
    std::vector<std::string> channelIDs; //CM 7/10/15 - change to using std::string insteam of QString
    int recordSize; //CM 5/18/15 - the number of data points in a record, used to parse the data stream appropriately for each channel's data
    double sampleRate; //CM 5/21/15 - record the sample rate of the DAQ (across the board for all sensors connected to the DAQ)

    //QList< QPair<int,QString> > activeChannels; //CM 5/22/15 - store which channels are active (connected to sensors), this is a list
                                                //  of tuples that represent the channel number and channel ID string. The channel number
                                                //  comes from the linear order of channels returned from the DAQs. E.g., if 2!CH7 has
                                                //  a sensor and it is number 7 of 64 sensors, its channel number is 7.
    //QList< QPair<int,std::string> > activeChannels; //CM 7/10/15 - change to using std::string
    list< pair<int,std::string> > activeChannelsStd; //CM 7/10/15 - change to using no qt data structures

    std::vector<Sensor_Data*> sensorData; //CM 5/26/15 - the sensor data returned from a memory read of the DAQ, each object is the result
                                          //  of a memory read

    int numCollectedPoints; //testing to see how many points the DAQ is giving me
    int numCollectedRecords; //keep track of the actual number of records collected

    CSV_Output* output; //CM 5/27/15 - every DAQ needs its own dedicated output object

    Config_Parser* configParser; //CM 6/1/15 - parser to load parameters for the DAQ
    std::string movePath; //CM 6/2/15 - the local path where data files are moved once completed


};

#endif
