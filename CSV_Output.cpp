//#include <iostream>
//#include <fstream>
#include <map>
//#include <QString>
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

    //QString dateTime = currentDateTime(); //CM 4/16/15 - string formate of date and time

    string dateTime = currentDateTime(); //CM 8/10/15 - change to use std string, CM 4/16/15 - string format of date and time


    currentFileName = dateTime+"_output.txt"; //CM 4/14/15 - create the first output file based on the current time

    //===== testing locations to move finished files to =================================
    //moveDir = "C:\\Users\\chmille3\\Documents\\SIL_Collection_Data";
    //moveDir = "C:\\Users\\itsloaner\\Downloads\\SIL_Collection_Data";
    //moveDir = "S:\\SIL_Collection_Data";
    //===================================================================================

    moveDir = ""; //initialize move path so if one is not set, created files will not be moved

    headerWritten = false; //CM 5/19/15
    initializeFileName = false; //CM 5/27/15

    fileLength = 60; //CM 4/14/15 - this is in seconds, start creating a new file every 1 minute (1 minute = 60 seconds)
    numPoints = 0;

    //testing ================
    outputStdStr = "";
    headerStdStr = "";
    //========================

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
//CM 5/26/15 - use record structure and safe array storage for writing to file
/*Parameters:
 *  sensorData:         Data returned from the DAQ. This is a QList of Sensor_Data object pointers.
 *  recordSize:         The number of samples in each record
 *  sampleRate:         The number of samples per second
 *  channelIDs:         List of IDs for each channel, format: 2!CH7 --> card in slot 2, channel 7 on that card
 *  numPhysicalChannels The number of physical channels on the DAQ
 *  activeChannels:     List of channels that have sensors connected to them. Firs parameter is the index of the channel in the list of channels on the DAQ.
 *                          The second parameter is the channel ID
 *  daqID                The unique identifier for the daq that the passed in sensor data originated.
 */
//void CSV_Output::writeDaqDataToFile(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, QStringList* channelIDs, int numPhysicalChannels, QList< QPair<int,QString> > activeChannels, std::string daqID){
//    std::string fileName = "";
////    QString outputStr = ""; //CM 5/18/15 - the string to be written to the file after pulling data from the records
////    QString headerStr = ""; //CM 5/19/15 - the string for the header of a CSV file that contains the channel IDs

//    int currentWriteTime = int(currentTimeSeconds()); //CM 4/14/15 - get the current (now) time for comparison to the last write time

//   //CM 4/14/15 - if reached the time threshold, then close then create a new file to write to
//   if(currentWriteTime-lastWriteTime >= timeThreshold){
//       lastWriteTime = currentWriteTime;

//       QString dateTime = currentDateTime(); //CM 4/16/15 - testing string formate of date and time

//       //testing=====================
//       std::ofstream Out(currentFileName, std::ios::app);
//       Out << headerStr.toStdString();
//       Out << outputStr.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//       headerStr.clear();
//       outputStr.clear();
//       Out.close();
//       //============================

//       //CM 4/16/15 - move completed file to another directory
//       std::string moveCommand = "move " + currentFileName + " " + moveDir;

//       system(moveCommand.c_str()); //move recent finished file

//       //perform time benchmark testing for processing each file
//       double timeForCollection = 0.0;
//       timeForCollection = endTimer();
//       std::cout<<"-------------------------------------------------"<<std::endl;
//       std::cout<<"Collection/Write cycle time lapse: "<<timeForCollection<<std::endl;
//       std::cout<<"-------------------------------------------------"<<std::endl;

// //      currentFileName = daqID+"_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       currentFileName = "daq_data_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       headerWritten = false; //CM 5/19/15 - with the new file, signal the header needs to be written

//       //only print info when wanting more details about what the collection system is doint
//       if(DETAILED_OUPUT == 1){
//           std::cout<<"Number of samples written: "<<numPoints<<" over "<<timeThreshold<<" seconds"<<std::endl
//                    <<'\t'<<"Number of samples written = sample rate * time slice for collecting (in seconds) * number of channels ==>"<<std::endl
//                    <<"\t \t"<<numPoints<<" = "<<sampleRate<<" * "<<timeThreshold<<" * "<<numPhysicalChannels<<std::endl
//                    <<std::endl;
//       }
//       numPoints = 0;
//   }

//   std::vector<Sensor_Data*>::iterator sensorIt; //iterator through the list of sensor objects representing samples returned from the DAQ

//    try{

//        //CM 5/27/15 - update current file name to include the DAQ ID when this function is called for the very first time
//        //      This is analogous to detecting the beginning of a write to the file so the channel ID header can be written
//        //      only once to the file.
//        if(initializeFileName == false){
//  //          currentFileName = daqID+"_"+currentFileName;
//            initializeFileName = true;
//        }

//        //open file for writing, append to end of file
//        //remove for testing ==========
//        //std::ofstream Out(currentFileName, std::ios::app);
//        //=============================

//        //testing================
//        /*
//        QString outputStr2 = "";
//        QString outputStr3 = "";
//         std::ofstream Out2(currentFileName+"_2", std::ios::app);
//         std::ofstream Out3(currentFileName+"_3", std::ios::app);
//         */
//        //=======================

//        if (DEBUG_MODE == 1){
//            std::cout << "Writing data..." << std::endl;
//        }

//        //initialize loop count variables for debugging. They  need to be declared outside of a check for debug mode is on/off
//        //=======================
//        int sensorItLoopCount = 0;
//        int recordEntryLoopCount = 0;
//        int recordSizeLoopCount = 0;
//        int channelLoopCount = 0;
//        //=======================


//        //Iterate through each sensor object and pull out the data
//        for(sensorIt = sensorData->begin(); sensorIt != sensorData->end();sensorIt++){

//            //keep track of the number of sensor loops
//            if(DEBUG_MODE == 1){
//                sensorItLoopCount++;
//            }



//            //====== testing just writing straight from safearrays without indexing  for specific channels ============
//            /*
//            double dd = 0;
//            Sensor_Data* sensorData = *sensorIt;
//            for(int y = 0; y < sensorData->getData()->rgsabound->cElements; y++){
//                ::SafeArrayGetElement(sensorData->getData(), (LONG *)&y, &dd);
//                outputStr.append(QString::number(dd)+"2345678901v2345678"+'\n');
//                //outputStr2.append(QString::number(dd)+'\n');
//                //outputStr3.append(QString::number(dd)+'\n');
//            }

//            continue;
//            */
//            //========================================================================================================


//            Sensor_Data* currentSensorData = NULL;
//            currentSensorData = *sensorIt; //grab the current sensor data object

//            //get the number of samples in the sample data

//            double d = 0.0; //hold each sample as the sensor data is iterated, once saved, this variable is
//                            //  set to the next sample from the sensor
//            double ts = 0.0;//CM 5/26/15 - store the time second timestamp for the sample
//            double tf = 0.0; //CM 5/26/15 - store the time fraction timestamp for the sample

//            //CM 5/27/15 - since the timestamp is the same for each channel for each record (and only processing one record at a time),
//            //      Then just grab the timestamp of the first entry, i.e., index = 0
//            int timeIndex = 0;
//            ::SafeArrayGetElement(currentSensorData->getTimeSeconds(), (LONG *)&timeIndex, &ts);
//            ::SafeArrayGetElement(currentSensorData->getTimeFraction(), (LONG *)&timeIndex, &tf);

//            //Now to iterate through the sensor data. This is completed by "jumping" through the linear stream of data
//            //  provided by the DAQ for each channel for each record. Even though I am not accessing it in linear fashion,
//            //  I only see one sample once, which leads to linear performance.
//            //  Format of linear stream:
//            //      For record of size 4 and 3 channels:
//            //              ------------------------------ record 1 ------------------------------ ----- record 2----------
//            //              [[channel 1 ; 4 samples][channel 2 ; 4 samples][channel 3 ; 4 samples][channel 1 ; 4 samples]...]

//           //iterate over all the available records
//            for (int record = 0; record < currentSensorData->getNumRecords(); record++)
//            {

//                if(DEBUG_MODE == 1){
//                    recordEntryLoopCount++;
//                }

//                //iterate through each record, i.e., recordSize is the number of samples in a record,
//                //  so if the record size is 20, then 20 samples are read for each channel for that specific record
//                for (size_t i = 0; i < recordSize; i++)
//                {
//                    if(DEBUG_MODE == 1){
//                        recordSizeLoopCount++;
//                    }

//                     //Only pull daata from channels with sensors
//                     if(ALL_CHANNEL_DATA == 0){
//                         int channel = 0; //specify which channel data to get
//                         //iterate through all the active channels
//                        for(int specificChannel = 0; specificChannel < activeChannels.size();specificChannel++){
//                            channel = activeChannels.at(specificChannel).first;

//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                QString currentChannelID = channelIDs->at(channel);
//                                //if the last channel, leave off the comma
//                                if(specificChannel == activeChannels.size()-1){
//                                    headerStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');

//                                    //removed for testing writing entire string once to file ================
//                                    //Out << headerStr.toStdString(); //since done, then write to file
//                                    //=======================================================================

//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
//                                    headerStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                }
//                            }

//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;

//                            ::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
//                                outputStr.append(QString::number(ts)+',');
//                                outputStr.append(QString::number(tf)+',');
//                            }

//                            //add current sample to string to be written to file
//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            if(specificChannel == activeChannels.size()-1){
//                                outputStr.append(QString::number(d));
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{
//                                outputStr.append(QString::number(d)+',');
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }
//                     //otherwise, pull samples from all channels, despite if they have sensors connected to them
//                     else{
//                        //iterate through all channels (active and inactive) and pull the data
//                         for(int channel = 0; channel < numPhysicalChannels; channel++)
//                        {
//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                //if the last channel, leave off the comma
//                                QString currentChannelID = channelIDs->at(channel);
//                                if(channel == numPhysicalChannels-1){
//                                    headerStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
////                                    Out << headerStr.toStdString(); //since done, then write to file
//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
//                                    headerStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                }
//                            }
//                            //================================================================================================================
//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;//+(record*data_count);

//                            ::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
//                                outputStr.append(QString::number(ts)+',');
//                                outputStr.append(QString::number(tf)+',');
//                            }


//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            //if(channel == numChannels-1){
//                            if(channel == numPhysicalChannels-1){

//                                outputStr.append(QString::number(d));
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{

//                                outputStr.append(QString::number(d)+',');
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }


//                    outputStr.append('\n'); //signal end of line once an entry in a record for all channels is recorded
//                }

//            }

//            //===========================================================================================================


//            //debugging - check to see how many times the loops above are actually being called
//            if(DEBUG_MODE == 1){
//                 std::cout<<"Safe loop count: "<<sensorItLoopCount<<" Record Entry Loop Count: "<<recordEntryLoopCount<< " Record Size Loop Count: "<<recordSizeLoopCount<<" Channel Loop Count: "<<channelLoopCount<<std::endl;
//                 recordEntryLoopCount = 0; //reset
//                 channelLoopCount = 0;
//                 recordSizeLoopCount = 0;
//            }

//        }
//        if(DISPLAY_FILE_IO_INFO == 1){
//            int numLines = outputStr.count('\n');
//            std::cout<<numLines<<" lines written to file for "<<sensorData->size()<<" records."<<std::endl;
//        }
////remove for testing ===================
//        /*
//        Out << outputStr.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr.clear();
//        Out.close();
//        */
////======================================

