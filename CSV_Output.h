#pragma once
#include <map>
#include <list>
//#include <QStringList>
//#include <QPair>
#include "Data_Output.h"
#include "sensor_data.h"


//This class writes sensor data to a CSV file and moves the file to a specified location once a file is complete.
//  The sensor data is split-up in mutiple files each a specific time length as specified in the config file.
//  Default length is 1 minute (60 seconds);
class CSV_Output : public Data_Output {

public:
	CSV_Output();
	~CSV_Output();


    //CM 5/18/15 - pass necessary information to write sensor data to CSV file
    //void writeDaqDataToFile(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, QStringList* channelIDs, int numPhysicalChannels, QList<QPair<int, QString> > activeChannels,std::string daqID);
    //void writeDaqDataToFile_standard_string(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, QList<QPair<int, std::string> > activeChannels,std::string daqID);
    //void writeDaqDataToFile_standard_string_orig(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, QList<QPair<int, std::string> > activeChannels,std::string daqID);
   // void writeDaqDataToFile_vector(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, QList<QPair<int, std::string> > activeChannels,std::string daqID);
    void writeDaqDataToFile_standard_string_remove_qt(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, std::list< std::pair<int,std::string> > activeChannels, std::string daqID);

    std::string getMoveDir(); //get the move directory
    void setMoveDir(std::string mvDir); //set the move directory

    int getfileLength();        //get the file length (in seconds)
    void setfileLength(int fl); //set the file length (in seconds)

private:
    int outputCount;    //CM 3/18/15 - testing - keep track of the number of times a write to file is called
    int lastWriteTime;  //CM 4/14/15 - record the time of the last file created.
                        //    This is used to control the record time of the data. i.e., only want to record
                        //    in 15 minutes, so for every 15 minutes, create a new file

    int fileLength;  //CM 8/12/15 - The length of each file created in seconds
    bool headerWritten; //CM 5/19/15 - bool to  flag when the header is or is not written to the current CSV file
                        //  False: when not written, True: when written
                        //  This is used to detect when a new CSV file is created, then the header is written

    bool initializeFileName; //CM 5/27/15 - when the file to be written to is first created, need to add the DAQ ID
                             //  but this is only necessary for the very first call to writing to file, i.e., this is
                             //  only done once after the program begins and the CSV_Output object is created, and
                             //  the write to file is called for the first time for that object.
                             //This is analogous to detecting the beginning of a write to the file so the channel ID header can be written
                             //  only once to the file.


    int numPoints; //used for debugging to see how many points are being written to a file

    std::string currentFileName;    //CM 4/14/15 - the current open file that is being written to
    std::string moveDir;            //CM 4/16/15 - the directory to move output files once writing is complete for a certain file
    std::string workingDir;         //CM 4/16/15 - the directory holding the current output files to be moved to the moveDir

    //QString outputStr;
    //QString headerStr;

    //CM 7/9/15 - changing to use regular strings for speed-up, QString.toStdString() is too slow (by about 5x)
    std::string outputStdStr;
    std::string headerStdStr;

    //CM 8/12/15 - record the timestamp of the first record of each file
    double firstTimeSecond;
    double firstTimeFraction;

    std::vector<double> samples; //CM 7/17/15 - store the sample data in a vector of doubles to make writing to disk faster

    int recordsCollected; //CM 8/6/15 - keep track of the number of records have been collected for the current file

};

