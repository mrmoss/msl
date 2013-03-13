//File Utility Source
//	Created By:		Mike Moss
//	Modified On:	03/04/2013

//Begin Define Guards
#ifndef MSL_FILE_UTIL_H
#define MSL_FILE_UTIL_H

//String Header
#include <string>

//Vector Header
#include <vector>

//MSL Namespace
namespace msl
{
	//List Directory Function (Lists files in directory as strings in a vector)
	std::vector<std::string> list_directory(const std::string& directory=".");

	//File to String Function (Loads a file from disk as a string, returns false on error)
	bool file_to_string(const std::string& filename,std::string& data);

	//String to File Function (Saves a string to disk as a file, returns false on error)
	bool string_to_file(const std::string& data,const std::string& filename);

	//Remove File Function (Removes a file from disk, returns false on error)
	bool remove_file(const std::string& filename);

	//Remove Directory Function (Removes a directory from disk recursively, returns false on error)
	bool remove_directory(const std::string& directory);

	//File Exists Function (Checks if a file exists on disk)
	bool file_exists(const std::string& filename);
}

//End Define Guards
#endif