//        //testing=========================
//        /*
//        Out2 << outputStr2.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr2.clear();
//        Out2.close();

//        Out3 << outputStr3.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr3.clear();
//        Out3.close();
//        */
//        //================================

//        outputCount++; //CM 3/18/15 - increment for next write to file
//    }
//    catch (...){
//       //for now, just output an error message indicating where the error occured
//       std::cout<<"Exception caught: CSV_Output class, wrtieDaqDataToFile."<<std::endl;
//    }

//}

//void CSV_Output::writeDaqDataToFile_standard_string(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, QList< QPair<int,std::string> > activeChannels, std::string daqID){
//    std::string fileName = "";
////    QString outputStr = ""; //CM 5/18/15 - the string to be written to the file after pulling data from the records
////    QString headerStr = ""; //CM 5/19/15 - the string for the header of a CSV file that contains the channel IDs


//    //CM 8/6/15 - how many records are to be written to a file based on the sample rate, record, and the desired time length of the file (timeThreshold)
// //   int recordsPerFile = timeThreshold / (double(recordSize)/double(sampleRate));

//    //std::cout<<"Records to be written to file: "<<recordsPerFile<<std::endl;

//    int currentWriteTime = int(currentTimeSeconds()); //CM 4/14/15 - get the current (now) time for comparison to the last write time

//    //CM 4/14/15 - if reached the time threshold, then close then create a new file to write to
//    //if(currentWriteTime-lastWriteTime >= timeThreshold){
//    //if(recordsCollected == recordsPerFile){
//    if(recordsCollected == timeThreshold){ //CM 8/7/15 - assuming 1 record equals 1 second of data
//       //lastWriteTime = currentWriteTime;

//       recordsCollected = 0; //reset

//       //QString dateTime = currentDateTime(); //CM 4/16/15 - testing string formate of date and time

//       //testing=====================
//       //CM 7/15/15 - time writing to file
//       clock_t start;
//       start = std::clock();
//       double duration = 0.0;

//       std::ofstream Out(currentFileName, std::ios::app);

//       //CM 7/17/15 - testing using a vector of doubles and outputing to the file ==================
//       /* This was ok, but took a long time to write to file
//       for(int x = 0; x < samples.size(); x++){
//           Out << samples.at(x) << '\n';
//       }
//       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//       std::cout<<"Time to write file iterating through vector: "<<duration<<std::endl;
//       */
//       //============================================================================================


//       Out << headerStdStr;
//       Out << outputStdStr; //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//       std::cout<<"Time to write file before string clear: "<<duration<<std::endl;
//       headerStdStr.clear();
//       outputStdStr.clear();
//       Out.flush(); //CM 7/23/15
//       Out.close();
//       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//       std::cout<<"Time to write file after string clear: "<<duration<<std::endl;

//       //CM 4/16/15 - move completed file to another directory
//       std::string moveCommand = "move " + currentFileName + " " + moveDir;

//       system(moveCommand.c_str()); //move recent finished file

//       //perform time benchmark testing for processing each file
//       double timeForCollection = 0.0;
//       timeForCollection = endTimer();
//       std::cout<<"-------------------------------------------------"<<std::endl;
//       std::cout<<"Collection/Write cycle time lapse: "<<timeForCollection<<std::endl;
//       std::cout<<"-------------------------------------------------"<<std::endl;

// //      currentFileName = daqID+"_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       //currentFileName = "daq_data_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       currentFileName = "daq_data_"+std::to_string(currentWriteTime)+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       headerWritten = false; //CM 5/19/15 - with the new file, signal the header needs to be written

//       //only print info when wanting more details about what the collection system is doint
//       if(DETAILED_OUPUT == 1){
//           std::cout<<"Number of samples written: "<<numPoints<<" over "<<timeThreshold<<" seconds"<<std::endl
//                    <<'\t'<<"Number of samples written = sample rate * time slice for collecting (in seconds) * number of channels ==>"<<std::endl
//                    <<"\t \t"<<numPoints<<" = "<<sampleRate<<" * "<<timeThreshold<<" * "<<numPhysicalChannels<<std::endl
//                    <<std::endl;
//       }
//       numPoints = 0;
//   }

//   std::vector<Sensor_Data*>::iterator sensorIt; //iterator through the list of sensor objects representing samples returned from the DAQ

//    try{

//        //CM 5/27/15 - update current file name to include the DAQ ID when this function is called for the very first time
//        //      This is analogous to detecting the beginning of a write to the file so the channel ID header can be written
//        //      only once to the file.
//        if(initializeFileName == false){
//  //          currentFileName = daqID+"_"+currentFileName;
//            initializeFileName = true;
//        }

//        //open file for writing, append to end of file
//        //remove for testing ==========
//        //std::ofstream Out(currentFileName, std::ios::app);
//        //=============================

//        //testing================
//        /*
//        QString outputStr2 = "";
//        QString outputStr3 = "";
//         std::ofstream Out2(currentFileName+"_2", std::ios::app);
//         std::ofstream Out3(currentFileName+"_3", std::ios::app);
//         */
//        //=======================

//        if (DEBUG_MODE == 1){
//            std::cout << "Writing data..." << std::endl;
//        }

//        //initialize loop count variables for debugging. They  need to be declared outside of a check for debug mode is on/off
//        //=======================
//        int sensorItLoopCount = 0;
//        int recordEntryLoopCount = 0;
//        int recordSizeLoopCount = 0;
//        int channelLoopCount = 0;
//        //=======================


//        //CM 7/15/15 - testing time required for iterating and storing data into a string ============
//        clock_t start;
//        start = std::clock();
//        double duration = 0.0;
//        double durationSum = 0.0;
//        //============================================================================================

//        //Iterate through each sensor object and pull out the data
//        for(sensorIt = sensorData->begin(); sensorIt != sensorData->end();sensorIt++){

//            //keep track of the number of sensor loops
//            if(DEBUG_MODE == 1){
//                sensorItLoopCount++;
//            }



//            //====== testing just writing straight from safearrays without indexing  for specific channels ============
//            /*
//            //testing getting a range from the safearray =====
//            BSTR* longArray = NULL;

//            //================================================

//            double dd = 0;
//            Sensor_Data* sensorData = *sensorIt;
//            for(int y = 0; y < sensorData->getData()->rgsabound->cElements; y++){
//                ::SafeArrayGetElement(sensorData->getData(), (LONG *)&y, &dd);
//                //outputStr.append(QString::number(dd)+"2345678901v2345678"+'\n');
//                //outputStr2.append(QString::number(dd)+'\n');
//                //outputStr3.append(QString::number(dd)+'\n');



//                std::cout<<"Trying to convert safearray to string"<<std::endl;
//                SafeArrayAccessData(sensorData->getData(),(void**)longArray); //testing getting a range from the safearray
//                std::cout<<"1"<<std::endl;
//                const char *p= const_cast<char *>(_com_util::ConvertBSTRToString(*longArray));
//               //std::cout<<"Long array: "<<*longArray<<std::endl;
//                std::cout<<"2"<<std::endl;
//                std::string dataStr = std::string(p);
//                std::cout<<"3"<<std::endl;
//                std::cout<<"data array size: "<<dataStr.size()<<std::endl;

//                outputStdStr.append(std::to_string(dd));
//            }

//            //duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//            //std::cout<<"Time to iterate over data without indexing: "<<duration<<std::endl;

//            continue;
//            */
//            //========================================================================================================


//            Sensor_Data* currentSensorData = NULL;
//            currentSensorData = *sensorIt; //grab the current sensor data object

//            //CM 8/7/15 - update to use safe array access ===========================================================
//            double *currentSensorDataDouble;
//            //string *currentSensorDataDouble;
//            HRESULT hr;

//            // Get a pointer to the elements of the array.
//            hr = ::SafeArrayAccessData(currentSensorData->getData(), (void**)&currentSensorDataDouble);

//            if (FAILED(hr)){
//                cout<<"Error geting access to SAFEARRAY of sensor data. Aborting..."<<endl;
//                return;
//            }
//            //=======================================================================================================

//            //======================================================================================
//            //CM 7/16/15 - adding support for accessing data from SAFEARRAY using a single lock
//            /*
//            //BSTR HUGEP *currentSensorDataBSTR;
//            double *currentSensorDataDouble;
//            //string *currentSensorDataDouble;
//            HRESULT hr;

//            // Get a pointer to the elements of the array.
//            hr = ::SafeArrayAccessData(currentSensorData->getData(), (void**)&currentSensorDataDouble);

//            if (FAILED(hr)){
//                cout<<"Error geting access to SAFEARRAY of sensor data. Aborting..."<<endl;
//                return;
//            }

//            recordsCollected++;// = currentSensorData->getNumRecords(); //CM 8/7/15 - for testing, set to the full number of records so writing to file will be triggered

//             //Testing speed of single lock access using SafeArrayAccessData() ======
//            double dd = 0.0;
//            string test = "123094871.23908749132";
//            string dd2 = "";
//            for (int i = 0; i < currentSensorData->getData()->rgsabound->cElements; i++)
//            {

//                dd = (double)currentSensorDataDouble[i];
//                //dd2 = (string)currentSensorDataDouble[i];
//          //      dd2 = std::to_string(dd);



//                //outputStdStr.append(std::to_string(dd)); //test time for double to string conversion - to_string is slow!!
//                outputStdStr.append(test); //test writing a string without any conversion - fast
//                //outputStdStr.append(dd2);

//            }

//            ::SafeArrayUnaccessData(currentSensorData->getData());

//            continue;
//            //======================================================================
//*/
//            //======================================================================================



//            //get the number of samples in the sample data

//            double d = 0.0; //hold each sample as the sensor data is iterated, once saved, this variable is
//                            //  set to the next sample from the sensor
//            double ts = 0.0;//CM 5/26/15 - store the time second timestamp for the sample
//            double tf = 0.0; //CM 5/26/15 - store the time fraction timestamp for the sample

//            //CM 5/27/15 - since the timestamp is the same for each channel for each record (and only processing one record at a time),
//            //      Then just grab the timestamp of the first entry, i.e., index = 0
//            int timeIndex = 0;
//            ::SafeArrayGetElement(currentSensorData->getTimeSeconds(), (LONG *)&timeIndex, &ts);
//            ::SafeArrayGetElement(currentSensorData->getTimeFraction(), (LONG *)&timeIndex, &tf);

//            //Now to iterate through the sensor data. This is completed by "jumping" through the linear stream of data
//            //  provided by the DAQ for each channel for each record. Even though I am not accessing it in linear fashion,
//            //  I only see one sample once, which leads to linear performance.
//            //  Format of linear stream:
//            //      For record of size 4 and 3 channels:
//            //              ------------------------------ record 1 ------------------------------ ----- record 2----------
//            //              [[channel 1 ; 4 samples][channel 2 ; 4 samples][channel 3 ; 4 samples][channel 1 ; 4 samples]...]

//           //iterate over all the available records
//            for (int record = 0; record < currentSensorData->getNumRecords(); record++)
//            {
//                recordsCollected++; //CM 8/6/15 - record each record written out

//                if(DEBUG_MODE == 1){
//                    recordEntryLoopCount++;
//                }

//                //iterate through each record, i.e., recordSize is the number of samples in a record,
//                //  so if the record size is 20, then 20 samples are read for each channel for that specific record
//                for (size_t i = 0; i < recordSize; i++)
//                {
//                    if(DEBUG_MODE == 1){
//                        recordSizeLoopCount++;
//                    }

