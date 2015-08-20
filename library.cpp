

/*
 *  library.cpp
 *
 *
 *  Created by Chreston Miller on 3/17/15.
 *  Copyright 2011 Virginia Tech. All rights reserved.
 *
 */


#include <iostream>
#include <time.h>
#include "library.h"

time_t startTime,endTime;

//CM 6/5/15 - configuration parameters =================================

int ALL_CHANNEL_DATA = 1; //CM 6/4/15 - testing chaninge a #define to a regular global variable so it can be dynamically changed
                          //    by the config file
int DEFAULT_BUFFER_SIZE =  20;
int DEBUG_MODE = 0;
int DETAILED_OUPUT = 0;
int INCLUDE_TIMESTAMP = 0;
int CHECK_RECORDS_ON_DAQ = 1;
int DISPLAY_FILE_IO_INFO = 0;
int PROGRESS_LINE_LENGTH = 20; //CM 8/18/15 - define length of progress bar (in characters)

#define DISPLAY_FILE_IO_INFO 0;

//======================================================================


//CM 4/14/15 - get number of seconds since Jan, 1, 2015
//      Courtesy of cplusplus.com: http://www.cplusplus.com/reference/ctime/time/
const double currentTimeSeconds(){
    time_t timer;
    struct tm y2k = {0};
    double seconds;

    y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 115; y2k.tm_mon = 0; y2k.tm_mday = 1;

    time(&timer);  /* get current time; same as: timer = time(NULL)  */

    seconds = difftime(timer,mktime(&y2k));

    return seconds;
}


//CM 8/10/15 - use straint C++ to get current date and time
//      Curtesy of TrungTN at http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    //strftime(buf, sizeof(buf), "%Y-%m-%d_%X", &tstruct);
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H_%M_%S", &tstruct);

    return buf;
}



void startTimer(){

    time(&startTime);
}

double endTimer(){
    double timeForCollection = 0.0;
    time(&endTime);

    timeForCollection = difftime(endTime,startTime);

    return timeForCollection;
}
