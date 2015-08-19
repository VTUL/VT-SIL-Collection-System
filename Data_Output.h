#include <vector>
#include <map>
//#include <QPair>

#include "library.h"
#include "sensor_data.h"
#include <list>


#ifndef __DATA_OUTPUT
#define __DATA_OUTPUT

//#pragma once
class Data_Output
{
public:
	Data_Output();
	virtual ~Data_Output();

	virtual void setOutputType(OutputType type);
	virtual OutputType getOutputType();
    //virtual void writeChannelDataToFile(std::map<std::string, Channel*>*) = 0; //pure virtual function to write data in the specific format to disk

    //CM 5/18/15 - pure virtual function to write data in the specific format to disk - testing using safe arrays
    //virtual void writeChannelDataToFile_test_orig(std::vector<QPair<int, SAFEARRAY *> > *records,int recordSize, double sampleRate, QStringList* channelIDs, QList< QPair<int,QString> > activeChannels) = 0;
    //virtual void writeDaqDataToFile(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, QStringList* channelIDs, int numPhysicalChannels, QList<QPair<int, QString> > activeChannels, std::string daqID) = 0;
    //void writeDaqDataToFile_standard_string(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, QList<QPair<int, std::string> > activeChannels,std::string daqID);
    void writeDaqDataToFile_standard_string_no_qt(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, std::list<std::pair<int, std::string> > activeChannels,std::string daqID);

protected:
	OutputType outputType;
};

#endif