//                     //Only pull daata from channels with sensors
//                     if(ALL_CHANNEL_DATA == 0){
//                         int channel = 0; //specify which channel data to get
//                         //iterate through all the active channels
//                        for(int specificChannel = 0; specificChannel < activeChannels.size();specificChannel++){
//                            channel = activeChannels.at(specificChannel).first;

//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                std::string currentChannelID = channelIDs->at(channel);
//                                //if the last channel, leave off the comma
//                                if(specificChannel == activeChannels.size()-1){
//                                    std::string temp = "Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n';
//                                    //headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
//                                    headerStdStr.append(temp);
//                                    temp.clear();

//                                    //removed for testing writing entire string once to file ================
//                                    //Out << headerStr.toStdString(); //since done, then write to file
//                                    //=======================================================================

//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
//                                    headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                }
//                            }

//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;


//                            //::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);
//                            d = (double)currentSensorDataDouble[index]; //CM 8/7/15 - update to use safe access

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
//                                //outputStdStr.append(QString::number(ts)+',');
//                                //outputStdStr.append(QString::number(tf)+',');

//   //                             outputStdStr.append(std::to_string(ts)+','); //CM 7/10/15 - using std::string
//   //                             outputStdStr.append(std::to_string(tf)+',');
//                            }

//                            //add current sample to string to be written to file
//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            if(specificChannel == activeChannels.size()-1){
//                                //outputStdStr.append(QString::number(d));
//                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{
//                                //outputStdStr.append(QString::number(d)+',');
//                                outputStdStr.append(std::to_string(d)+','); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }
//                     //otherwise, pull samples from all channels, despite if they have sensors connected to them
//                     else{
//                        //iterate through all channels (active and inactive) and pull the data
//                         for(int channel = 0; channel < numPhysicalChannels; channel++)
//                        {
//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                //if the last channel, leave off the comma
//                                //QString currentChannelID = channelIDs->at(channel);
//                                std::string currentChannelID = channelIDs->at(channel); //CM 7/10/15 - using std::string
//                                if(channel == numPhysicalChannels-1){
//                                    headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
////                                    Out << headerStr.toStdString(); //since done, then write to file
//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
//                                    headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                }
//                            }
//                            //================================================================================================================
//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;//+(record*data_count);

//                            //::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);
//                            d = (double)currentSensorDataDouble[index]; //CM 8/7/15 - update to use safe access

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
//                                //outputStdStr.append(QString::number(ts)+',');
//                                //outputStdStr.append(QString::number(tf)+',');

//    //                            outputStdStr.append(std::to_string(ts)+','); //CM 7/10/15 - using std::string
//    //                            outputStdStr.append(std::to_string(tf)+',');
//                            }


//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            //if(channel == numChannels-1){
//                            if(channel == numPhysicalChannels-1){

//                                //outputStdStr.append(QString::number(d));
//                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{

//                                //outputStdStr.append(QString::number(d)+',');
//                                outputStdStr.append(std::to_string(d)+','); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }


//                    outputStdStr.append("\n"); //signal end of line once an entry in a record for all channels is recorded
//                }

//            }

//            //===========================================================================================================


//            //debugging - check to see how many times the loops above are actually being called
//            if(DEBUG_MODE == 1){
//                 std::cout<<"Safe loop count: "<<sensorItLoopCount<<" Record Entry Loop Count: "<<recordEntryLoopCount<< " Record Size Loop Count: "<<recordSizeLoopCount<<" Channel Loop Count: "<<channelLoopCount<<std::endl;
//                 recordEntryLoopCount = 0; //reset
//                 channelLoopCount = 0;
//                 recordSizeLoopCount = 0;
//            }

//        }

//        //CM 7/15/15 - testing time required for iterating and storing data into a string ============
//        duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//        std::cout<<"Time to iterate over data and store sensor data into string: "<<duration<<std::endl;

//        //std::cout<<"Time to iterate over data and store sensor data into string: "<<durationSum<<std::endl;
//        //============================================================================================

//        //==========================================================================================
//        //CM 7/10/15 - I do not need to display this info as it is a performace hit
//        /*
//        if(DISPLAY_FILE_IO_INFO == 1){
//            int numLines = outputStdStr.count('\n');
//            std::cout<<numLines<<" lines written to file for "<<sensorData->size()<<" records."<<std::endl;
//        }
//        */
//        //==========================================================================================
//        //remove for testing ===================
//        /*
//        Out << outputStr.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr.clear();
//        Out.close();
//        */
////======================================

//        //testing=========================
//        /*
//        Out2 << outputStr2.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr2.clear();
//        Out2.close();

//        Out3 << outputStr3.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr3.clear();
//        Out3.close();
//        */
//        //================================

//        outputCount++; //CM 3/18/15 - increment for next write to file
//    }
//    catch (...){
//       //for now, just output an error message indicating where the error occured
//       std::cout<<"Exception caught: CSV_Output class, wrtieDaqDataToFile."<<std::endl;
//    }

//    channelIDs = NULL; //CM 7/24/15 - cut association with pointer to channel IDs
//    sensorData = NULL; //CM 7/23/15 - cut association with pointer to sensor data
//}

