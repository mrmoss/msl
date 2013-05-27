//JSON Header
//	Created By:		Mike Moss
//	Modified On:	05/26/2013

//Begin Define Guards
#ifndef MSL_JSON_H
#define MSL_JSON_H

//Map Header
#include <map>

//OStream Header
#include <ostream>

//String Header
#include <string>

//String Utility Header
#include "string_util.hpp"

//MSL Namespace
namespace msl
{
	//JSON Class Declaration
	class json
	{
		public:
			//Constructor (Default)
			json(const std::string& json_string="");

			//Set Operator (Sets a variable to a value)
			template<typename T> void set(const std::string& lhs,const T& rhs);

			//Get Operator (Returns the value of a variable)
			std::string get(const std::string& index);

			//String Function (Returns the JSON string)
			std::string str() const;

		private:
			//Member Variables
			std::map<std::string,std::string> _data;
	};
}

//Stream Out Operator
std::ostream& operator<<(std::ostream& lhs,const msl::json& rhs)
{
	return (lhs<<rhs.str());
}
#include <iostream>
//Constructor (Default)
msl::json::json(const std::string& json_string)
{
	//Create Editable Version of JSON String
	std::string parse_string=json_string;

	//Check for Start and End Brackets
	if(msl::starts_with(parse_string,"{")&&msl::ends_with(parse_string,"}"))
	{
		//Remove Starting Bracket
		parse_string.erase(0,1);

		//Parsing Variables
		std::string variable_name="";
		std::string variable_value="";
		unsigned int bracket_count_start=0;
		unsigned int bracket_count_end=0;

		//Go Through String
		for(unsigned int ii=0;ii<parse_string.size();++ii)
		{
			//Look for Variable Name Until Colon
			if(parse_string[ii]!=':')
			{
				variable_name+=parse_string[ii];
			}

			//Found Variable Name
			else
			{
				//Check for Start and End Quotes
				if(msl::starts_with(variable_name,"\"")&&msl::ends_with(variable_name,"\""))
				{
					//Remove Start and End Quotes
					variable_name.erase(0,1);
					variable_name.erase(variable_name.size()-1,1);

					//Go Through Rest of String
					for(unsigned int jj=ii+1;jj<parse_string.size();++jj)
					{
						//Count Start Brackets
						if(parse_string[jj]=='{')
							++bracket_count_start;

						//Count End Brackets
						if(parse_string[jj]=='}')
							++bracket_count_end;

						//Look for Variable Value
						if((parse_string[jj]!=','||bracket_count_start!=bracket_count_end)&&jj+1<parse_string.size())
						{
							variable_value+=parse_string[jj];
						}

						//Found Variable Value
						else
						{
							//Pass the Variable Value (Since we've read it already)
							ii+=variable_value.size();

							//Skip Separating Commas
							if(ii+1<parse_string.size()&&parse_string[ii+1]==',')
								++ii;

							//Remove Quotes (If there are any)
							if(msl::starts_with(variable_value,"\"")&&msl::ends_with(variable_value,"\""))
							{
								variable_value.erase(0,1);
								variable_value.erase(variable_value.size()-1,1);
							}

							//Add the Variable
							set(variable_name,variable_value);

							//Reset Parsing Variables
							variable_name="";
							variable_value="";

							//Break
							break;
						}
					}
				}
			}
		}
	}
}

//Set Operator (Sets a variable to a value)
template<typename T> void msl::json::set(const std::string& lhs,const T& rhs)
{
	_data[lhs]=msl::to_string(rhs);
}

//Get Operator (Returns the value of a variable)
std::string msl::json::get(const std::string& index)
{
	return _data[index];
}

//String Function (Returns the JSON string)
std::string msl::json::str() const
{
	//Add Initial Bracket
	std::string json_string="{";

	//Go Through Variables
	for(std::map<std::string,std::string>::const_iterator ii=_data.begin();ii!=_data.end();++ii)
	{
		//Add Variable to String
		json_string+="\""+ii->first+"\":";

		//Check for JSON Object Value
		if(msl::starts_with(ii->second,"{")&&msl::ends_with(ii->second,"}"))
			json_string+=ii->second;

		//Other Data Value
		else
			json_string+="\""+ii->second+"\"";

		//Get Next Variable
		std::map<std::string,std::string>::const_iterator next=ii;
		++next;

		//If Next is Not Last Add Comma
		if(next!=_data.end())
			json_string+=",";
	}

	//Add Final Bracket
	json_string+="}";

	//Return JSON String
	return json_string;
}

//End Define Guards
#endif

//Example
/*
//IO Stream Header
#include <iostream>

//JSON Header
#include "msl/json.hpp"

int main()
{
	//Create a JSON Object
	msl::json oo("{\"test1\":\"123\",\"test2\":\"1.456\",\"test3\":{\"test4\":{\"abc\":\"678\"}},\"test5\":\"lol kk\"}");

	//Print Out JSON Object
	std::cout<<oo<<std::endl;

	//Print Out JSON Values
	std::cout<<"\ttest1="<<oo.get("test1")<<std::endl;
	std::cout<<"\ttest2="<<oo.get("test2")<<std::endl;
	std::cout<<"\ttest3="<<oo.get("test3")<<std::endl;
	std::cout<<"\t\ttest4="<<msl::json(oo.get("test3")).get("test4")<<std::endl;
	std::cout<<"\t\t\tabc="<<msl::json(msl::json(oo.get("test3")).get("test4")).get("abc")<<std::endl;
	std::cout<<"\ttest5="<<oo.get("test5")<<std::endl;

	//All Done
	return 0;
}
*/