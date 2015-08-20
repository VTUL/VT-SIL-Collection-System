//======= CM 5/8/15 - for sleep function ============
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
//===================================================

#include <conio.h> //CM 5/13/15 - include for kbhit to catch keyboard input without pausing the program


#include "library.h"
#include "DAQ.h"
#include "Data_Output.h"
#include "Data_Output_Factory.h"



DAQ::DAQ(long int b)
{
	// Create a new instance of the VTEXDsa driver
	try{
        //initialize all variables
		driver = new IVTEXDsaPtr(__uuidof(VTEXDsa));
        channels = NULL;
		data = NULL;
		ts_sec = NULL;
		ts_frac = NULL;
		addl_data = NULL;
        numPhysicalChannels = 0;
        bufferSize = b;
		daqID = "";
        dataFormat = RAW_Record; //CM 4/14/15 - the format the data is put in after recieved from the DAQ.
                                 // The format is determined by the post-processing performed after the data is recieved from the DAQ

        parsedChannelNames = false; //CM 3/19/15 - test class variable instead of local in continuous collection function

        numCollectedPoints = 0;
        numCollectedRecords = 0;
        sampleRate = 0;

//        sensorData.reserve(bufferSize*2); //CM 5/11/15 - reserve twice the buffer size beforehand, this should increase efficiency, and
                                          //    when the buffer is full and more data is required to be stored, this is a built in
                                          //    handler for overflow

        //CM 5/27/15 - initialize the output object
        output = (CSV_Output*)Data_Output_Factory::CreateDataOutput(CSV);

        configParser = new Config_Parser(); //CM 6/1/15

	}
	catch (...)
	{
		//Do something to intelligently deal with any other non-VTEX errors.

        //for now, just output an error message indicating where the error occured
        std::cout<<"Exception caught: DAQ class, constructor."<<std::endl;
	}
}

//deconstructor
DAQ::~DAQ()
{
    try{
        std::cout<<"DAQ deconstructor called"<<std::endl;

        close();
    }
    catch(...){
        //for now, just output an error message indicating where the error occured
        std::cout<<"Exception caught: DAQ class, deconstructor."<<std::endl;
    }
}

//returns the unique ID of the DAQ (as a string)
std::string DAQ::getDaqID(){
    return daqID;
}