void CSV_Output::writeDaqDataToFile_standard_string_remove_qt(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, list< pair<int,std::string> > activeChannels, std::string daqID){
    std::string fileName = "";
//    QString outputStr = ""; //CM 5/18/15 - the string to be written to the file after pulling data from the records
//    QString headerStr = ""; //CM 5/19/15 - the string for the header of a CSV file that contains the channel IDs


    //CM 8/6/15 - how many records are to be written to a file based on the sample rate, record, and the desired time length of the file (timeThreshold)
 //   int recordsPerFile = timeThreshold / (double(recordSize)/double(sampleRate));

    //std::cout<<"Records to be written to file: "<<recordsPerFile<<std::endl;

    int currentWriteTime = int(currentTimeSeconds()); //CM 4/14/15 - get the current (now) time for comparison to the last write time

    /* CM 8/18/15 - test moving writing file to end so writing can occur after parsing data since I changed my logic to collect all
     *  the records for one file in one collection sequence in DAQ.cpp
     *

    //CM 4/14/15 - if reached the time threshold, then close then create a new file to write to
    //if(currentWriteTime-lastWriteTime >= timeThreshold){
    //if(recordsCollected == recordsPerFile){
    if(recordsCollected == fileLength){ //CM 8/7/15 - assuming 1 record equals 1 second of data
       //lastWriteTime = currentWriteTime;

       recordsCollected = 0; //reset

       //QString dateTime = currentDateTime(); //CM 4/16/15 - testing string formate of date and time

       //testing=====================
       //CM 7/15/15 - time writing to file
       clock_t start;
       start = std::clock();
       double duration = 0.0;

       //std::ofstream Out(currentFileName, std::ios::app);

       //CM 8/12/15 - converting timestamp from DAQs to current date and time ======================
       time_t t = firstTimeSecond;
       //struct tm lt;

       // convert now to string form
       char* dt = ctime(&t);

       struct tm * timeinfo;

       timeinfo = localtime(&t);

       cout << "The local date and time is: " << dt << endl;

       char buf[80];

       // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
       // for more information about date/time format
       strftime(buf, sizeof(buf), "%Y-%m-%d_%H_%M_%S", timeinfo); //format timestamp of first data point in the file to a date/time

        cout<<buf<<endl;
       //cout<<timeinfo->tm_year<<":"<<timeinfo->tm_yday<<":"<<timeinfo->tm_hour<<":"<<timeinfo->tm_min<<":"<<timeinfo->tm_sec<<endl;
       //==================================================================================

       currentFileName = string(buf);       //create a string out of the formated timstamp
       currentFileName = currentFileName + ".csv"; //add file extention
       std::ofstream Out(currentFileName, std::ios::app); //CM 8/12/15 - Now open a file with the name being the timestampe of the first data point

       //====================================================================================================================
       //CM 8/12/15 - add header to store information necessary to calculate the exact time of each sample in the file
       //string infoHeaderStr;

       //NOTE: use to_string() to retain the accuracy of the timestamp info, i.e., all significant figures are retained

       //infoHeaderStr = "Time Integer: "<<firstTimeSecond<<" Time Fraction: "<<firstTimeFraction<<" Sample Rate (samples per second): "<<sampleRate<<" Length of file in seconds"<<'\n';
       Out << "Time Integer, Time Fraction, Sample Rate (samples per second), Length of file in seconds\n";
       Out <<to_string(firstTimeSecond)<<','<<to_string(firstTimeFraction)<<','<<sampleRate<<','<<fileLength<<'\n';

       //====================================================================================================================


       Out << headerStdStr;
       Out << outputStdStr; //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
       std::cout<<"Time to write file before string clear: "<<duration<<std::endl;
       headerStdStr.clear();
       outputStdStr.clear();
       Out.flush(); //CM 7/23/15
       Out.close();
       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
       std::cout<<"Time to write file after string clear: "<<duration<<std::endl;

       //CM 4/16/15 - move completed file to another directory
       std::string moveCommand = "move " + currentFileName + " " + moveDir;

       system(moveCommand.c_str()); //move recent finished file

       //perform time benchmark testing for processing each file
       double timeForCollection = 0.0;
       timeForCollection = endTimer();
       std::cout<<"-------------------------------------------------"<<std::endl;
       std::cout<<"Collection/Write cycle time lapse: "<<timeForCollection<<std::endl;
       std::cout<<"-------------------------------------------------"<<std::endl;

 //      currentFileName = daqID+"_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
       //currentFileName = "daq_data_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
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
   */

   std::vector<Sensor_Data*>::iterator sensorIt; //iterator through the list of sensor objects representing samples returned from the DAQ

    try{

        //CM 5/27/15 - update current file name to include the DAQ ID when this function is called for the very first time
        //      This is analogous to detecting the beginning of a write to the file so the channel ID header can be written
        //      only once to the file.
        if(initializeFileName == false){
  //          currentFileName = daqID+"_"+currentFileName;
            initializeFileName = true;
        }

        //open file for writing, append to end of file
        //remove for testing ==========
        //std::ofstream Out(currentFileName, std::ios::app);
        //=============================

        //testing================
        /*
        QString outputStr2 = "";
        QString outputStr3 = "";
         std::ofstream Out2(currentFileName+"_2", std::ios::app);
         std::ofstream Out3(currentFileName+"_3", std::ios::app);
         */
        //=======================

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


        //CM 7/15/15 - testing time required for iterating and storing data into a string ============
        clock_t start;
        start = std::clock();
        double duration = 0.0;
        double durationSum = 0.0;
        //============================================================================================

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
                /*
                double d = 0.0; //hold each sample as the sensor data is iterated, once saved, this variable is
                                //  set to the next sample from the sensor
                double ts = 0.0;//CM 5/26/15 - store the time second timestamp for the sample
                double tf = 0.0; //CM 5/26/15 - store the time fraction timestamp for the sample
                */

                //CM 5/27/15 - since the timestamp is the same for each channel for each record (and only processing one record at a time),
                //      Then just grab the timestamp of the first entry, i.e., index = 0
                int timeIndex = 0;
                ::SafeArrayGetElement(currentSensorData->getTimeSeconds(), (LONG *)&timeIndex, &firstTimeSecond);
                ::SafeArrayGetElement(currentSensorData->getTimeFraction(), (LONG *)&timeIndex, &firstTimeFraction);
            }
            //===============================================================



            //====== testing just writing straight from safearrays without indexing  for specific channels ============
            /*
            //testing getting a range from the safearray =====
            BSTR* longArray = NULL;

            //================================================

            double dd = 0;
            Sensor_Data* sensorData = *sensorIt;
            for(int y = 0; y < sensorData->getData()->rgsabound->cElements; y++){
                ::SafeArrayGetElement(sensorData->getData(), (LONG *)&y, &dd);
                //outputStr.append(QString::number(dd)+"2345678901v2345678"+'\n');
                //outputStr2.append(QString::number(dd)+'\n');
                //outputStr3.append(QString::number(dd)+'\n');



                std::cout<<"Trying to convert safearray to string"<<std::endl;
                SafeArrayAccessData(sensorData->getData(),(void**)longArray); //testing getting a range from the safearray
                std::cout<<"1"<<std::endl;
                const char *p= const_cast<char *>(_com_util::ConvertBSTRToString(*longArray));
               //std::cout<<"Long array: "<<*longArray<<std::endl;
                std::cout<<"2"<<std::endl;
                std::string dataStr = std::string(p);
                std::cout<<"3"<<std::endl;
                std::cout<<"data array size: "<<dataStr.size()<<std::endl;

                outputStdStr.append(std::to_string(dd));
            }

            //duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
            //std::cout<<"Time to iterate over data without indexing: "<<duration<<std::endl;

            continue;
            */
            //========================================================================================================


            //Sensor_Data* currentSensorData = NULL;
            //currentSensorData = *sensorIt; //grab the current sensor data object

            //CM 8/7/15 - update to use safe array access ===========================================================
            //double *currentSensorDataDouble = NULL;
            currentSensorDataDouble = NULL;
            //string *currentSensorDataDouble;
            HRESULT hr;

            // Get a pointer to the elements of the array.
            hr = ::SafeArrayAccessData(currentSensorData->getData(), (void**)&currentSensorDataDouble);

            if (FAILED(hr)){
                cout<<"Error geting access to SAFEARRAY of sensor data. Aborting..."<<endl;
                return;
            }
            //=======================================================================================================

            //======================================================================================
            //CM 7/16/15 - adding support for accessing data from SAFEARRAY using a single lock
            /*
            //BSTR HUGEP *currentSensorDataBSTR;
            double *currentSensorDataDouble;
            //string *currentSensorDataDouble;
            HRESULT hr;

            // Get a pointer to the elements of the array.
            hr = ::SafeArrayAccessData(currentSensorData->getData(), (void**)&currentSensorDataDouble);

            if (FAILED(hr)){
                cout<<"Error geting access to SAFEARRAY of sensor data. Aborting..."<<endl;
                return;
            }

            recordsCollected++;// = currentSensorData->getNumRecords(); //CM 8/7/15 - for testing, set to the full number of records so writing to file will be triggered

             //Testing speed of single lock access using SafeArrayAccessData() ======
            double dd = 0.0;
            string test = "123094871.23908749132";
            string dd2 = "";
            for (int i = 0; i < currentSensorData->getData()->rgsabound->cElements; i++)
            {

                dd = (double)currentSensorDataDouble[i];
                //dd2 = (string)currentSensorDataDouble[i];
          //      dd2 = std::to_string(dd);



                //outputStdStr.append(std::to_string(dd)); //test time for double to string conversion - to_string is slow!!
                outputStdStr.append(test); //test writing a string without any conversion - fast
                //outputStdStr.append(dd2);

            }

            ::SafeArrayUnaccessData(currentSensorData->getData());

            continue;
            //======================================================================
*/
            //======================================================================================



            //get the number of samples in the sample data

            //double d = 0.0; //hold each sample as the sensor data is iterated, once saved, this variable is
                            //  set to the next sample from the sensor
            double ts = 0.0;//CM 5/26/15 - store the time second timestamp for the sample
            double tf = 0.0; //CM 5/26/15 - store the time fraction timestamp for the sample

            //CM 5/27/15 - since the timestamp is the same for each channel for each record (and only processing one record at a time),
            //      Then just grab the timestamp of the first entry, i.e., index = 0
            int timeIndex = 0;
            ::SafeArrayGetElement(currentSensorData->getTimeSeconds(), (LONG *)&timeIndex, &ts);
            ::SafeArrayGetElement(currentSensorData->getTimeFraction(), (LONG *)&timeIndex, &tf);


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

                //debugging =====================
                //cout<<recordsCollected<<endl;
                //================================

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
                                //if(specificChannel == activeChannels.size()-1){
                                //CM 8/7/15 - if the last channel, then leave off the comma - check the next iterator to see if at the end
                                if(std::next(it,1) == activeChannels.end()){
                                    //std::string temp = "Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n';
                                    std::string temp = "Data_"+currentChannelID+'\n'; //CM 8/7/15 - remove the timestamp from the header
                                    //headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
                                    headerStdStr.append(temp);
                                    temp.clear();

                                    //removed for testing writing entire string once to file ================
                                    //Out << headerStr.toStdString(); //since done, then write to file
                                    //=======================================================================

                                    headerWritten = true; //header is written, do not write one again until the next CSV file
                                }
                                //otherwise, put the comma on since more channels are to come
                                else{
                                    //headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
                                    headerStdStr.append("Data_"+currentChannelID+','); //CM 8/7/15 - remove the timestamp from the header
                                }
                            }

                            //index calculation: (the current channel * the size of a record) + the current index in the record
                            size_t index = (channel*recordSize)+i;


                            //::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);
                            d = (double)currentSensorDataDouble[index]; //CM 8/7/15 - update to use safe access

                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
                            if(INCLUDE_TIMESTAMP == 1){
                                //outputStdStr.append(QString::number(ts)+',');
                                //outputStdStr.append(QString::number(tf)+',');

        //                        outputStdStr.append(std::to_string(ts)+','); //CM 7/10/15 - using std::string
        //                        outputStdStr.append(std::to_string(tf)+',');
                            }

                            //add current sample to string to be written to file
                            //CM 5/19/15 - if at the last channel, do not add a comma
                            //if(specificChannel == activeChannels.size()-1){
                            //CM 8/7/15 - if the last channel, then leave off the comma - check the next iterator to see if at the end
                            if(std::next(it,1) == activeChannels.end()){
                                //outputStdStr.append(QString::number(d));
                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
                                if(DETAILED_OUPUT == 1){
                                    numPoints++;
                                }
                            }
                            //Otherwise, still more channels to go, so add a comma
                            else{
                                //outputStdStr.append(QString::number(d)+',');
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
                                //QString currentChannelID = channelIDs->at(channel);
                                std::string currentChannelID = channelIDs->at(channel); //CM 7/10/15 - using std::string
                                if(channel == numPhysicalChannels-1){
                                    //headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
                                    headerStdStr.append("Data_"+currentChannelID+'\n'); //CM 8/7/15 - remove the timestamp from the header
//                                    Out << headerStr.toStdString(); //since done, then write to file
                                    headerWritten = true; //header is written, do not write one again until the next CSV file
                                }
                                //otherwise, put the comma on since more channels are to come
                                else{
                                    //headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
                                    headerStdStr.append("Data_"+currentChannelID+','); //CM 8/7/15 - remove the timestamp from the header
                                }
                            }
                            //================================================================================================================
                            //index calculation: (the current channel * the size of a record) + the current index in the record
                            size_t index = (channel*recordSize)+i;//+(record*data_count);

                            //::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);
                            d = (double)currentSensorDataDouble[index]; //CM 8/7/15 - update to use safe access

                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
                            if(INCLUDE_TIMESTAMP == 1){
                                //outputStdStr.append(QString::number(ts)+',');
                                //outputStdStr.append(QString::number(tf)+',');

    //                            outputStdStr.append(std::to_string(ts)+','); //CM 7/10/15 - using std::string
    //                            outputStdStr.append(std::to_string(tf)+',');
                            }


                            //CM 5/19/15 - if at the last channel, do not add a comma
                            //if(channel == numChannels-1){
                            if(channel == numPhysicalChannels-1){

                                //outputStdStr.append(QString::number(d));
                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
                                if(DETAILED_OUPUT == 1){
                                    numPoints++;
                                }
                            }
                            //Otherwise, still more channels to go, so add a comma
                            else{

                                //outputStdStr.append(QString::number(d)+',');
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

        //CM 7/15/15 - testing time required for iterating and storing data into a string ============
        duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
        std::cout<<"Time to iterate over data and store sensor data into string: "<<duration<<std::endl;

        //std::cout<<"Time to iterate over data and store sensor data into string: "<<durationSum<<std::endl;
        //============================================================================================

        //==========================================================================================
        //CM 7/10/15 - I do not need to display this info as it is a performace hit
        /*
        if(DISPLAY_FILE_IO_INFO == 1){
            int numLines = outputStdStr.count('\n');
            std::cout<<numLines<<" lines written to file for "<<sensorData->size()<<" records."<<std::endl;
        }
        */
        //==========================================================================================
        //remove for testing ===================
        /*
        Out << outputStr.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
        outputStr.clear();
        Out.close();
        */
//======================================

        //testing=========================
        /*
        Out2 << outputStr2.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
        outputStr2.clear();
        Out2.close();

        Out3 << outputStr3.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
        outputStr3.clear();
        Out3.close();
        */
        //================================

        outputCount++; //CM 3/18/15 - increment for next write to file
    }
    catch (...){
       //for now, just output an error message indicating where the error occured
       std::cout<<"Exception caught: CSV_Output class, wrtieDaqDataToFile."<<std::endl;
    }

    channelIDs = NULL; //CM 7/24/15 - cut association with pointer to channel IDs
    sensorData = NULL; //CM 7/23/15 - cut association with pointer to sensor data

    //========================================================================================
    // CM 8/18/15 - test moving writing file to end so writing can occur after parsing data since I changed my logic to collect all
    //  the records for one file in one collection sequence in DAQ.cpp
    //  There "should" not be a problem with this since I collect the same number of records in one collection sequence
    //  (before coming to this function) as the fileLength, hence recordsCollected "should" always equal fileLength
    //  when control reaches here.


    //CM 4/14/15 - if reached the time threshold, then close then create a new file to write to
    //if(currentWriteTime-lastWriteTime >= timeThreshold){
    //if(recordsCollected == recordsPerFile){
