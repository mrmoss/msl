//JSON Source
//	Created By:		Mike Moss
//	Modified On:	11/07/2013

//Definitions for "json.hpp"
#include "json.hpp"

//Stream Out Operator
std::ostream& operator<<(std::ostream& lhs,const msl::json& rhs)
{
	return (lhs<<rhs.str());
}

//Constructor (Default)
msl::json::json(const std::string& json_string)
{
	//Error Variable
	bool error=false;

	//Get Data Between Brackets
	std::string temp=msl::extract_between(json_string,'{','}',false);

	//Parse Data
	while(temp.size()>0)
	{
		//Remove Whitespace
		while(temp.size()>0&&isspace(temp[0]))
			temp.erase(0,1);

		//Get Variable Name
		std::string var=msl::extract_between(temp,'\"','\"',true);
		temp.erase(0,var.size());
		var=msl::extract_between(var,'\"','\"',false);

		//Bad Variable Name
		if(var=="")
		{
			error=true;
			break;
		}

		//Remove Whitespace
		while(temp.size()>0&&isspace(temp[0]))
			temp.erase(0,1);

		//Remove Colon
		if(temp.size()>0&&temp[0]==':')
		{
			temp.erase(0,1);
		}

		//No Colon, Error
		else
		{
			error=true;
			break;
		}

		//Remove Whitespace
		while(temp.size()>0&&isspace(temp[0]))
			temp.erase(0,1);

		//Variable Value Variable
		std::string val="";

		//Check Size
		if(temp.size()>0)
		{
			//Extract Object
			if(temp[0]=='{')
			{
				//Get Object
				val=msl::extract_between(temp,'{','}',true);
				temp.erase(0,val.size());
			}

			//Extract String ("")
			else if(temp[0]=='\"')
			{
				//Get String
				val=msl::extract_until(temp,',',false);
				temp.erase(0,val.size());

				//Remove Whitespace At End of String
				while(val.size()>0&&std::isspace(val[val.size()-1]))
					val.erase(val.size()-1,1);

				//Valid String, Remove Quotes
				if(val.size()>=2&&val[0]=='\"'&&val[val.size()-1]=='\"')
				{
					val=val.substr(1,val.size()-2);
				}
				//Bad String
				else
				{
					error=true;
					break;
				}
			}

			//Extract Number
			else
			{
				//Get Number
				val=msl::extract_until(temp,',',false);
				temp.erase(0,val.size());

				//Remove Whitespace At End of Number
				while(val.size()>0&&std::isspace(val[val.size()-1]))
					val.erase(val.size()-1,1);

				//Period Finder
				bool found_period=false;

				//Check for Valid Number
				for(unsigned int ii=0;ii<val.size();++ii)
				{
					//Non-Number or Period or Mutiple Periods
					if((!std::isdigit(val[ii])&&val[ii]!='.')||(val[ii]=='.'&&found_period))
					{
						error=true;
						break;
					}

					//Look for Multiple Periods
					if(val[ii]=='.')
						found_period=true;
				}
			}

			//Remove Whitespace
			while(temp.size()>0&&isspace(temp[0]))
				temp.erase(0,1);

			//Remove Comma
			if(temp.size()>0&&temp[0]==',')
			{
				temp.erase(0,1);
			}

			//No Comma, Error
			else if(temp.size()>0)
			{
				error=true;
				break;
			}
		}

		//On Error
		else
		{
			error=true;
		}

		//Add Variable
		set(var,val);
	}

	//Errors Return Empty JSON Object
	if(error)
		_data.clear();
}

//Size Accessor (Returns number of variables)
unsigned int msl::json::size() const
{
	return _data.size();
}

//Get Operator (Returns variable from an index)
std::string msl::json::get(const unsigned int index)
{
	//Create a Count Variable (Damn iterators...)
	unsigned int count=0;

	//Iterate Through Variables
	for(std::map<std::string,std::string>::const_iterator ii=_data.begin();ii!=_data.end();++ii)
	{
		//Found Variable, Return Value
		if(count==index)
			return ii->first;

		//Increment Counter
		++count;
	}

	//Return Empty Value When Not Found
	return "";
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

		//Add Value to String
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