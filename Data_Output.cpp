#include "library.h"
#include "Data_Output.h"


Data_Output::Data_Output()
{
}


Data_Output::~Data_Output()
{
}


////////////////////////////////////////////////////////////////////////////////////setOutputType()
//Sets the file type
//
//Parameters:
//		type			The type of file being parsed (e.g., csv, xml, ...)
//		
//Returns:				Nothing
//
//Pre:					All the parameters have not been initialized or set to values
//Post:					The file type of the current file being parsed is changed
//Called by:			Class I_Parser
//Calls:				none
//
void Data_Output::setOutputType(OutputType type){
	outputType = type;
}

////////////////////////////////////////////////////////////////////////////////////getOutputType()
//Returns the file type
//
//Parameters:
//		none
//		
//Returns:				The type of file being parsed (e.g., csv, xml, ...)
//
//Pre:					All the parameters have not been initialized or set to values
//Post:					The file type of the current file being parsed is known
//Called by:			Class I_Parser
//Calls:				none
//
OutputType Data_Output::getOutputType(){
	return outputType;
}