//    if(recordsCollected == fileLength){ //CM 8/7/15 - assuming 1 record equals 1 second of data
       //lastWriteTime = currentWriteTime;

       recordsCollected = 0; //reset

       //QString dateTime = currentDateTime(); //CM 4/16/15 - testing string formate of date and time

       //debugging =====================
       //CM 7/15/15 - time writing to file
       /*
       clock_t start;
       start = std::clock();
       double duration = 0.0;
       */
       //===============================

       //std::ofstream Out(currentFileName, std::ios::app);

       //CM 8/12/15 - converting timestamp from DAQs to current date and time =============
       time_t t = firstTimeSecond;

       // convert now to string form
       char* dt = ctime(&t);
       struct tm * timeinfo;
       timeinfo = localtime(&t);

       //debugging =================
       //cout << "The local date and time is: " << dt << endl;
       //===========================

       char buf[80];

       // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
       // for more information about date/time format
       strftime(buf, sizeof(buf), "%Y-%m-%d_%H_%M_%S", timeinfo); //format timestamp of first data point in the file to a date/time

       //debugging =================
        //cout<<buf<<endl;
        //==========================

       //cout<<timeinfo->tm_year<<":"<<timeinfo->tm_yday<<":"<<timeinfo->tm_hour<<":"<<timeinfo->tm_min<<":"<<timeinfo->tm_sec<<endl;
       //==================================================================================

       currentFileName = string(buf);       //create a string out of the formated timstamp
       currentFileName = currentFileName + ".csv"; //add file extention
       std::ofstream Out(currentFileName, std::ios::app); //CM 8/12/15 - Now open a file with the name being the timestampe of the first data point

       //====================================================================================================================
       //CM 8/12/15 - add header to store information necessary to calculate the exact time of each sample in the file
       //string infoHeaderStr;

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

       //debugging =================
       //duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
       //std::cout<<"Time to write file before string clear: "<<duration<<std::endl;
       //============================
       headerStdStr.clear();
       outputStdStr.clear();
       Out.flush(); //CM 7/23/15
       Out.close();

       //debugging =================
       //duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
       //std::cout<<"Time to write file after string clear: "<<duration<<std::endl;
       //===========================

       //CM 4/16/15 - move completed file to another directory
       std::string moveCommand = "move " + currentFileName + " " + moveDir;

       system(moveCommand.c_str()); //move recent finished file

       //perform time benchmark testing for processing each file
       double timeForCollection = 0.0;
       timeForCollection = endTimer();
       std::cout<<"-------------------------------------------------"<<std::endl;
       std::cout<<"Collection/Write cycle time lapse: "<<timeForCollection<<std::endl;
       std::cout<<"-------------------------------------------------"<<std::endl;

 //      currentFileName = daqID+"_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
       //currentFileName = "daq_data_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
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
//   }
    //CM 8/18/15 - catch when the records collected is not equal the file length (remember one record is one second and file
    //      length is in seconds). This "should" never happen, but put this else clause just in case
//    else{
//        cout<<"RECORDS COLLECTED IS NOT THE SAME AS FILE LENGTH!!! THIS SHOULD NOT HAPPEN!!"<<endl;
//        cout<<'\t'<<"Records Collected: "<<recordsCollected<<" File Length: "<<fileLength<<endl;

//    }
   //=========================================================================================

}

//void CSV_Output::writeDaqDataToFile_vector(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, QList< QPair<int,std::string> > activeChannels, std::string daqID){
//    std::string fileName = "";


//   int currentWriteTime = int(currentTimeSeconds()); //CM 4/14/15 - get the current (now) time for comparison to the last write time


//   //CM 4/14/15 - if reached the time threshold, then close then create a new file to write to
//   if(currentWriteTime-lastWriteTime >= timeThreshold){
//       lastWriteTime = currentWriteTime;

//       QString dateTime = currentDateTime(); //CM 4/16/15 - testing string formate of date and time

//       //testing=====================
//       //CM 7/15/15 - time writing to file
//       //clock_t start;
//       //start = std::clock();
//       //double duration = 0.0;

//       //std::ofstream Out(currentFileName, std::ios::app);

//       //CM 7/17/15 - testing using a vector of doubles and outputing to the file ===================
//       /*
//       for(int x = 0; x < samples.size(); x++){
//           Out << samples.at(x) << '\n';
//       }
//       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//       std::cout<<"Time to write file iterating through vector: "<<duration<<std::endl;
//       */
//       //============================================================================================

//       //original ====================================================
//       /*
//       Out << headerStdStr;
//       Out << outputStdStr; //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//       std::cout<<"Time to write file before string clear: "<<duration<<std::endl;
//       headerStdStr.clear();
//       outputStdStr.clear();
//       Out.close();
//       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//       std::cout<<"Time to write file after string clear: "<<duration<<std::endl;
//       */
//       //=============================================================

//       //CM 4/16/15 - move completed file to another directory
//       std::string moveCommand = "move " + currentFileName + " " + moveDir;

//       system(moveCommand.c_str()); //move recent finished file

//       //perform time benchmark testing for processing each file
//       double timeForCollection = 0.0;
//       timeForCollection = endTimer();
//       std::cout<<"-------------------------------------------------"<<std::endl;
//       std::cout<<"Collection/Write cycle time lapse: "<<timeForCollection<<std::endl;
//       std::cout<<"-------------------------------------------------"<<std::endl;

// //      currentFileName = daqID+"_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       currentFileName = "daq_data_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       headerWritten = false; //CM 5/19/15 - with the new file, signal the header needs to be written

//       //only print info when wanting more details about what the collection system is doint
//       if(DETAILED_OUPUT == 1){
//           std::cout<<"Number of samples written: "<<numPoints<<" over "<<timeThreshold<<" seconds"<<std::endl
//                    <<'\t'<<"Number of samples written = sample rate * time slice for collecting (in seconds) * number of channels ==>"<<std::endl
//                    <<"\t \t"<<numPoints<<" = "<<sampleRate<<" * "<<timeThreshold<<" * "<<numPhysicalChannels<<std::endl
//                    <<std::endl;
//       }
//       numPoints = 0;
//   }

//   std::vector<Sensor_Data*>::iterator sensorIt; //iterator through the list of sensor objects representing samples returned from the DAQ

//    try{

//        //CM 5/27/15 - update current file name to include the DAQ ID when this function is called for the very first time
//        //      This is analogous to detecting the beginning of a write to the file so the channel ID header can be written
//        //      only once to the file.
//        if(initializeFileName == false){
//  //          currentFileName = daqID+"_"+currentFileName;
//            initializeFileName = true;
//        }

//        //open file for writing, append to end of file
//        //remove for testing ==========
//        //std::ofstream Out(currentFileName, std::ios::app);
//        //=============================

//        //testing================
//        /*
//        QString outputStr2 = "";
//        QString outputStr3 = "";
//         std::ofstream Out2(currentFileName+"_2", std::ios::app);
//         std::ofstream Out3(currentFileName+"_3", std::ios::app);
//         */
//        //=======================

//        if (DEBUG_MODE == 1){
//            std::cout << "Writing data..." << std::endl;
//        }

//        //initialize loop count variables for debugging. They  need to be declared outside of a check for debug mode is on/off
//        //=======================
//        int sensorItLoopCount = 0;
//        int recordEntryLoopCount = 0;
//        int recordSizeLoopCount = 0;
//        int channelLoopCount = 0;
//        //=======================

//        std::ofstream Out(currentFileName, std::ios::app); //CM 7/17/15 - open output file for each batch of data
//        FILE * pFile;
//        pFile = fopen ("myfile.txt", "w");
//        std::ofstream outBin("myBinfile.bin",ios::binary);
//        //std::ofstream inBin("myBinfile.bin",ios::binary);
//        double *currentSensorDataDouble;
//        HRESULT hr;
//        double duration = 0.0;
//        double durationSum = 0.0;
//        Sensor_Data* currentSensorData = NULL;
//        double dd = 0.0;
//        string test = "123094871.23908749132";
//        string *dd2 = NULL;
//        stringstream ss;
//        char  *conversion = NULL;



//        //CM 7/15/15 - testing time required for iterating and storing data into a string ============
//        clock_t start;
//        start = std::clock();

//        //============================================================================================

//        //Iterate through each sensor object and pull out the data
//        for(sensorIt = sensorData->begin(); sensorIt != sensorData->end();sensorIt++){

//            //keep track of the number of sensor loops
//            if(DEBUG_MODE == 1){
//                sensorItLoopCount++;
//            }



//            //====== testing just writing straight from safearrays without indexing  for specific channels ============
//            /*
//            //testing getting a range from the safearray =====
//            BSTR* longArray = NULL;

//            //================================================

//            double dd = 0;
//            Sensor_Data* sensorData = *sensorIt;
//            for(int y = 0; y < sensorData->getData()->rgsabound->cElements; y++){
//                ::SafeArrayGetElement(sensorData->getData(), (LONG *)&y, &dd);
//                //outputStr.append(QString::number(dd)+"2345678901v2345678"+'\n');
//                //outputStr2.append(QString::number(dd)+'\n');
//                //outputStr3.append(QString::number(dd)+'\n');



//                std::cout<<"Trying to convert safearray to string"<<std::endl;
//                SafeArrayAccessData(sensorData->getData(),(void**)longArray); //testing getting a range from the safearray
//                std::cout<<"1"<<std::endl;
//                const char *p= const_cast<char *>(_com_util::ConvertBSTRToString(*longArray));
//               //std::cout<<"Long array: "<<*longArray<<std::endl;
//                std::cout<<"2"<<std::endl;
//                std::string dataStr = std::string(p);
//                std::cout<<"3"<<std::endl;
//                std::cout<<"data array size: "<<dataStr.size()<<std::endl;

//                outputStdStr.append(std::to_string(dd));
//            }

//            //duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//            //std::cout<<"Time to iterate over data without indexing: "<<duration<<std::endl;

//            continue;
//            */
//            //========================================================================================================


//            //Sensor_Data* currentSensorData = NULL;
//            currentSensorData = *sensorIt; //grab the current sensor data object

//            //======================================================================================
//            /*
//            //CM 7/16/15 - adding support for accessing data from SAFEARRAY using a single lock

//            //BSTR HUGEP *currentSensorDataBSTR;
//            //double *currentSensorDataDouble;
//            //char *currentSensorDataChar;
//            //string *currentSensorDataDouble;
//            //HRESULT hr;

//            // Get a pointer to the elements of the array.
//            hr = ::SafeArrayAccessData(currentSensorData->getData(), (void**)&currentSensorDataDouble);
//            //hr = ::SafeArrayAccessData(currentSensorData->getData(), (void**)&currentSensorDataChar);

//            if (FAILED(hr)){
//                cout<<"Error geting access to SAFEARRAY of sensor data. Aborting..."<<endl;
//                return;
//            }

//             //Testing speed of single lock access using SafeArrayAccessData() ======

//            //double dd = 0.0;
//            //string test = "123094871.23908749132";
//            //string *dd2 = NULL;
//            //stringstream ss;

//            //samples.reserve(currentSensorData->getData()->rgsabound->cElements);

//            //============================================================================
//            //CM 7/21/15 - testing using fwrite to file using pointer to array of doubles provided from SafeArrayAccessData()
//             //fwrite (currentSensorDataDouble , sizeof(double), currentSensorData->getData()->rgsabound->cElements, pFile);
//             //cout<<(double)currentSensorDataDouble[3]<<endl;

//             //cout<<(char)currentSensorDataChar[3]<<endl;
//             Out.write(reinterpret_cast<const char*>(currentSensorDataDouble), std::streamsize((currentSensorData->getData()->rgsabound->cElements)*sizeof(double)));
//            //============================================================================

//            //char   *conversion = NULL;
//            //char output[50];

//            //char   *allSamples = NULL;

//            for (int i = 0; i < currentSensorData->getData()->rgsabound->cElements; i++)
//            {
//                dd = (double)currentSensorDataDouble[i];


//                //snprintf(output,50,"%f",dd);
//                //printf("%s",output);
//                //outBin << dd;
//                //dd2 = (string)currentSensorDataDouble[i];
//                //dd2 = std::to_string(dd);

//                //Out<< dd << '\n';
//                //ss << dd << '\n';
////                outBin<< dd << '\n';

