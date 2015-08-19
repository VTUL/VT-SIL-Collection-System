#ifndef LIBRARY
#define LIBRARY

//#include <QString>

#include <sstream>
#include <iostream>
#include <fstream>
//#include <tchar.h>

/* testing moving #define macros to extern global variables
#define DEFAULT_BUFFER_SIZE 20 //set default buffer size to 20,000 (2000 for testing)
#define DEBUG_MODE 0 //Set debug mode (value = 1) or turn off (value = 0)
#define DETAILED_OUPUT 0 //provides detailed output about operations being performed by the collection system
//#define ALL_CHANNEL_DATA 0 //pull all the samples on all channels despite whether or not a channel has a connected sensor
                           //   Value = 0: only pull sensor data from channels with sensors
                           //   Value = 1: pull all sensor data
#define INCLUDE_TIMESTAMP 1 //Toggle whether the timestamp is written to the CSV file, value 0: do not include, valule 1: include it
#define CHECK_RECORDS_ON_DAQ 1 //Check to see if the DAQ is filling up and records are not being pulled fast enough
                               //   value 0: do not check (print out info), value 1: print out records currently on DAQ that have not been pulled
#define DISPLAY_FILE_IO_INFO 0 //Display information pertaining to data being written to file, e.g., if CSV, how many lines are written
                               //   per X records collected; value 1: display file I/O info, value 0: do not display file I/O info
*/

#ifndef TIMER_VARS
#define TIMER_VARS

extern time_t startTime,endTime;
extern int ALL_CHANNEL_DATA;
extern int DEFAULT_BUFFER_SIZE;
extern int DEBUG_MODE;
extern int DETAILED_OUPUT;
extern int INCLUDE_TIMESTAMP;
extern int CHECK_RECORDS_ON_DAQ;
extern int DISPLAY_FILE_IO_INFO;
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

//const std::string currentDateTime(); //CM 3/18/15
const double currentTimeSeconds(); //CM 4/14/15 - current timestamp in seconds
//const QString currentDateTime(); //CM 4/16/15 - current timestamp in a readable format
const std::string currentDateTime(); //CM 8/10/15 - current timestamp using standard string

void startTimer(); //CM 5/18/15 - start global timer for timing how long operations take
double endTimer(); //CM 5/18/15 - end global timer for timing how long operations take

//to_bool function courteousy of Chris Jester-Young at: http://stackoverflow.com/questions/2165921/converting-from-a-stdstring-to-bool
inline bool to_bool(std::string const& s) {
    return s != "0";
}


#endif // LIBRARY