int DAQ::initialize(std::string ipAddress){

	try{
		daqID = ipAddress;
		_bstr_t resource_str(daqID.c_str());
		try
		{

			// Initialize the driver
			driver->Initialize(resource_str, VARIANT_FALSE, VARIANT_TRUE, "");

            //==========================================================================================
            //CM 6/8/15 - adding sync code for time and connecting to multiple DAQs in on driver session
            /*
            Note: To enable LAN Synchronization, all EMX-2500 controllers must be set to use PTP as the time source
            */

            // The ReferenceOscillator Source and TimestampSource must be set to 'System'
            driver->ReferenceOscillator->Source = VTEXDsaReferenceOscillatorSourceSystem;
            driver->ReferenceOscillator->TimestampSource = VTEXDsaReferenceOscillatorTimestampSourceSystem;

            // Set the CoordinationLine to "LAN,PXI0" and call SendSoftwareSync
            driver->Sync->CoordinationLine = "LAN,PXI0";
            driver->Sync->SendSoftwareSync();

            VARIANT_BOOL is_synchronized = driver->Sync->IsSynchronized;
            if(is_synchronized)
                std::cout << "LAN Synchronization Enabled" << std::endl;
            else
                std::cout << "There was an issue enabling LAN Synchronization" << std::endl;
            //==========================================================================================


			// "ALL" is a channel group which allows you to modify all of the channels at once
			/**/
			driver->Channels->Item["ALL"]->Enabled = true; //Enable the channels
			driver->Channels->Item["ALL"]->AutoRangeMode = VTEXDsaAutoRangeModeOff;
			driver->Channels->Item["ALL"]->Range = 10;
			driver->Channels->Item["ALL"]->Function = VTEXDsaChannelFunctionIEPE; // Set the channels to measure voltage
			driver->Channels->Item["ALL"]->Coupling = VTEXDsaCouplingAC; //CM 11/4/14 - set to AC
			driver->Channels->Item["ALL"]->IEPECurrent = 0.004; //CM 11/4/14 - signal using IEPE and set amp value
			/**/

            driver->Measurement->NumRecordsPerTrigger = 0; //CM 4/13/15 - set to 0 when performing continuous collection, otherwise, set to >= 1


            //==== CM 6/1/15 - load the configuration file============================

            //CM 8/10/15 - update to use standard string
            configParser->parseConfigFileStr("daq_config.txt"); //Take the file name of the config file and parse it

            //CM 6/2/15 - grab the move path fromt he confog file and set the output move path to it
            movePath = configParser->getMovePath();
            output->setMoveDir(movePath);

            //store the record size and sample rate specified in the config file
            sampleRate = configParser->getSampleRate();
            recordSize = sampleRate; //CM 8/7/15 - each record will equal a second's worth of data (number of sample points per second)

            bufferSize = configParser->getSecondsPerFile(); //CM 8/7/15 - update buffer size based on how many seconds to be recorded per file.
                                                            //              This will be the number of records to be collect per file.
                                                            //              NOTE: THIS REMOVES HANDLING THE RECORD SIZE FROM THE USER

            //set global variables used as flags (see library.h for definitions ======
            ALL_CHANNEL_DATA = configParser->getStoreAllChannelData();
            CHECK_RECORDS_ON_DAQ = configParser->getCheckRecordsOnDaq();
            //========================================================================


            //set daq driver parameters based on what is in the config file
            driver->Measurement->Sampling->SampleRate = sampleRate;
            driver->Measurement->Sampling->RecordSize = recordSize;
            driver->Measurement->Sampling->ClockFrequency = configParser->getClockFrequency();
            driver->Measurement->Sampling->Prescaler = configParser->getPrescaler();
            //========================================================================

            //CM 5/21/15 - outputting the set-up of the system ======================
            //get the number of physical channels
            numPhysicalChannels = driver->Channels->GetNumChannels();
            double clockFreq = 0.0;
            long preScaler = 0.0;
            driver->Measurement->Sampling->get_ClockFrequency(&clockFreq);
            driver->Measurement->Sampling->get_Prescaler(&preScaler);
            sampleRate = driver->Measurement->Sampling->SampleRate;
            std::cout<<"==================================================================================="<<std::endl;
            std::cout<<"System set-up for DAQ: "<<daqID<<std::endl;
            std::cout<<"Clock Frequency: "<<clockFreq<<std::endl;
            std::cout<<"Prescaler: "<<preScaler<<std::endl;
            std::cout<<"Actual sampling rate: "<<sampleRate<<" samples a second"<<std::endl
                     <<"Record size: "<<recordSize<<std::endl
                     <<"Number of physical channels: " << numPhysicalChannels
                     <<std::endl<<std::endl
                     <<"Sampling every "<<recordSize/sampleRate<<" of a second ==> "<<std::endl
                     <<'\t'<<"Record size / sampling rate = "<<recordSize<<'/'<<sampleRate<<" = "<<recordSize/sampleRate<<" of a second"<<std::endl
                     <<"\tHence, recording Samply rate * number of channels = "<<sampleRate*numPhysicalChannels<<" samples every second"<<std::endl
                     <<std::endl;
            std::cout<<"==================================================================================="<<std::endl;
            //========================================================================


            driver->StreamingData->EnableStreaming(); //CM 12/2/14 - enable streaming

            // Initiate the measurement, i.e., get a measurement
            driver->Measurement->Initiate();
		}
		catch (_com_error& e)
		{
            //ouput what section of the error handling code the error was caught
            std::cout<<"Exception caught: DAQ class, initialize, VTEX error section."<<std::endl;


			if (e.Error() == E_IVI_INSTRUMENT_STATUS) // If the error is IVI_INSTRUMENT_STATUS, we need to check the error queue on the instrument
			{
				BSTR error = NULL;
				long error_code;
				driver->Utility->ErrorQuery(&error_code, &error);
				std::wcerr << "Error Code: " << error_code << std::endl;
				std::wcerr << error << std::endl;

				::SysFreeString(error);
			}
			else
			{
				std::wcerr << e.ErrorMessage() << std::endl;
			}
		}
	}
	catch (...)
	{
		//Do something to intelligently deal with any other non-VTEX errors.
        //for now, just output an error message indicating where the error occured
        std::cout<<"Exception caught: DAQ class, initialize."<<std::endl;
	}
	return 0;
}