//                //samples.push_back(dd); //CM 7/17/15 - store sample in vector
//                //conversion = reinterpret_cast<char *>(&dd); //CM 7/17/15 - NOTE: FOR SOME REASON conversion IS EMPTY!!!!
//                //dd2 = reinterpret_cast<string*>(&dd);
//                //cout<<dd2<<endl;
//                //cout<<conversion<<endl;
//                //strcat (conversion, "\0");
//                //Out << conversion <<endl;
////                outputStdStr.append(string(conversion));
//                //cout<<conversion<<endl;
//                //strcat (allSamples,conversion);
//                //Out<<*(reinterpret_cast<double *>(conversion))<<'\n';
//                //Out<<reinterpret_cast<string>(dd)<<'\n';
//                //outputStdStr.append(*(reinterpret_cast<double *>(conversion)));

//                //outputStdStr.append(std::to_string(dd)); //test time for double to string conversion - to_string is slow!!
//                //outputStdStr.append(test); //test writing a string without any conversion - fast
//                 //outputStdStr.append(dd2);

//            }
///
//            //Out << ss;
//            //Out<<*(reinterpret_cast<double *>(allSamples))<<'\n';
////            Out << outputStdStr << '\n';
////            outputStdStr.clear();

//            //samples.clear();

//            ::SafeArrayUnaccessData(currentSensorData->getData());

//            continue;
//            //======================================================================
//*/
//            //======================================================================================



//            //get the number of samples in the sample data

//            double d = 0.0; //hold each sample as the sensor data is iterated, once saved, this variable is
//                            //  set to the next sample from the sensor
//            double ts = 0.0;//CM 5/26/15 - store the time second timestamp for the sample
//            double tf = 0.0; //CM 5/26/15 - store the time fraction timestamp for the sample

//            //CM 5/27/15 - since the timestamp is the same for each channel for each record (and only processing one record at a time),
//            //      Then just grab the timestamp of the first entry, i.e., index = 0
//            int timeIndex = 0;
//           ::SafeArrayGetElement(currentSensorData->getTimeSeconds(), (LONG *)&timeIndex, &ts);
//            ::SafeArrayGetElement(currentSensorData->getTimeFraction(), (LONG *)&timeIndex, &tf);

//            //Now to iterate through the sensor data. This is completed by "jumping" through the linear stream of data
//            //  provided by the DAQ for each channel for each record. Even though I am not accessing it in linear fashion,
//            //  I only see one sample once, which leads to linear performance.
//            //  Format of linear stream:
//            //      For record of size 4 and 3 channels:
//            //              ------------------------------ record 1 ------------------------------ ----- record 2----------
//            //              [[channel 1 ; 4 samples][channel 2 ; 4 samples][channel 3 ; 4 samples][channel 1 ; 4 samples]...]

//           //iterate over all the available records
//            for (int record = 0; record < currentSensorData->getNumRecords(); record++)
//            {

//                if(DEBUG_MODE == 1){
//                    recordEntryLoopCount++;
//                }

//                //iterate through each record, i.e., recordSize is the number of samples in a record,
//                //  so if the record size is 20, then 20 samples are read for each channel for that specific record
//                for (size_t i = 0; i < recordSize; i++)
//                {
//                    if(DEBUG_MODE == 1){
//                        recordSizeLoopCount++;
//                    }

//                     //Only pull daata from channels with sensors
//                     if(ALL_CHANNEL_DATA == 0){
//                         int channel = 0; //specify which channel data to get
//                         //iterate through all the active channels
//                        for(int specificChannel = 0; specificChannel < activeChannels.size();specificChannel++){
//                            channel = activeChannels.at(specificChannel).first;

//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                std::string currentChannelID = channelIDs->at(channel);
//                                //if the last channel, leave off the comma
//                                if(specificChannel == activeChannels.size()-1){
//                                    std::string temp = "Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n';
//                                    //headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
//                                    headerStdStr.append(temp);
//                                    temp.clear();

//                                    //removed for testing writing entire string once to file ================
//                                    //Out << headerStr.toStdString(); //since done, then write to file
//                                    //=======================================================================

//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
//                                    headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                }
//                            }

//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;

//                            ::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
//                                //outputStdStr.append(QString::number(ts)+',');
//                                //outputStdStr.append(QString::number(tf)+',');

//   //                             outputStdStr.append(std::to_string(ts)+','); //CM 7/10/15 - using std::string
//   //                             outputStdStr.append(std::to_string(tf)+',');
//                            }

//                            //add current sample to string to be written to file
//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            if(specificChannel == activeChannels.size()-1){
//                                //outputStdStr.append(QString::number(d));
//                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{
//                                //outputStdStr.append(QString::number(d)+',');
//                                outputStdStr.append(std::to_string(d)+','); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }
//                     //otherwise, pull samples from all channels, despite if they have sensors connected to them
//                     else{
//                        //iterate through all channels (active and inactive) and pull the data
//                         for(int channel = 0; channel < numPhysicalChannels; channel++)
//                        {
//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                //if the last channel, leave off the comma
//                                //QString currentChannelID = channelIDs->at(channel);
//                                std::string currentChannelID = channelIDs->at(channel); //CM 7/10/15 - using std::string
//                                if(channel == numPhysicalChannels-1){
//                                    headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
////                                    Out << headerStr.toStdString(); //since done, then write to file
//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
//                                    headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                }
//                            }
//                            //================================================================================================================
//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;//+(record*data_count);

//                            ::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
//                                //outputStdStr.append(QString::number(ts)+',');
//                                //outputStdStr.append(QString::number(tf)+',');

//    //                            outputStdStr.append(std::to_string(ts)+','); //CM 7/10/15 - using std::string
//    //                            outputStdStr.append(std::to_string(tf)+',');
//                            }


//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            //if(channel == numChannels-1){
//                            if(channel == numPhysicalChannels-1){

//                                //outputStdStr.append(QString::number(d));
//                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{

//                                //outputStdStr.append(QString::number(d)+',');
//                                outputStdStr.append(std::to_string(d)+','); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }


//                    outputStdStr.append("\n"); //signal end of line once an entry in a record for all channels is recorded
//                }

//            }

//            //===========================================================================================================


//            //debugging - check to see how many times the loops above are actually being called
//            if(DEBUG_MODE == 1){
//                 std::cout<<"Safe loop count: "<<sensorItLoopCount<<" Record Entry Loop Count: "<<recordEntryLoopCount<< " Record Size Loop Count: "<<recordSizeLoopCount<<" Channel Loop Count: "<<channelLoopCount<<std::endl;
//                 recordEntryLoopCount = 0; //reset
//                 channelLoopCount = 0;
//                 recordSizeLoopCount = 0;
//            }

//        }

//        Out.close();

//        //CM 7/15/15 - testing time required for iterating and storing data into a string ============
//        duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//        std::cout<<"Time to iterate over data and store sensor data into string: "<<duration<<std::endl;

//        //std::cout<<"Time to iterate over data and store sensor data into string: "<<durationSum<<std::endl;
//        //============================================================================================

//        //==========================================================================================
//        //CM 7/10/15 - I do not need to display this info as it is a performace hit
//        /*
//        if(DISPLAY_FILE_IO_INFO == 1){
//            int numLines = outputStdStr.count('\n');
//            std::cout<<numLines<<" lines written to file for "<<sensorData->size()<<" records."<<std::endl;
//        }
//        */
//        //==========================================================================================
//        //remove for testing ===================
//        /*
//        Out << outputStr.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr.clear();
//        Out.close();
//        */
////======================================

//        //testing=========================
//        /*
//        Out2 << outputStr2.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr2.clear();
//        Out2.close();

//        Out3 << outputStr3.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr3.clear();
//        Out3.close();
//        */
//        //================================

//        outputCount++; //CM 3/18/15 - increment for next write to file
//    }
//    catch (...){
//       //for now, just output an error message indicating where the error occured
//       std::cout<<"Exception caught: CSV_Output class, wrtieDaqDataToFile."<<std::endl;
//    }

//}

//Write sensor data to file
//CM 5/26/15 - use record structure and safe array storage for writing to file
/*Parameters:
 *  sensorData:         Data returned from the DAQ. This is a QList of Sensor_Data object pointers.
 *  recordSize:         The number of samples in each record
 *  sampleRate:         The number of samples per second
 *  channelIDs:         List of IDs for each channel, format: 2!CH7 --> card in slot 2, channel 7 on that card
 *  numPhysicalChannels The number of physical channels on the DAQ
 *  activeChannels:     List of channels that have sensors connected to them. Firs parameter is the index of the channel in the list of channels on the DAQ.
 *                          The second parameter is the channel ID
 *  daqID                The unique identifier for the daq that the passed in sensor data originated.
 */
//void CSV_Output::writeDaqDataToFile_standard_string_orig(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, QList< QPair<int,std::string> > activeChannels, std::string daqID){
//    std::string fileName = "";
////    QString outputStr = ""; //CM 5/18/15 - the string to be written to the file after pulling data from the records
////    QString headerStr = ""; //CM 5/19/15 - the string for the header of a CSV file that contains the channel IDs


//   int currentWriteTime = int(currentTimeSeconds()); //CM 4/14/15 - get the current (now) time for comparison to the last write time

//   //debugging - see time for writing to file ====================
//   //std::cout<<"Before time check. Current time: "<<currentWriteTime<<" Last write time: "<<lastWriteTime<<std::endl;
//   //=============================================================

//   //CM 4/14/15 - if reached the time threshold, then close then create a new file to write to
//   if(currentWriteTime-lastWriteTime >= timeThreshold){
//       lastWriteTime = currentWriteTime;

//       QString dateTime = currentDateTime(); //CM 4/16/15 - testing string formate of date and time

//       //testing=====================
//       //CM 7/15/15 - time writing to file
//       clock_t start;
//       start = std::clock();
//       double duration = 0.0;

//       std::ofstream Out(currentFileName, std::ios::app);
//       Out << headerStdStr;
//       Out << outputStdStr; //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//       std::cout<<"Time to write file before string clear: "<<duration<<std::endl;
//       headerStdStr.clear();
//       outputStdStr.clear();
//       Out.close();
//       duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//       std::cout<<"Time to write file after string clear: "<<duration<<std::endl;
//       //============================

//       //CM 4/16/15 - move completed file to another directory
//       std::string moveCommand = "move " + currentFileName + " " + moveDir;

//       system(moveCommand.c_str()); //move recent finished file

//       //perform time benchmark testing for processing each file
//       double timeForCollection = 0.0;
//       timeForCollection = endTimer();
//       std::cout<<"-------------------------------------------------"<<std::endl;
//       std::cout<<"Collection/Write cycle time lapse: "<<timeForCollection<<std::endl;
//       std::cout<<"-------------------------------------------------"<<std::endl;

// //      currentFileName = daqID+"_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       currentFileName = "daq_data_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       headerWritten = false; //CM 5/19/15 - with the new file, signal the header needs to be written

//       //only print info when wanting more details about what the collection system is doint
//       if(DETAILED_OUPUT == 1){
//           std::cout<<"Number of samples written: "<<numPoints<<" over "<<timeThreshold<<" seconds"<<std::endl
//                    <<'\t'<<"Number of samples written = sample rate * time slice for collecting (in seconds) * number of channels ==>"<<std::endl
//                    <<"\t \t"<<numPoints<<" = "<<sampleRate<<" * "<<timeThreshold<<" * "<<numPhysicalChannels<<std::endl
//                    <<std::endl;
//       }
//       numPoints = 0;
//   }

