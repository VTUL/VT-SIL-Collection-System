//#include <QString>

#include <string>

using namespace std;

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

//This class reads in a configuration file for DAQs and pulls out the necessary information for a specific DAQ

class Config_Parser
{
public:
     //Config_Parser(int DAQ_ID = 1, int clockF = 204800, int p = 1, int sampR = 1600, int recordS = 160); //default constructor, if no DAQ ID given, then set to 1, i.e., the first DAQ
                                                                                                        //  All other parameters can be specified when object is created, otherwise,
                                                                                                        //  default ones are set
     Config_Parser(int clockF = 204800, int p = 1, int sampR = 1600, int recordS = 160);

    ~Config_Parser();

    //void parseConfigFile(QString fName); //Take the file name of the config file and parse it
    void parseConfigFileStr(string fName); //Take the file name of the config file and parse it

    //int getDaqID();
    int getClockFrequency();
    int getPrescaler();
    double getSampleRate();
    int getSecondsPerFile();
    int getRecordSize();
    string getMovePath();
    int getRecordsPerWrite();
    bool getStoreAllChannelData();
    bool getCheckRecordsOnDaq();

    //void setDaqID(int id);
    void setClockFrequency(int cf);
    void setPrescaler(int p);
    void setSampleRate(double sr);
    void setSecondsPerFile(int spf);
    void setRecordSize(int rs);
    void setMovePath(string mvPath);
    void setRecordsPerWrite(int rw);
    void setStoreAllChannelData(bool sc);
    void setCheckRecordsOnDaq(bool cr);

private:
   //int daqID; //DAQ id, e.g., 1,2,...

    //the desired DAQ parameters to be set
    int clockFrequency;
    int prescaler;
    double sampleRate;
    int secondsPerFile; //CM 8/7/15 - number of seconds of data in each file created
    int recordSize;
    int recordsPerWrite; //number of records to collect before writing to file/disk
    bool storeAllChannelData; //flag to specify if storing data from all channels even if no sensors connected
                                //  value 0: only process channels with sensors
                                //  value 1: process all channels
    bool checkRecordsOnDaq; //Flag to toggle for checking to see if the DAQ is filling up and records are not being pulled fast enough
                            //   value 0: do not check (print out info),
                            //   value 1: print out records currently on DAQ that have not been pulled

    //QString fileName; // the name of the config file being parsed
    //QString movePath; //the path to move sample files locally, i.e., the sensor data collected
                      //    is written to files in defined time chuncks (e.g., 15 minutes),
                      //    these files are moved to the path specified here

    //CM 8/10/15 - change to using standard strings
    string fileNameStr;
    string movePathStr;



};

#endif // CONFIG_PARSER_H
