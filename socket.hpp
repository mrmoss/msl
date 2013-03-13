//Socket Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Required Libraries:
//	wsock32 (windows only)

//Begin Define Guards
#ifndef MSL_SOCKET_H
#define MSL_SOCKET_H

//String Header
#include <string>

//String Stream Header
#include <sstream>

//Windows Dependices
#if(defined(_WIN32)&&!defined(__CYGWIN__))
	#include <winsock.h>

	#if(!defined(socklen_t))
		typedef int socklen_t;
	#endif

//Unix Dependices
#else
	#include <netinet/in.h>
	#include <unistd.h>
	#include <fcntl.h>

	#ifndef SOCKET
		#define SOCKET unsigned int
		#define INVALID_SOCKET (SOCKET)(~0)
		#define SOCKET_ERROR (-1)
	#endif
#endif

//MSL Namespace
namespace msl
{
	//End Line Variable(std::endl doesn't work)
	extern const char endl;

	//Socket Class Pre-Declaration(For msl::ipv4)
	class socket;

	//IPv4 Address Class Declaration
	class ipv4
	{
		public:
			//Constructor (Default)
			ipv4(const unsigned char ip[4]=NULL,const unsigned short port=0);

			//Copy Constructor
			ipv4(const msl::ipv4& copy);

			//Copy Assignment Operator
			msl::ipv4& operator=(const msl::ipv4& copy);

			//Build Function (Returns Raw Socket Address)
			sockaddr_in build() const;

			//String Accessor (X.X.X.X:PORT)
			std::string str() const;

			//Socket Class Friend
			friend class msl::socket;

		private:
			//Member Variables
			unsigned char _ip[4];
			unsigned short _port;
	};

	//Socket Class Declaration (NOT SURE IF BEING A STD::OSTREAM CHILD IS THE WAY TO GO HERE)
	class socket:public std::ostream
	{
		public:
			//Constructor (Default)
			socket(const std::string& address="0.0.0.0:0");

			//Copy Constructor
			socket(const msl::socket& copy);

			//Copy Assignment Operator
			socket& operator=(const msl::socket& copy);

			//Boolean Operator (Tests If Socket Is Good)
			operator bool() const;

			//Not Operator (For Boolean Operator)
			bool operator!() const;

			//Create Function (Hosts a Socket Locally)
			void create();

			//Connect Function (Connects to a Remote Socket)
			void connect();

			//Close Function (Closes a Local Socket)
			void close();

			//Accept Function (Accepts a Remote Connection to a Local Socket)
			msl::socket accept();

			//Read Function (Returns True if Read was Successful)
			bool read(void* buffer,const unsigned int size) const;

			//Write Function (Returns True if Write was Successful)
			bool write(void* buffer,const unsigned int size) const;

			//Check Function (Checks How Many Bytes there are to be Read, -1 on Error)
			int check() const;

			//Stream Out Operator
			template <typename T> friend msl::socket& operator<<(msl::socket& lhs,const T& rhs);

		private:
			//Member Variables
			msl::ipv4 _address;
			SOCKET _socket;
			bool _hosting;
	};

	//Socket Class Stream Operator (Templated Function)
	template <typename T> msl::socket& operator<<(msl::socket& lhs,const T& rhs)
	{
		//Create a String Stream
		std::ostringstream ostr;

		//Put in Data
		ostr<<rhs;

		//Write Data
		lhs.write(reinterpret_cast<void*>(const_cast<char*>(ostr.str().c_str())),ostr.str().size());

		//Return Stream
		return lhs;
	}
}

//Socket Create Function (Hosts a Socket Locally)
SOCKET socket_create(const msl::ipv4 ip,const unsigned int time_out=0,const bool UDP=false,const unsigned int buffersize=200);

//Socket Connection Function (Connects to a Remote Socket)
SOCKET socket_connect(const msl::ipv4 ip,const unsigned int time_out=0,const bool UDP=false);

//Socket Accept Function (Accepts a Remote Connection to a Local Socket)
SOCKET socket_accept(const SOCKET socket,msl::ipv4& client_ip,const unsigned int time_out=0);

//Socket Close Function (Closes a Local Socket)
SOCKET socket_close(const SOCKET socket);

//Socket Check Read Function (Checks How Many Bytes there are to be Read, -1 on Error)
int socket_check_read(const SOCKET socket,const unsigned int time_out=0);

//Socket Peek Function (Same as socket_read but Leaves Bytes in Socket Buffer)
bool socket_peek(const SOCKET socket,void* buffer,const unsigned int size);

//Socket Read Function (Reads Bytes from Socket Buffer)
bool socket_read(const SOCKET socket,void* buffer,const unsigned int size);

//Socket Write Function (Writes Bytes to Socket)
bool socket_write(const SOCKET socket,void* buffer,const unsigned int size);

//End Define Guards
#endif