//collect raw, unprocessed data from the DAQ, this is store in raw format until writing to file is performed
bool DAQ:: continuousRawData(){
    bool loop = true; //CM 3/19/15

    int numRecordsRecorded = 0; //count the real number of records seen before a write to disk is performed

    try{

        //==========================
        while (loop == true){


            //=======================================================================================================
            //CM 8/13/15 - test to see if a keystroke was pressed --> use this to elegantly close the program
            char c;
            if(kbhit()){
               c = getch();
               printf("You have pressed: %c\n", c);

               if(c == 'e'){
                   //close();
                   cout<<"Exiting..."<<endl;
                   loop = false;

                   //CM 8/17/15 - set the buffer full flag so whatever is in the buffer will get written out before exiting
               }
            }
            //=======================================================================================================

            // Read out all of the data from the FIFO with a timeout of 1000ms
            // Set the number of records to read to -1, which indicates that all records should be read, otherwise,
            //  specify how many records at a time you want, e.g., set to 1 currently to only process 1 record at a time
            //
            driver->StreamingData->MemoryRead(1, &actual_records, &channels, &data, &ts_sec, &ts_frac, &addl_data); //CM - 12/1/14 - trying to get streamind data working

            //if have a record available
            if(actual_records > 0){

                //if no records to grab then just continue
                if(data == NULL){
                    if (DEBUG_MODE == 1){
                        std::wcout << "Data pointer is NULL!!!: " << std::endl;
                    }
                    continue;
                }

                numCollectedRecords = numCollectedRecords + actual_records;
                numRecordsRecorded = numRecordsRecorded + actual_records; //count the real number of records seen before a write to disk is performed


                 //if the channels names have not been parsed and stored after a memory read of the daq,
                 //  then record them.
                 //  NOTE:This only happens for the very first record collected in the entire session
                 if (parsedChannelNames == false){
                     //convert the BSTR of the channels to a form that is easily processed
                     const char *p= const_cast<char *>(_com_util::ConvertBSTRToString(channels));


                     //======================================================================================================
                     //CM 7/10/15 - Removing use of QString for speed-up - conerting QString to std::string for writing
                     //             to file is very costly.
                     std::string channelsStr = std::string(p); //this is a string with channel IDs separated by commas
                     std::stringstream ss(channelsStr);


                     //now need to  parse the channel IDs that are deliminated by ','s
                     while (ss.good()){
                         std::string substr;
                         getline(ss, substr, ',');
                         channelIDs.push_back(substr); //add the channel ID to the vector
                     }


                     std::vector<std::string> activeChannelList; //store the channel names that have sensors on them

                     //======================================================================================================

                     std::string currentChannel;

                     int numChannelsWithNoSensor = 0;

                     //iterate over the channels to find out which channel(s) have sensors
                     for(int i = 0; i < numPhysicalChannels;i++){
                         currentChannel = channelIDs.at(i); //CM 7/10/15 - using string
                         //if channel has no sensor
                         if( (driver->Channels->Item[currentChannel.c_str()]->Overload->Status) & 0x20){
                            numChannelsWithNoSensor++;
                         }
                         //otherwise, channel has a sensor, so record which one
                         else{
                             activeChannelList.push_back(currentChannel); //CM 7/10/15 - using string
                         }
                     }
                     std::cout<<daqID<<":"<<std::endl;
                     std::cout<<"Number of channels without sensors: "<<numChannelsWithNoSensor<<std::endl;
                     std::cout<<activeChannelList.size()<<" channel(s) with sensors: ";

                     //iterate through the channels with sensors (active channels) and output which ones
                     for(int i = 0; i < activeChannelList.size();i++){
                         //if at the last active channel, do not add a comma
                         if(i == activeChannelList.size()-1){
                             std::cout<<activeChannelList.at(i)<<std::endl; //CM 7/10/15 - using string
                         }
                         //otherwise, more channels to go, so add a comma
                         else{
                             std::cout<<activeChannelList.at(i)<<", "; //CM 7/10/15 - using string
                         }

                         //====================================================================================================
                         //CM 7/10/15 - iterate through the vector looking for the active channel index in the vector of channel IDs
                         //             This is used to get the channels that only have sensors on them
                         int channelIndex = -1;
                         for (int x = 0; x < channelIDs.size(); x++){
                             //if the ID in the channelIDs vector at index x is the same as the current active channel ID,
                             //     then store that index and exit the loop
                             if(channelIDs.at(x) == activeChannelList.at(i)){
                                channelIndex = x;
                                break;
                             }
                         }
                         //====================================================================================================

                         //CM 8/7/15 - remove use of Qt data structures ========================
                         pair<int,std::string> bundleStd = pair<int,std::string>(channelIndex,activeChannelList.at(i)); //change to using std::string
                         activeChannelsStd.push_back(bundleStd);
                         //=====================================================================
                     }
                     std::cout<<std::endl;
                     std::cout<<"======================================================================="<<std::endl;

                     parsedChannelNames = true; //signal the channel names (identifiers) have been parsed and recorded
                 }

                 //CM 8/18/15 - print out the progress ========================
                 //if the first record, then output the progress title
                 if(numRecordsRecorded == 1){
                     cout<<"Collecting for "<<bufferSize<<" seconds:"<<endl;
                     cout<<"\tProgress (in seconds): "<<endl;
                     cout<<"\t \t";
                 }

                 //if not the last record of the file, add a comma
                 if(numRecordsRecorded <= bufferSize-1){

                    //if not reached the end of a progress line (define width of number of record numbers printed out)
                    if((numRecordsRecorded % PROGRESS_LINE_LENGTH) != 0){
                        cout<<numRecordsRecorded<<',';
                    }
                    //otherwise, go to the next line
                    else{
                        cout<<numRecordsRecorded<<','<<endl<<"\t \t";
                    }
                 }
                 //otherwise, leave off the comma and end the line
                 else{
                     //if not reached the end of a progress line (define width of number of record numbers printed out)
                     //     for the last record
                     if((numRecordsRecorded % PROGRESS_LINE_LENGTH) != 0){
                         //cout<<endl<<"\t \t"<<numRecordsRecorded;
                         cout<<numRecordsRecorded<<endl;
                     }
                     //otherwise, go to the next line and print the last record number
                     else{
                        cout<<numRecordsRecorded<<"\t \t"<<endl;
                     }
                 }



                 //CM 8/20/15 - keep the following in case needed in future, the current solution is used above.
                 //    NOTE: This is being kept since some useful math was figured out and I do not want to remove it just yet
                 /*I was doing a special ratio for a progress bar, but easiest is to just print out the
                  * current record number (i.e., the current second of data being collected)

                 double ratio = bufferSize/PROGRESS_LENGTH;

                 //if the current record number is an equal divisor of the progress length, then
                 // output a dot '.'
                 //     This takes the ratio of the buffer size and the progress length and
                 //     performs a modulus to see if the current record number divides equally
                 //     according to the ratio
                 //         E.g., if the # of records per file (R) is 120 and the progress length (L)
                 //         is 60, then R/L = 120/60 = 2. Let R/L = D. then given the current record number
                 //         being collected (r), if r % D == 0, then print a dot, or in other words,
                 //         if r divides evenly into D (i.e., r is an even number since D = 2), then
                 //         print a dot '.'
                 if(ratio >= 1){ //if the ration is over or equal to 1, then treat as normal
                     if(numRecordsRecorded % int(ratio) == 0){
                         cout<<'.';
                     }
                 }
                 //otherwise, ratio D is 0 < D < 1. this happens when the number of records per file is less than
                 //     the length of the progress bar max dots
                 else{
                    ratio = PROGRESS_LENGTH/bufferSize; //reverse ratio to account for 0 < R/L < 1

                    //the ratio represents the number of dots to output per record received
                    for(int r = 0;r<ratio;r++){
                        cout<<'.';
                    }

                 }
                 */
                 //============================================================

                 size_t data_count = data->rgsabound->cElements;

                 //CM 6/4/15 - Keep the following so testing on timestamps can be performed ============================
                 //NOTES: number of seconds and second fractions is the same as the number of channel IDs
                 //     If more than one record are provided by the DAQ, then the above numbers will be
                 //     the number of channel ID's * number of records received.
                 /*
                 size_t second_count = ts_sec->rgsabound->cElements;
                 std::cout<<"number of seconds elements: "<<second_count<<std::endl;
                 std::cout<<"number of data samples: "<<data_count<<std::endl;
                 std::cout<<"actual records: "<<actual_records<<std::endl;
                 */
                 //=====================================================================================================


                 numCollectedPoints = numCollectedPoints + int(data_count);

                 Sensor_Data* sData = new Sensor_Data(actual_records,data,ts_sec,ts_frac);
                 appendToData(sData);
                 sData = NULL;

                 ts_sec = NULL;
                 ts_frac = NULL;
                 data = NULL;

                 //CM 7/24/15 - releasing BSTRs ==================== for some reason this causes a crash with 5 DAQs but not 1
                 //     For the 1 DAQ, there is no memory leak.
                 //     POSSIBLE FIX: store these variables in the Sensor_Data object
                 //     and treat like the other SAFEARRAYs stored (including release of memory in the Sensor_Data object)
                 //::SysFreeString(channels);
                 //::SysFreeString(addl_data);
                 //=================================================

                //check to see if the buffer is full, if so, then exit collection loop so data can be written to disk
                if (getBufferFull() == true){
					loop = false; //end collection loop to write to disk
                    //CM 5/21/15 - print out number of points collected per number of records and how that was calculated =============
                    cout<<endl;
                    std::cout<<"************************************************************************************"<<std::endl;
                    std::cout<<daqID<<" collected "<<numCollectedPoints<<" samples for "<<numRecordsRecorded<<" records."<<std::endl;
                    //only print info when wanting more details about what the collection system is doint
                    if(DETAILED_OUPUT == 1){
                        std::cout<<"\tCalculation to achieve "<<numCollectedPoints<<" samples:"<<std::endl;
                        std::cout/*<<'\t'<<"Sample Rate: "<<driver->Measurement->Sampling->SampleRate*/<<std::endl
                                 <<"\t \t"<<"Number of records: "<<numRecordsRecorded<<std::endl
                                 <<"\t \t"<<"Record size: "<<recordSize<<std::endl
                                 <<"\t \t"<<"Number of channels: "<<numPhysicalChannels<<std::endl
                                 <<"\t \t"<<"Collected samples = number of records * record size * number of channels"<<std::endl
                                 <<"\t \t"<<numCollectedPoints<<" = "<<numRecordsRecorded<<" * "<<recordSize<<" * "<<numPhysicalChannels<<std::endl
                                 <<std::endl;
                    }
                    std::cout<<"************************************************************************************"<<std::endl;
                    numRecordsRecorded = 0;
                    numCollectedPoints = 0;
                    //======================

                    //see if buffer on DAQ is filling up ===============
                    if(CHECK_RECORDS_ON_DAQ == 1){
                        //if records are on DAQ, print out how many
                        if(driver->StreamingData->NumMemoryRecords > 0){
                            std::cout<<daqID<<": Current number of records on DAQ to pull: "<<driver->StreamingData->NumMemoryRecords<<std::endl;
                        }
                    }
                    //===================================================

					return true;
				}
				//=====================================================================================

            }
            std::this_thread::sleep_for (std::chrono::milliseconds(5)); //CM 5/8/15 - give system a rest

        }

    }
    catch (_com_error& e)
    {
        if (e.Error() == E_IVI_INSTRUMENT_STATUS) // If the error is IVI_INSTRUMENT_STATUS, we need to check the error queue on the instrument
        {
            //for now, just output an error message indicating where the error occured
            std::cout<<"Exception caught: DAQ class, continuousRawData, if section of VTEX error handling."<<std::endl;

            BSTR error = NULL;
            long error_code;
            driver->Utility->ErrorQuery(&error_code, &error);
            std::wcerr << "Error Code: " << error_code << std::endl;
            std::wcerr << error << std::endl;

            ::SysFreeString(error);
        }
        else
        {
            std::wcerr << e.ErrorMessage() << std::endl;
            //for now, just output an error message indicating where the error occured
            std::cout<<"Exception caught: DAQ class, continuouseRawData, else clause of VTEX error handling."<<std::endl;
        }
        driver->Close(); // If an exception occurs, try to close the driver session gracefully
    }

    return false; //CM 8/18/15 - when the collection loop is ended before a full collection cycle is reached,
                  //                 then trigger collection needs to end, i.e., return false to signal to stop
                  //                 collection (see DAQ_Interface->collectDaqData()

}

