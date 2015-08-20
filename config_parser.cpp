#include <iostream>
#include <fstream>

#include "config_parser.h"
#include "library.h"

//constructor
Config_Parser::Config_Parser(int clockF, int p, int sampR, int recordS)
{

    clockFrequency = clockF;
    prescaler = p;
    sampleRate = sampR;
    recordSize = recordS;

    fileNameStr = "daq_config.txt";
    movePathStr = ""; //set to empty as the user must specify in config file

    //set to default values in case reading the config file cannot be read
    ALL_CHANNEL_DATA = 1;
    DEFAULT_BUFFER_SIZE =  20;
    DEBUG_MODE = 0;
    DETAILED_OUPUT = 0;
    INCLUDE_TIMESTAMP = 1;
    CHECK_RECORDS_ON_DAQ = 1;
    DISPLAY_FILE_IO_INFO = 0;

}

//deconstructor
Config_Parser::~Config_Parser()
{
    //nothing right now
}


 //CM 8/10/15 - update to use standard string instead of QString
 //Take the file name of the config file and parse it
 void Config_Parser::parseConfigFileStr(string fName){
    fileNameStr = fName; //store the passed in config file name

    cout<<"Opening configuration file: "<<fileNameStr<<endl;


    //Use regular C++ style file I/O
    //open an input file stream to the file
    ifstream In(fileNameStr.c_str());

    if(In.good()){
        cout<<endl;
        cout<<"File stream for config file is good --> opened succesfully. Continuing with loading parameters from config file..."<<endl;
        cout<<endl;
    }
    else{
        cout<<endl;
        cout<<"File stream for config file is bad --> file could not be opened!!! Reverting to default values..."<<endl;
        cout<<endl;
        return;
    }

    string currentLine;
    getline(In,currentLine,'\n'); //get an entire line

    //loop through the file while not at the end and pull out the config information
    while(!In.eof()){

        //if a comment marker is found or an empty line, then ignore and move on
        //if(currentLine.count('#') > 0 || currentLine.isEmpty()){
        //look to see if comment marker ('#') is found or the line is empty.
        //      NOTE: find() returns npos if no match is found
        if(currentLine.find("#") != string::npos || currentLine.empty() == 1){

            //move on to next line
            getline(In,currentLine,'\n'); //get an entire line
            continue;
        }
        //if the line contains "a daq "DAQ", then pull out all the info and stop reading the file
        //  This will work as any commented lines that may have "DAQ" in it, will be ignored by the above if-clause
        else if(currentLine.find("DAQ") != string::npos){

            //CM 8/10/15 - for each line in the config file that contains parameters, there is an ID and a value: E.g, 2;1600\n
            string parameterID = "";
            string parameterValue = "";


                //NOTE: for now, just assuming a certain order of the parameters in the file.
                //  I am NOT using the number scheme yet!!!

                //grab clock frequency line

                //CM 8/10/15 - each line has two parts (E.g, 2;1600\n) so read in until reach ';', then until a '\n' is found
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value

                clockFrequency = atoi(parameterValue.c_str()); //convert parameter value to double (truncate to int)

                //grab prescaler line
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                prescaler = atoi(parameterValue.c_str()); //save it

                //grab sample rate line
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                sampleRate =  atof(parameterValue.c_str()); //save it

                //grab seconds per file line
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                secondsPerFile = atoi(parameterValue.c_str());; //save it

                //grab move path line
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                movePathStr = parameterValue; //save it

                //grab store all sensors line
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                storeAllChannelData = to_bool(parameterValue);

                //grab store check records on daqs line
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                checkRecordsOnDaq = to_bool(parameterValue);

                //display the config info parsed from the config file
                cout<<"Parameters from the config file:"
                    <<endl
                    <<"Clock Frequency: "<<clockFrequency<<endl
                    <<"Prescaler: "<<prescaler<<endl
                    <<"Sample Rate: "<<sampleRate<<endl
                    <<"Length of file (in seconds): "<<secondsPerFile<<endl
                    <<"Move Path: "<<movePathStr<<endl
                    <<"Store all sensor data: "<<storeAllChannelData<<endl
                    <<"Check records on DAQs: "<<checkRecordsOnDaq<<endl
                    <<endl;
                break;
        }

        getline(In,currentLine,'\n'); //get an entire line
    }

    In.close(); //close the file handle to the config file

    return;
 }


//all the setters and getters for the specified DAQ parameters

int Config_Parser::getClockFrequency(){
    return clockFrequency;
}

int Config_Parser::getPrescaler(){
    return prescaler;
}

double Config_Parser::getSampleRate(){
    return sampleRate;
}

int Config_Parser::getSecondsPerFile(){
    return secondsPerFile;
}

int Config_Parser::getRecordSize(){
    return recordSize;
}

string Config_Parser::getMovePath(){
     return movePathStr;
 }

int Config_Parser::getRecordsPerWrite(){
    return recordsPerWrite;
}

bool Config_Parser::getStoreAllChannelData(){
    return storeAllChannelData;
}

bool Config_Parser::getCheckRecordsOnDaq(){
    return checkRecordsOnDaq;
}

void Config_Parser::setClockFrequency(int cf){
    clockFrequency = cf;
}

void Config_Parser::setPrescaler(int p){
    prescaler = p;
}

void Config_Parser::setSampleRate(double sr){
    sampleRate = sr;
}

void Config_Parser::setRecordSize(int rs){
    recordSize = rs;
}

void Config_Parser::setSecondsPerFile(int spf){
    secondsPerFile = spf;
}

void Config_Parser::setMovePath(string mvPath){
    movePathStr = mvPath;
}

void Config_Parser::setRecordsPerWrite(int rw){
    recordsPerWrite = rw;
}

void Config_Parser::setStoreAllChannelData(bool sc){
    storeAllChannelData = sc;
}

void Config_Parser::setCheckRecordsOnDaq(bool cr){
    checkRecordsOnDaq = cr;
}
