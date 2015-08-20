#include <vector>
#include <map>

#include "library.h"
#include "sensor_data.h"
#include <list>


#ifndef __DATA_OUTPUT
#define __DATA_OUTPUT

class Data_Output
{
public:
	Data_Output();
	virtual ~Data_Output();

	virtual void setOutputType(OutputType type);
	virtual OutputType getOutputType();

    //CM 5/18/15 - virtual function to write data in the specific format to disk
    void writeDaqDataToFile_standard_string_no_qt(std::vector<Sensor_Data*>* sensorData, int recordSize, double sampleRate, std::vector<std::string>* channelIDs, int numPhysicalChannels, std::list<std::pair<int, std::string> > activeChannels,std::string daqID);

protected:
    OutputType outputType; //enum from library.h, define the specific output format being used, e.g., CSV or HDF5
};

#endif