//CM 5/13/15 - try storing the safearray from the DAQ directly
void DAQ::appendToData(Sensor_Data *sd){

   sensorData.push_back(sd); //store the new sensor data object
    sd = NULL;

   //CM 5/11/15 - see if buffer is full, if so, then trip flag
   if(numCollectedRecords >= bufferSize){
       bufferFull = true;

   }

}





//write the data in each channel to the specified output object stored in the daq object
void DAQ::writeDAQDataToFile(){

    //output->writeDaqDataToFile(&sensorData,recordSize,sampleRate,&channelIDs, numPhysicalChannels, activeChannels, daqID);

    //CM 7/10/15 - change to using std::string
    output->writeDaqDataToFile(&sensorData, recordSize, sampleRate, &channelIDs, numPhysicalChannels, activeChannelsStd, daqID);
}


int DAQ::close(){

    //CM 8/13/15 - closing out the session including releasing memory

    delete output;
    delete configParser;

    std::vector<Sensor_Data*>::iterator it;

    //clean up memory for the sensor data vector stored
    for(it = sensorData.begin(); it != sensorData.end(); it++){
        delete *it;
    }

    driver->Measurement->Abort(); //CM 5/27/15 - stop measurment

    driver->StreamingData->DisableStreaming(); //CM - 12/2/14 - disable streaming

    driver->Close();


	::CoUninitialize();

	return 0;
}

//CM 5/18/15
bool DAQ::getBufferFull(){
    return bufferFull;
}

//CM 5/21/15 - get the sample rate of the DAQ
double DAQ::getSampleRate(){
    return sampleRate;
}

//CM 3/18/15 - empty the buffer (data parameter)so new data can be collected
bool DAQ::clearBuffer(){ //empty the buffer (data parameter)so new data can be collected

    try{

        //CM 7/23/15 - free the memory for the sensor data objects =====================
        std::vector<Sensor_Data*>::iterator it;

        //clean up memory for the sensor data vector stored
        for(it = sensorData.begin(); it != sensorData.end(); it++){
            delete *it;
        }
        //==============================================================================


        sensorData.clear();//reserve the buffer size again since clear() reduces the size of the buffer to 0

        bufferFull = false; //CM 5/11/15 - reset buffer full flag

        numCollectedRecords = 0; //reset
    }
    catch(...){
        //for now, just output an error message indicating where the error occured
        std::cout<<"Exception caught: DAQ class, clear buffer."<<std::endl;
    }

    return true;
}