//   std::vector<Sensor_Data*>::iterator sensorIt; //iterator through the list of sensor objects representing samples returned from the DAQ

//    try{

//        //CM 5/27/15 - update current file name to include the DAQ ID when this function is called for the very first time
//        //      This is analogous to detecting the beginning of a write to the file so the channel ID header can be written
//        //      only once to the file.
//        if(initializeFileName == false){
//  //          currentFileName = daqID+"_"+currentFileName;
//            initializeFileName = true;
//        }

//        //open file for writing, append to end of file
//        //remove for testing ==========
//        //std::ofstream Out(currentFileName, std::ios::app);
//        //=============================

//        //testing================
//        /*
//        QString outputStr2 = "";
//        QString outputStr3 = "";
//         std::ofstream Out2(currentFileName+"_2", std::ios::app);
//         std::ofstream Out3(currentFileName+"_3", std::ios::app);
//         */
//        //=======================

//        if (DEBUG_MODE == 1){
//            std::cout << "Writing data..." << std::endl;
//        }

//        //initialize loop count variables for debugging. They  need to be declared outside of a check for debug mode is on/off
//        //=======================
//        int sensorItLoopCount = 0;
//        int recordEntryLoopCount = 0;
//        int recordSizeLoopCount = 0;
//        int channelLoopCount = 0;
//        //=======================


//        //CM 7/15/15 - testing time required for iterating and storing data into a string ============
//        clock_t start;
//        start = std::clock();
//        double duration = 0.0;
//        //============================================================================================

//        //Iterate through each sensor object and pull out the data
//        for(sensorIt = sensorData->begin(); sensorIt != sensorData->end();sensorIt++){

//            //keep track of the number of sensor loops
//            if(DEBUG_MODE == 1){
//                sensorItLoopCount++;
//            }



//            //====== testing just writing straight from safearrays without indexing  for specific channels ============
//            /*
//            double dd = 0;
//            Sensor_Data* sensorData = *sensorIt;
//            for(int y = 0; y < sensorData->getData()->rgsabound->cElements; y++){
//                ::SafeArrayGetElement(sensorData->getData(), (LONG *)&y, &dd);
//                outputStr.append(QString::number(dd)+"2345678901v2345678"+'\n');
//                //outputStr2.append(QString::number(dd)+'\n');
//                //outputStr3.append(QString::number(dd)+'\n');
//            }

//            continue;
//            */
//            //========================================================================================================


//            Sensor_Data* currentSensorData = NULL;
//            currentSensorData = *sensorIt; //grab the current sensor data object

//            //get the number of samples in the sample data

//            double d = 0.0; //hold each sample as the sensor data is iterated, once saved, this variable is
//                            //  set to the next sample from the sensor
//            double ts = 0.0;//CM 5/26/15 - store the time second timestamp for the sample
//            double tf = 0.0; //CM 5/26/15 - store the time fraction timestamp for the sample

//            //CM 5/27/15 - since the timestamp is the same for each channel for each record (and only processing one record at a time),
//            //      Then just grab the timestamp of the first entry, i.e., index = 0
//            int timeIndex = 0;
//            ::SafeArrayGetElement(currentSensorData->getTimeSeconds(), (LONG *)&timeIndex, &ts);
//            ::SafeArrayGetElement(currentSensorData->getTimeFraction(), (LONG *)&timeIndex, &tf);

//            //Now to iterate through the sensor data. This is completed by "jumping" through the linear stream of data
//            //  provided by the DAQ for each channel for each record. Even though I am not accessing it in linear fashion,
//            //  I only see one sample once, which leads to linear performance.
//            //  Format of linear stream:
//            //      For record of size 4 and 3 channels:
//            //              ------------------------------ record 1 ------------------------------ ----- record 2----------
//            //              [[channel 1 ; 4 samples][channel 2 ; 4 samples][channel 3 ; 4 samples][channel 1 ; 4 samples]...]

//           //iterate over all the available records
//            for (int record = 0; record < currentSensorData->getNumRecords(); record++)
//            {

//                if(DEBUG_MODE == 1){
//                    recordEntryLoopCount++;
//                }

//                //iterate through each record, i.e., recordSize is the number of samples in a record,
//                //  so if the record size is 20, then 20 samples are read for each channel for that specific record
//                for (size_t i = 0; i < recordSize; i++)
//                {
//                    if(DEBUG_MODE == 1){
//                        recordSizeLoopCount++;
//                    }

//                     //Only pull daata from channels with sensors
//                     if(ALL_CHANNEL_DATA == 0){
//                         int channel = 0; //specify which channel data to get
//                         //iterate through all the active channels
//                        for(int specificChannel = 0; specificChannel < activeChannels.size();specificChannel++){
//                            channel = activeChannels.at(specificChannel).first;

//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                std::string currentChannelID = channelIDs->at(channel);
//                                //if the last channel, leave off the comma
//                                if(specificChannel == activeChannels.size()-1){
//                                    std::string temp = "Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n';
//                                    //headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
//                                    headerStdStr.append(temp);
//                                    temp.clear();

//                                    //removed for testing writing entire string once to file ================
//                                    //Out << headerStr.toStdString(); //since done, then write to file
//                                    //=======================================================================

//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
//                                    headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                }
//                            }

//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;

//                            ::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
//                                //outputStdStr.append(QString::number(ts)+',');
//                                //outputStdStr.append(QString::number(tf)+',');

//                                outputStdStr.append(std::to_string(ts)+','); //CM 7/10/15 - using std::string
//                                outputStdStr.append(std::to_string(tf)+',');
//                            }

//                            //add current sample to string to be written to file
//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            if(specificChannel == activeChannels.size()-1){
//                                //outputStdStr.append(QString::number(d));
//                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{
//                                //outputStdStr.append(QString::number(d)+',');
//                                outputStdStr.append(std::to_string(d)+','); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }
//                     //otherwise, pull samples from all channels, despite if they have sensors connected to them
//                     else{
//                        //iterate through all channels (active and inactive) and pull the data
//                         for(int channel = 0; channel < numPhysicalChannels; channel++)
//                        {
//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                //if the last channel, leave off the comma
//                                //QString currentChannelID = channelIDs->at(channel);
//                                std::string currentChannelID = channelIDs->at(channel); //CM 7/10/15 - using std::string
//                                if(channel == numPhysicalChannels-1){
//                                    headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
////                                    Out << headerStr.toStdString(); //since done, then write to file
//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
//                                    headerStdStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                }
//                            }
//                            //================================================================================================================
//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;//+(record*data_count);

//                            ::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
//                                //outputStdStr.append(QString::number(ts)+',');
//                                //outputStdStr.append(QString::number(tf)+',');

//                                outputStdStr.append(std::to_string(ts)+','); //CM 7/10/15 - using std::string
//                                outputStdStr.append(std::to_string(tf)+',');
//                            }


//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            //if(channel == numChannels-1){
//                            if(channel == numPhysicalChannels-1){

//                                //outputStdStr.append(QString::number(d));
//                                outputStdStr.append(std::to_string(d)); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{

//                                //outputStdStr.append(QString::number(d)+',');
//                                outputStdStr.append(std::to_string(d)+','); //CM 7/10/15 - using std::string
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }


//                    outputStdStr.append("\n"); //signal end of line once an entry in a record for all channels is recorded
//                }

//            }

//            //===========================================================================================================


//            //debugging - check to see how many times the loops above are actually being called
//            if(DEBUG_MODE == 1){
//                 std::cout<<"Safe loop count: "<<sensorItLoopCount<<" Record Entry Loop Count: "<<recordEntryLoopCount<< " Record Size Loop Count: "<<recordSizeLoopCount<<" Channel Loop Count: "<<channelLoopCount<<std::endl;
//                 recordEntryLoopCount = 0; //reset
//                 channelLoopCount = 0;
//                 recordSizeLoopCount = 0;
//            }

//        }

//        //CM 7/15/15 - testing time required for iterating and storing data into a string ============
//        duration = (std::clock()-start) / (double) CLOCKS_PER_SEC;
//        std::cout<<"Time to iterate over data and store sensor data into string: "<<duration<<std::endl;
//        //============================================================================================

//        //==========================================================================================
//        //CM 7/10/15 - I do not need to display this info as it is a performace hit
//        /*
//        if(DISPLAY_FILE_IO_INFO == 1){
//            int numLines = outputStdStr.count('\n');
//            std::cout<<numLines<<" lines written to file for "<<sensorData->size()<<" records."<<std::endl;
//        }
//        */
//        //==========================================================================================
//        //remove for testing ===================
//        /*
//        Out << outputStr.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr.clear();
//        Out.close();
//        */
////======================================

//        //testing=========================
//        /*
//        Out2 << outputStr2.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr2.clear();
//        Out2.close();

//        Out3 << outputStr3.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr3.clear();
//        Out3.close();
//        */
//        //================================

//        outputCount++; //CM 3/18/15 - increment for next write to file
//    }
//    catch (...){
//       //for now, just output an error message indicating where the error occured
//       std::cout<<"Exception caught: CSV_Output class, wrtieDaqDataToFile."<<std::endl;
//    }

//}



//void CSV_Output::writeDaqDataToFile_tried_standard_string(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, QStringList* channelIDs, int numPhysicalChannels, QList< QPair<int,QString> > activeChannels, std::string daqID){
//    std::string fileName = "";
////    QString outputStr = ""; //CM 5/18/15 - the string to be written to the file after pulling data from the records
////    QString headerStr = ""; //CM 5/19/15 - the string for the header of a CSV file that contains the channel IDs


//   int currentWriteTime = int(currentTimeSeconds()); //CM 4/14/15 - get the current (now) time for comparison to the last write time

//   //CM 4/14/15 - if reached the time threshold, then close then create a new file to write to
//   if(currentWriteTime-lastWriteTime >= timeThreshold){
//       lastWriteTime = currentWriteTime;

//       QString dateTime = currentDateTime(); //CM 4/16/15 - testing string formate of date and time

//       //testing=====================
//       std::ofstream Out(currentFileName, std::ios::app);
//       Out << headerStr;
//       Out << outputStr; //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//       headerStr.clear();
//       outputStr.clear();
//       Out.close();
//       //============================

//       //CM 4/16/15 - move completed file to another directory
//       std::string moveCommand = "move " + currentFileName + " " + moveDir;

//       system(moveCommand.c_str()); //move recent finished file

//       //perform time benchmark testing for processing each file
//       double timeForCollection = 0.0;
//       timeForCollection = endTimer();
//       std::cout<<"-------------------------------------------------"<<std::endl;
//       std::cout<<"Collection/Write cycle time lapse: "<<timeForCollection<<std::endl;
//       std::cout<<"-------------------------------------------------"<<std::endl;

// //      currentFileName = daqID+"_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       currentFileName = "daq_data_"+dateTime.toStdString()+"_output.txt"; //CM 4/14/15 - create the new output file based on the current time
//       headerWritten = false; //CM 5/19/15 - with the new file, signal the header needs to be written

//       //only print info when wanting more details about what the collection system is doint
//       if(DETAILED_OUPUT == 1){
//           std::cout<<"Number of samples written: "<<numPoints<<" over "<<timeThreshold<<" seconds"<<std::endl
//                    <<'\t'<<"Number of samples written = sample rate * time slice for collecting (in seconds) * number of channels ==>"<<std::endl
//                    <<"\t \t"<<numPoints<<" = "<<sampleRate<<" * "<<timeThreshold<<" * "<<numPhysicalChannels<<std::endl
//                    <<std::endl;
//       }
//       numPoints = 0;
//   }

