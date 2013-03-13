//Socket Utility Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Begin Define Guards
#ifndef MSL_SOCKET_UTIL_H
#define MSL_SOCKET_UTIL_H

//String Header
#include <string>

//MSL Namespace
namespace msl
{
	//HTTP Create Header Function (Creates a header for sending
	std::string http_create_header(const unsigned int message_size,const std::string mime_type="text/html");

	//HTTP to ASCII Function (Converts a string with http symbols to ascii symbols)
	std::string http_to_ascii(std::string symbols);

	//HTTP Pack String Function (Packages a string for http travel)
	std::string http_pack_string(const std::string message,const std::string mime_type="text/html");
}

//End Define Guards
#endif
