//to provide support for safearrays
#import "IviDriverTypeLib.dll" no_namespace
#if _WIN64
#import "VTEXDsa_64.dll" no_namespace
#else
#import "VTEXDsa.dll" no_namespace
#endif


#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H


//CM 5/26/15 - class added

//The purpose of thei class is to bundle the data returned from the DAQ for easy organization and management.
//  e.g., for each X records returned, store the data SAFEARRAY and respective time SAFEARRAYs in one place,
//  then provide simple access to them (hard stuff hidden).

class Sensor_Data
{
public:
    Sensor_Data(int r = 0, SAFEARRAY* d = NULL, SAFEARRAY* ts = NULL, SAFEARRAY* tf = NULL);
    ~Sensor_Data();

    int getNumRecords();
    SAFEARRAY* getData();
    SAFEARRAY* getTimeSeconds();
    SAFEARRAY* getTimeFraction();

    void setNumRecords(int r);
    void setData(SAFEARRAY* d);
    void setTimeSeconds(SAFEARRAY* ts);
    void setTimeFraction(SAFEARRAY* tf);


private:

    int numRecords;  //the number of records in this sensor data bundle
    SAFEARRAY* data; //the linear data stream of sensor samples returned from the memory read of the DAQ

    //both of these parts of the timestamp are required for accuracy since there are not enough digits in a double to
    //cover seconds since midnight Dec 1, 1969 with nanosecond resolution
    SAFEARRAY* timeSeconds; //the timestamp in seconds
    SAFEARRAY* timeFraction; //fraction of seconds that is part of the timestampe

};

#endif // SENSOR_DATA_H
