//String Utility Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Begin Define Guards
#ifndef MSL_STRING_UTIL_H
#define MSL_STRING_UTIL_H

//String Header
#include <string>

//String Stream Header
#include <sstream>

//MSL Namespace
namespace msl
{
	//String Convert Functions
	bool to_bool(std::string value);
	double to_double(const std::string& value);
	int to_int(const std::string& value);
	char to_char(const std::string& value);
	unsigned char to_uchar(const std::string& value);

	//To String Convert Function
	template<typename T> std::string to_string(const T& value)
	{
		//Convert Variables
		std::ostringstream ostr;

		//Convert
		ostr<<value;

		//Return Converted Value
		return ostr.str();
	}

	//Starts With Function (Checks if string starts with another string)
	bool starts_with(const std::string& str,const std::string& start);

	//Ends With Function (Checks if string ends with another string)
	bool ends_with(const std::string& str,const std::string& end);
}

//End Define Guards
#endif