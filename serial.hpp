//CURRENTLY_UNIX_ONLY

#ifndef MSL_SERIAL_H
#define MSL_SERIAL_H

#include <string>

#include <sstream>

#define SERIAL int
#define SERIAL_ERROR (-1)

namespace msl
{
	// (NOT SURE IF BEING A STD::OSTREAM CHILD IS THE WAY TO GO HERE)
	class serial:public std::ostream
	{
		public:
			serial(const std::string& name,const unsigned int baud);
			operator bool() const;
			bool operator!() const;
			void connect();
			void close();
			int read(void* buffer,const unsigned int size);
			int write(void* buffer,const unsigned int size);
			int available() const;
			void set_timeout(const long time_out);
			long timeout() const;
			SERIAL system_serial() const;

			template <typename T> friend msl::serial& operator<<(msl::serial& lhs,const T& rhs);

		private:
			SERIAL _port;
			std::string _name;
			unsigned int _baud;
			long _time_out;
	};

	template <typename T> msl::serial& operator<<(msl::serial& lhs,const T& rhs)
	{
		std::ostringstream ostr;

		ostr<<rhs;

		lhs.write(reinterpret_cast<void*>(const_cast<char*>(ostr.str().c_str())),ostr.str().size());

		return lhs;
	}

	SERIAL serial_connect(const std::string& name,const unsigned int baud);
	SERIAL serial_close(const SERIAL port);
	int serial_available(const SERIAL port,const long time_out=0);
	int serial_read(const SERIAL port,void* buffer,const unsigned int size,const long time_out=200);
	int serial_write(const SERIAL port,void* buffer,const unsigned int size,const long time_out=200);
}

#endif
