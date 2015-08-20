#ifndef LIBRARY
#define LIBRARY


#include <sstream>
#include <iostream>
#include <fstream>


#ifndef TIMER_VARS
#define TIMER_VARS

extern time_t startTime,endTime; //Global timing variables for clocking user specified sections of code.
                                 //     NOTE: see note below about current usage.
extern int ALL_CHANNEL_DATA; //Pull all the samples on all channels despite whether or not a channel has a connected sensor
                             //   Value = 0: only pull sensor data from channels with sensors
                             //   Value = 1: pull all sensor data

extern int DEFAULT_BUFFER_SIZE; //default number of records to buffer before writing to disk

extern int DEBUG_MODE;          //Set debug mode (value = 1) or turn off (value = 0), turned on, extra info will be printed

extern int DETAILED_OUPUT;      //Provides detailed output about operations being performed by the collection system
                                //   Value = 0: do not show detailed output
                                //   Value = 1: show detailed output

extern int INCLUDE_TIMESTAMP;   //Toggle whether the timestamp is written to the CSV file, value 0: do not include, valule 1: include it
                                //  NOTE: This flag is no longer used as recording and storage of the timestamp has been changed.
                                //  Use of this can be removed.

extern int CHECK_RECORDS_ON_DAQ;    //Check to see if the DAQ is filling up and records are not being pulled fast enough
                                    //  Value = 0: do not show when records are being back-logged
                                    //  Value = 1: show when records are being back-logged

extern int DISPLAY_FILE_IO_INFO;    //Display information pertaining to data being written to file, e.g., if CSV, how many lines are written
                                    //   per X records collected; value 1: display file I/O info, value 0: do not display file I/O info

extern int PROGRESS_LINE_LENGTH; //CM 8/18/15 - during each collection phase, print out progress feedback (i.e., how much progress has passed)
                                 //              this variable sets the length of the progress bar (so no matter how long a collection cycle
                                 //              is, it will always take the same space. The longer the cycle, the slower the progress will show

#endif

enum OutputType
{
    CSV,HDF5,No_Type
};


//CM 4/14/15 - enum to specify the format of the data after collected from the DAQ.
//  Segmented_Channel - the sensor data has be parsed into separate channels
//  RAW_Record - the sensor data has not been parsed into separate channels and is stored
//      in the Record format (format returned by the DAQ)
enum DataFormat
{
    Segmented_Channel,RAW_Record
};

const double currentTimeSeconds(); //CM 4/14/15 - current timestamp in seconds
const std::string currentDateTime(); //CM 8/10/15 - current timestamp using standard string


//NOTE: I have not gotten the global start and end timers to work properly. I can get local ones to work just fine.
void startTimer(); //CM 5/18/15 - start global timer for timing how long operations take
double endTimer(); //CM 5/18/15 - end global timer for timing how long operations take

//to_bool function courteousy of Chris Jester-Young at: http://stackoverflow.com/questions/2165921/converting-from-a-stdstring-to-bool
inline bool to_bool(std::string const& s) {
    return s != "0";
}


#endif // LIBRARY

