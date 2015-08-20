#include "library.h"
#include "Data_Output.h"


Data_Output::Data_Output()
{
}


Data_Output::~Data_Output()
{
}


////////////////////////////////////////////////////////////////////////////////////setOutputType()
//Sets the output type
//
//Parameters:
//		type			The type of file being parsed (e.g., csv, xml, ...)
//		
//Returns:				Nothing
//
//Pre:					All the parameters have not been initialized or set to values
//Post:					The file type of the current file being parsed is changed
//Called by:			Class Data_Output
//Calls:				none
//
void Data_Output::setOutputType(OutputType type){
	outputType = type;
}

////////////////////////////////////////////////////////////////////////////////////getOutputType()
//Returns the output type
//
//Parameters:
//		none
//		
//Returns:				The type of file being parsed (e.g., csv, xml, ...)
//
//Pre:					All the parameters have not been initialized or set to values
//Post:					The file type of the current file being parsed is known
//Called by:			Class Data_Output
//Calls:				none
//
OutputType Data_Output::getOutputType(){
	return outputType;
}
