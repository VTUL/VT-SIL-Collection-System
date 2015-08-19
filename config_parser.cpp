//#include <QFile>
//#include <QTextStream>
#include <iostream>
#include <fstream>

#include "config_parser.h"
#include "library.h"

//constructor
//Config_Parser::Config_Parser(int DAQ_ID, int clockF, int p, int sampR, int recordS)
Config_Parser::Config_Parser(int clockF, int p, int sampR, int recordS)
{

    //daqID = DAQ_ID;

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

 //Take the file name of the config file and parse it
// void Config_Parser::parseConfigFile(QString fName){
//    fileName = fName; //store the passed in config file name

//    cout<<"Opening configuration file: "<<fileName.toStdString()<<endl;


//    //Use regular C++ style file I/O
//    //open an input file stream to the file
//    ifstream In(fileName.toStdString().c_str());

//    if(In.good()){
//        cout<<endl;
//        cout<<"File stream for config file is good --> opened succesfully. Continuing with loading parameters from config file..."<<endl;
//        cout<<endl;
//    }
//    else{
//        cout<<endl;
//        cout<<"File stream for config file is bad --> file could not be opened!!! Reverting to default values..."<<endl;
//        cout<<endl;
//        return;
//    }

//    string line;
//    QString currentLine;
//    getline(In,line,'\n'); //get an entire line
//    currentLine = QString(line.c_str()); //convert to QString for manipulation ease

//    //loop through the file while not at the end and pull out the config information
//    while(!In.eof()){

//        //if a comment marker is found or an empty line, then ignore and move on
//        if(currentLine.count('#') > 0 || currentLine.isEmpty()){
//            //move on to next line
//            getline(In,line,'\n'); //get an entire line
//            currentLine = QString(line.c_str()); //convert to QString for manipulation ease
//            continue;
//        }
//        //if the line contains a daq identifier that matches the ID for daqID, then pull out all the info
//        //and stop reading the file
//        else if(currentLine.count("DAQ")>0){
//            QStringList lineList = currentLine.split(';');

//            //CM 6/10/15 - remove since can connect to multiple DAQs in one session ==================
//            //  I will keep this here in case for future use
//            /*
//            int id = lineList.at(lineList.size()-1).toInt();



//            //if found the config for the daqID, then pull the parameter values
//            if(id == daqID){
//            */
//            //========================================================================================

//                //NOTE: for now, just assuming a certain order of the parameters in the file.
//                //  I am NOT using the number scheme yet!!!

//                //grab clock frequency line
//                getline(In,line,'\n');   //get the line
//                currentLine = QString(line.c_str()); //convert to QString
//                lineList = currentLine.split(';'); //split along ';'
//                int cf = lineList.at(lineList.size()-1).toInt(); //grab the clock frequency
//                clockFrequency = cf; //save it

//                //grab prescaler line
//                getline(In,line,'\n');   //get the line
//                currentLine = QString(line.c_str()); //convert to QString
//                lineList = currentLine.split(';'); //split along ';'
//                int p = lineList.at(lineList.size()-1).toInt(); //grab the prescaler
//                prescaler = p; //save it

//                //grab sample rate line
//                getline(In,line,'\n');   //get the line
//                currentLine = QString(line.c_str()); //convert to QString
//                lineList = currentLine.split(';'); //split along ';'
//                int sr = lineList.at(lineList.size()-1).toInt(); //grab the prescaler
//                sampleRate = sr; //save it

//                //grab seconds per file line
//                getline(In,line,'\n');   //get the line
//                currentLine = QString(line.c_str()); //convert to QString
//                lineList = currentLine.split(';'); //split along ';'
//                int spf = lineList.at(lineList.size()-1).toInt(); //grab the prescaler
//                secondsPerFile = spf; //save it

//                //CM 8/7/15 - remove setting record size from config file - keep in here in case we decide to use it again
//                //grab record size line
//                /*
//                getline(In,line,'\n');   //get the line
//                currentLine = QString(line.c_str()); //convert to QString
//                lineList = currentLine.split(';'); //split along ';'
//                int rs = lineList.at(lineList.size()-1).toInt(); //grab the prescaler
//                recordSize = rs; //save it
//                */

//                //grab move path line
//                getline(In,line,'\n');   //get the line
//                currentLine = QString(line.c_str()); //convert to QString
//                lineList = currentLine.split(';'); //split along ';'
//                QString mvPath = lineList.at(lineList.size()-1); //grab the move path string
//                movePath = mvPath; //save it

//                //CM 8/7/15 - remove setting records per save from config file - keep in here in case we decide to use it again
//                //grab records per write line
//                /*
//                getline(In,line,'\n');   //get the line
//                currentLine = QString(line.c_str()); //convert to QString
//                lineList = currentLine.split(';'); //split along ';'
//                int rw = lineList.at(lineList.size()-1).toInt(); //grab the records per write
//                recordsPerWrite = rw; //save it
//                */

//                //grab store all sensors line
//                getline(In,line,'\n');   //get the line
//                currentLine = QString(line.c_str()); //convert to QString
//                lineList = currentLine.split(';'); //split along ';'
//                bool sc = lineList.at(lineList.size()-1).toInt(); //grab the store all sensors bool value
//                storeAllChannelData = sc; //save it

//                //grab store check records on daqs line
//                getline(In,line,'\n');   //get the line
//                currentLine = QString(line.c_str()); //convert to QString
//                lineList = currentLine.split(';'); //split along ';'
//                bool cr = lineList.at(lineList.size()-1).toInt(); //grab the check records on daqs bool value
//                checkRecordsOnDaq = cr; //save it

//                //display the config info parsed from the config file
//                cout<<"Parameters from the config file:"
//                    <<endl
//                    <<"DAQ ID: "<<daqID<<endl
//                    <<"Clock Frequency: "<<clockFrequency<<endl
//                    <<"Prescaler: "<<prescaler<<endl
//                    <<"Sample Rate: "<<sampleRate<<endl
//                    <<"Length of file (in seconds): "<<secondsPerFile<<endl
//                    <<"Move Path: "<<movePath.toStdString()<<endl
//                    //<<"Records per write: "<<recordsPerWrite<<endl
//                    //<<"Record Size: "<<recordSize<<endl
//                    <<"Store all sensor data: "<<storeAllChannelData<<endl
//                    <<"Check records on DAQs: "<<checkRecordsOnDaq<<endl
//                    <<endl;

//                break;
//            //}
//        }

//        getline(In,line,'\n'); //get an entire line
//        currentLine = QString(line.c_str()); //convert to QString for manipulation ease
//    }

//    In.close(); //close the file handle to the config file

//    return;
// }

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
    //QString currentLine;
    getline(In,currentLine,'\n'); //get an entire line
    //currentLine = QString(line.c_str()); //convert to QString for manipulation ease

    //loop through the file while not at the end and pull out the config information
    while(!In.eof()){

        //debugging =============
        //std::cout<<currentLine<<std::endl;
        //std::cout<<currentLine.front()<<'\t'<<currentLine.empty()<<std::endl;
        //=======================

        //if a comment marker is found or an empty line, then ignore and move on
        //if(currentLine.count('#') > 0 || currentLine.isEmpty()){
        //look to see if comment marker ('#') is found or the line is empty.
        //      NOTE: find() returns npos if no match is found
        if(currentLine.find("#") != string::npos || currentLine.empty() == 1){

            //debugging =============
            //std::cout<<"2"<<std::endl;
            //=======================

            //debugging =============
            //std::cout<<"1 "<<currentLine<<std::endl;
            //=======================

            //move on to next line
            getline(In,currentLine,'\n'); //get an entire line
            //currentLine = QString(line.c_str()); //convert to QString for manipulation ease
            continue;
        }
        //if the line contains a daq identifier that matches the ID for daqID, then pull out all the info
        //and stop reading the file
        //else if(currentLine.find("DAQ")>0){
        else if(currentLine.find("DAQ") != string::npos){
            //QStringList lineList = currentLine.split(';');

            //debugging =============
            //std::cout<<"2 current line: "<<currentLine<<std::endl;
            //std::cout<<"In.peek(): "<<In.peek()<<std::endl;
            //=======================

            //CM 8/10/15 - for each line in the config file that contains parameters, there is an ID and a value: E.g, 2;1600\n
            string parameterID = "";
            string parameterValue = "";

            //===========================================================================
            //CM 8/10/15 - parse the current line before getting the rest of the parameters
            //      NOTE: this only needs to be done for the first parameter since it is currently pulled from the file
            /*
            std::stringstream ss(currentLine); //string stream for the channelID string
            std::vector<std::string> parsedParameters; //vector to hold the parsed pieces of the channelID string

            //iterate through the string stream, split along 'H', which is the letter in the channelID before the channel number, e.g., channedID = 2!CH4, then channel number is 4
            while (ss.good()){
                std::string substr;
                getline(ss, substr, ';');
                parsedParameters.push_back(substr);
            }
            */
            //===========================================================================



            //CM 6/10/15 - remove since can connect to multiple DAQs in one session ==================
            //  I will keep this here in case for future use
            /*
            int id = lineList.at(lineList.size()-1).toInt();



            //if found the config for the daqID, then pull the parameter values
            if(id == daqID){
            */
            //========================================================================================

                //NOTE: for now, just assuming a certain order of the parameters in the file.
                //  I am NOT using the number scheme yet!!!

                //CM 8/10/15 - remove old way with QString and update with using standard string
                //grab clock frequency line
                /*
                getline(In,line,'\n');   //get the line
                currentLine = QString(line.c_str()); //convert to QString
                lineList = currentLine.split(';'); //split along ';'
                int cf = lineList.at(lineList.size()-1).toInt(); //grab the clock frequency
                clockFrequency = cf; //save it
                */
                //CM 8/10/15 - each line has two parts (E.g, 2;1600\n) so read in until reach ';', then until a '\n' is found
                getline(In,parameterID,';');   //get the parameter ID

                //debugging =============
                //std::cout<<"parameterID "<<parameterID<<std::endl;
                //=======================

                getline(In,parameterValue,'\n');   //get the parameter value
                //currentLine = QString(line.c_str()); //convert to QString
                //lineList = currentLine.split(';'); //split along ';'
                //int cf = lineList.at(lineList.size()-1).toInt(); //grab the clock frequency
                //clockFrequency = cf; //save it

                //debugging =============
                //std::cout<<"parameterValue "<<parameterValue<<std::endl;
                //=======================

               // parameterID = parsedParameters.at(0);
               // parameterValue = parsedParameters.at(1);

                //debugging =============
                //std::cout<<parameterID<<std::endl;
                //std::cout<<parameterValue<<std::endl;
                //=======================

                clockFrequency = atoi(parameterValue.c_str()); //convert parameter value to double (truncate to int)

                //debugging =============
                //std::cout<<clockFrequency<<std::endl;
                //=======================

                //grab prescaler line
                //getline(In,line,'\n');   //get the line
                //currentLine = QString(line.c_str()); //convert to QString
                //lineList = currentLine.split(';'); //split along ';'
                //int p = lineList.at(lineList.size()-1).toInt(); //grab the prescaler
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                prescaler = atoi(parameterValue.c_str()); //save it

                //debugging =============
                //std::cout<<prescaler<<std::endl;
                //=======================

                //grab sample rate line
                //getline(In,line,'\n');   //get the line
                //currentLine = QString(line.c_str()); //convert to QString
                //lineList = currentLine.split(';'); //split along ';'
                //int sr = lineList.at(lineList.size()-1).toInt(); //grab the prescaler
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                sampleRate =  atof(parameterValue.c_str()); //save it

                //debugging =============
                //std::cout<<sampleRate<<std::endl;
                //=======================

                //grab seconds per file line
                //getline(In,line,'\n');   //get the line
                //currentLine = QString(line.c_str()); //convert to QString
                //lineList = currentLine.split(';'); //split along ';'
                //int spf = lineList.at(lineList.size()-1).toInt(); //grab the prescaler
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                secondsPerFile = atoi(parameterValue.c_str());; //save it

                //debugging =============
                //std::cout<<secondsPerFile<<std::endl;
                //=======================

                //CM 8/7/15 - remove setting record size from config file - keep in here in case we decide to use it again
                //grab record size line
                /*
                getline(In,line,'\n');   //get the line
                currentLine = QString(line.c_str()); //convert to QString
                lineList = currentLine.split(';'); //split along ';'
                int rs = lineList.at(lineList.size()-1).toInt(); //grab the prescaler
                recordSize = rs; //save it
                */

                //grab move path line
                //getline(In,line,'\n');   //get the line
                //currentLine = QString(line.c_str()); //convert to QString
                //lineList = currentLine.split(';'); //split along ';'
                //QString mvPath = lineList.at(lineList.size()-1); //grab the move path string
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                movePathStr = parameterValue; //save it

                //debugging =============
                //std::cout<<movePathStr<<std::endl;
                //=======================

                //CM 8/7/15 - remove setting records per save from config file - keep in here in case we decide to use it again
                //grab records per write line
                /*
                getline(In,line,'\n');   //get the line
                currentLine = QString(line.c_str()); //convert to QString
                lineList = currentLine.split(';'); //split along ';'
                int rw = lineList.at(lineList.size()-1).toInt(); //grab the records per write
                recordsPerWrite = rw; //save it
                */

                //grab store all sensors line
                //getline(In,line,'\n');   //get the line
                //currentLine = QString(line.c_str()); //convert to QString
                //lineList = currentLine.split(';'); //split along ';'
                //bool sc = lineList.at(lineList.size()-1).toInt(); //grab the store all sensors bool value
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                //istringstream ( parameterValue ) >> storeAllChannelData;
                //storeAllChannelData = bool(parameterValue); //save it
                storeAllChannelData = to_bool(parameterValue);

                //debugging =============
                std::cout<<storeAllChannelData<<std::endl;
                //=======================

                //grab store check records on daqs line
                //getline(In,line,'\n');   //get the line
                //currentLine = QString(line.c_str()); //convert to QString
                //lineList = currentLine.split(';'); //split along ';'
                //bool cr = lineList.at(lineList.size()-1).toInt(); //grab the check records on daqs bool value
                //checkRecordsOnDaq = cr; //save it
                getline(In,parameterID,';');   //get the parameter ID
                getline(In,parameterValue,'\n');   //get the parameter value
                //istringstream ( parameterValue ) >> checkRecordsOnDaq;
                checkRecordsOnDaq = to_bool(parameterValue);

                //debugging =============
                std::cout<<checkRecordsOnDaq<<std::endl;
                //=======================

                //display the config info parsed from the config file
                cout<<"Parameters from the config file:"
                    <<endl
                    //<<"DAQ ID: "<<daqID<<endl
                    <<"Clock Frequency: "<<clockFrequency<<endl
                    <<"Prescaler: "<<prescaler<<endl
                    <<"Sample Rate: "<<sampleRate<<endl
                    <<"Length of file (in seconds): "<<secondsPerFile<<endl
                    <<"Move Path: "<<movePathStr<<endl
                    //<<"Records per write: "<<recordsPerWrite<<endl
                    //<<"Record Size: "<<recordSize<<endl
                    <<"Store all sensor data: "<<storeAllChannelData<<endl
                    <<"Check records on DAQs: "<<checkRecordsOnDaq<<endl
                    <<endl;

                break;
            //}
        }

        getline(In,currentLine,'\n'); //get an entire line
        //currentLine = QString(line.c_str()); //convert to QString for manipulation ease
    }

    In.close(); //close the file handle to the config file

    return;
 }


//all the setters and getters for the specified DAQ parameters

 //get function for parameters read from config file
 //int Config_Parser::getDaqID(){
 //    return daqID;
 //}

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


//set function for parameters read from config file
//void Config_Parser::setDaqID(int id){
//    daqID = id;
//}

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