//   std::vector<Sensor_Data*>::iterator sensorIt; //iterator through the list of sensor objects representing samples returned from the DAQ

//    try{

//        //CM 5/27/15 - update current file name to include the DAQ ID when this function is called for the very first time
//        //      This is analogous to detecting the beginning of a write to the file so the channel ID header can be written
//        //      only once to the file.
//        if(initializeFileName == false){
//  //          currentFileName = daqID+"_"+currentFileName;
//            initializeFileName = true;
//        }

//        //open file for writing, append to end of file
//        //remove for testing ==========
//        //std::ofstream Out(currentFileName, std::ios::app);
//        //=============================

//        //testing================
//        /*
//        QString outputStr2 = "";
//        QString outputStr3 = "";
//         std::ofstream Out2(currentFileName+"_2", std::ios::app);
//         std::ofstream Out3(currentFileName+"_3", std::ios::app);
//         */
//        //=======================

//        if (DEBUG_MODE == 1){
//            std::cout << "Writing data..." << std::endl;
//        }

//        //initialize loop count variables for debugging. They  need to be declared outside of a check for debug mode is on/off
//        //=======================
//        int sensorItLoopCount = 0;
//        int recordEntryLoopCount = 0;
//        int recordSizeLoopCount = 0;
//        int channelLoopCount = 0;
//        //=======================


//        //Iterate through each sensor object and pull out the data
//        for(sensorIt = sensorData->begin(); sensorIt != sensorData->end();sensorIt++){

//            //keep track of the number of sensor loops
//            if(DEBUG_MODE == 1){
//                sensorItLoopCount++;
//            }



//            //====== testing just writing straight from safearrays without indexing  for specific channels ============
//            /*
//            double dd = 0;
//            Sensor_Data* sensorData = *sensorIt;
//            for(int y = 0; y < sensorData->getData()->rgsabound->cElements; y++){
//                ::SafeArrayGetElement(sensorData->getData(), (LONG *)&y, &dd);
//                outputStr.append(QString::number(dd)+"2345678901v2345678"+'\n');
//                //outputStr2.append(QString::number(dd)+'\n');
//                //outputStr3.append(QString::number(dd)+'\n');
//            }

//            continue;
//            */
//            //========================================================================================================


//            Sensor_Data* currentSensorData = NULL;
//            currentSensorData = *sensorIt; //grab the current sensor data object

//            //get the number of samples in the sample data

//            double d = 0.0; //hold each sample as the sensor data is iterated, once saved, this variable is
//                            //  set to the next sample from the sensor
//            double ts = 0.0;//CM 5/26/15 - store the time second timestamp for the sample
//            double tf = 0.0; //CM 5/26/15 - store the time fraction timestamp for the sample

//            //CM 5/27/15 - since the timestamp is the same for each channel for each record (and only processing one record at a time),
//            //      Then just grab the timestamp of the first entry, i.e., index = 0
//            int timeIndex = 0;
//            ::SafeArrayGetElement(currentSensorData->getTimeSeconds(), (LONG *)&timeIndex, &ts);
//            ::SafeArrayGetElement(currentSensorData->getTimeFraction(), (LONG *)&timeIndex, &tf);

//            //Now to iterate through the sensor data. This is completed by "jumping" through the linear stream of data
//            //  provided by the DAQ for each channel for each record. Even though I am not accessing it in linear fashion,
//            //  I only see one sample once, which leads to linear performance.
//            //  Format of linear stream:
//            //      For record of size 4 and 3 channels:
//            //              ------------------------------ record 1 ------------------------------ ----- record 2----------
//            //              [[channel 1 - 4 samples][channel 2 - 4 samples][channel 3 - 4 samples][channel 1 - 4 samples]...]

//           //iterate over all the available records
//            for (int record = 0; record < currentSensorData->getNumRecords(); record++)
//            {

//                if(DEBUG_MODE == 1){
//                    recordEntryLoopCount++;
//                }

//                //iterate through each record, i.e., recordSize is the number of samples in a record,
//                //  so if the record size is 20, then 20 samples are read for each channel for that specific record
//                for (size_t i = 0; i < recordSize; i++)
//                {
//                    if(DEBUG_MODE == 1){
//                        recordSizeLoopCount++;
//                    }

//                     //Only pull daata from channels with sensors
//                     if(ALL_CHANNEL_DATA == 0){
//                         int channel = 0; //specify which channel data to get
//                         //iterate through all the active channels
//                        for(int specificChannel = 0; specificChannel < activeChannels.size();specificChannel++){
//                            channel = activeChannels.at(specificChannel).first;

//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                QString currentChannelID = channelIDs->at(channel);

//                                //testing ================
//                                std::string currentChannelIDStr = currentChannelID.toStdString().c_str();
//                                //========================

//                                //if the last channel, leave off the comma
//                                if(specificChannel == activeChannels.size()-1){
////                                    headerStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
//                                    //testing using regular strings and not QStrings ===========
//                                    headerStr.append("Time_Sec_"+currentChannelIDStr+",Time_Frac_"+currentChannelIDStr+",Data_"+currentChannelIDStr+'\n');
//                                    //==========================================================

////                                    Out << headerStr.toStdString(); //since done, then write to file
//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
////                                    headerStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                    //testing using regular strings and not QStrings ===========

//                                    headerStr.append("Time_Sec_"+currentChannelIDStr+",Time_Frac_"+currentChannelIDStr+",Data_"+currentChannelIDStr+',');
//                                    //==========================================================
//                                }
//                            }

//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;

//                            ::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
////                                outputStr.append(QString::number(ts)+',');
////                                outputStr.append(QString::number(tf)+',');

//                                //testing using regular strings and not QStrings ===========
//                                outputStr.append(std::to_string(ts)+',');
//                                outputStr.append(std::to_string(tf)+',');
//                                //==========================================================
//                            }

//                            //add current sample to string to be written to file
//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            if(specificChannel == activeChannels.size()-1){
////                                outputStr.append(QString::number(d));
//                                //testing using regular strings and not QStrings ===========
//                                outputStr.append(std::to_string(d));
//                                //==========================================================
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{
////                                outputStr.append(QString::number(d)+',');
//                                //testing using regular strings and not QStrings ===========
//                                outputStr.append(std::to_string(d)+',');
//                                //==========================================================
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }
//                     //otherwise, pull samples from all channels, despite if they have sensors connected to them
//                     else{
//                        //iterate through all channels (active and inactive) and pull the data
//                         for(int channel = 0; channel < numPhysicalChannels; channel++)
//                        {
//                            if(DEBUG_MODE == 1){
//                                channelLoopCount++;
//                            }

//                            //====== CM 5/19/15 - if no header for the current file, create it on the first pass of the channel info =========
//                            if(headerWritten == false){
//                                //if the last channel, leave off the comma
//                                QString currentChannelID = channelIDs->at(channel);

//                                //testing ================
//                                std::string currentChannelIDStr = currentChannelID.toStdString().c_str();
//                                //========================

//                                if(channel == numPhysicalChannels-1){
////                                    headerStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+'\n');
////                                    Out << headerStr.toStdString(); //since done, then write to file

//                                    //testing using regular strings and not QStrings ===========
//                                    headerStr.append("Time_Sec_"+currentChannelIDStr+",Time_Frac_"+currentChannelIDStr+",Data_"+currentChannelIDStr+'\n');
//                                    //==========================================================

//                                    headerWritten = true; //header is written, do not write one again until the next CSV file
//                                }
//                                //otherwise, put the comma on since more channels are to come
//                                else{
////                                    headerStr.append("Time_Sec_"+currentChannelID+",Time_Frac_"+currentChannelID+",Data_"+currentChannelID+',');
//                                    //testing using regular strings and not QStrings ===========
//                                    headerStr.append("Time_Sec_"+currentChannelIDStr+",Time_Frac_"+currentChannelIDStr+",Data_"+currentChannelIDStr+',');
//                                    //==========================================================
//                                }
//                            }
//                            //================================================================================================================
//                            //index calculation: (the current channel * the size of a record) + the current index in the record
//                            size_t index = (channel*recordSize)+i;//+(record*data_count);

//                            ::SafeArrayGetElement(currentSensorData->getData(), (LONG *)&index, &d);

//                            //CM 5/27/15 - if one does not want to include the timestamp, then turn it off
//                            if(INCLUDE_TIMESTAMP == 1){
////                                outputStr.append(QString::number(ts)+',');
////                                outputStr.append(QString::number(tf)+',');

//                                //testing using regular strings and not QStrings ===========
//                                outputStr.append(std::to_string(ts)+',');
//                                outputStr.append(std::to_string(tf)+',');
//                                //==========================================================
//                            }


//                            //CM 5/19/15 - if at the last channel, do not add a comma
//                            //if(channel == numChannels-1){
//                            if(channel == numPhysicalChannels-1){

////                                outputStr.append(QString::number(d));
//                                //testing using regular strings and not QStrings ===========
//                                outputStr.append(std::to_string(d));
//                                //==========================================================
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                            //Otherwise, still more channels to go, so add a comma
//                            else{

////                                outputStr.append(QString::number(d)+',');
//                                //testing using regular strings and not QStrings ===========
//                                outputStr.append(std::to_string(d)+',');
//                                //==========================================================
//                                if(DETAILED_OUPUT == 1){
//                                    numPoints++;
//                                }
//                            }
//                        }
//                     }


////                    outputStr.append('\n'); //signal end of line once an entry in a record for all channels is recorded
//                     //testing using regular strings and not QStrings ===========
//                     outputStr.append("\n"); //signal end of line once an entry in a record for all channels is recorded
//                     //==========================================================
//                }

//            }

//            //===========================================================================================================


//            //debugging - check to see how many times the loops above are actually being called
//            if(DEBUG_MODE == 1){
//                 std::cout<<"Safe loop count: "<<sensorItLoopCount<<" Record Entry Loop Count: "<<recordEntryLoopCount<< " Record Size Loop Count: "<<recordSizeLoopCount<<" Channel Loop Count: "<<channelLoopCount<<std::endl;
//                 recordEntryLoopCount = 0; //reset
//                 channelLoopCount = 0;
//                 recordSizeLoopCount = 0;
//            }

//        }
//        //removed for testing using regular strings and not QStrings ===========
//        /*
//        if(DISPLAY_FILE_IO_INFO == 1){
//            int numLines = outputStr.count('\n');

//            std::cout<<numLines<<" lines written to file for "<<sensorData->size()<<" records."<<std::endl;
//        }
//        */
//        //======================================================================

////remove for testing ===================
//        /*
//        Out << outputStr.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr.clear();
//        Out.close();
//        */
////======================================

//        //testing=========================
//        /*
//        Out2 << outputStr2.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr2.clear();
//        Out2.close();

//        Out3 << outputStr3.toStdString(); //CM 5/27/15 - write the entire set of record(s) info to the file at once (i.e., only one file I/O)
//        outputStr3.clear();
//        Out3.close();
//        */
//        //================================

//        outputCount++; //CM 3/18/15 - increment for next write to file
//    }
//    catch (...){
//       //for now, just output an error message indicating where the error occured
//       std::cout<<"Exception caught: CSV_Output class, wrtieDaqDataToFile."<<std::endl;
//    }

//}


