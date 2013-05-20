//CURRENTLY_UNIX_ONLY

#include "serial.hpp"
#include "msl/time_util.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

msl::serial::serial(const std::string& name,const unsigned int baud):_port(SERIAL_ERROR),_name(name),_baud(baud),_time_out(200)
{}

msl::serial::operator bool() const
{
	return (_port!=SERIAL_ERROR&&available()>=0);
}

bool msl::serial::operator!() const
{
	return !static_cast<bool>(*this);
}

void msl::serial::connect()
{
	_port=serial_connect(_name,_baud);
}

void msl::serial::close()
{
	serial_close(_port);
}

int msl::serial::available() const
{
	return serial_available(_port,0);
}

int msl::serial::read(void* buffer,const unsigned int size)
{
	return serial_read(_port,buffer,size,_time_out);
}

int msl::serial::write(void* buffer,const unsigned int size)
{
	return serial_write(_port,buffer,size,_time_out);
}

void msl::serial::set_timeout(const long time_out)
{
	_time_out=time_out;
}

long msl::serial::timeout() const
{
	return _time_out;
}

SERIAL msl::serial::system_serial() const
{
	return _port;
}

SERIAL msl::serial_connect(const std::string& name,const unsigned int baud)
{
	SERIAL port=open(name.c_str(),O_RDWR|O_NOCTTY|O_SYNC);

	if(port==SERIAL_ERROR)
		return serial_close(port);

	if(fcntl(port,F_SETFL,FNDELAY)==-1)
		return serial_close(port);

	if(ioctl(port,TIOCEXCL)==-1)
		return serial_close(port);

	termios options;

	if(tcgetattr(port,&options)==-1)
		return serial_close(port);

	speed_t baud_rate;

	if(baud==300)
		baud_rate=B300;
	else if(baud==1200)
		baud_rate=B1200;
	else if(baud==2400)
		baud_rate=B2400;
	else if(baud==4800)
		baud_rate=B4800;
	else if(baud==9600)
		baud_rate=B9600;
	else if(baud==19200)
		baud_rate=B19200;
	else if(baud==38400)
		baud_rate=B38400;
	else if(baud==57600)
		baud_rate=B57600;
	else if(baud==115200)
		baud_rate=B115200;
	else
		return serial_close(port);

	if(cfsetispeed(&options,baud_rate)==-1||cfsetospeed(&options,baud_rate)==-1)
		return serial_close(port);

	options.c_cflag|=(CS8|CLOCAL|CREAD|HUPCL);
	options.c_iflag|=(IGNBRK|IGNPAR);
	options.c_iflag&=~(IXON|IXOFF|IXANY);
	options.c_lflag=0;
	options.c_oflag=0;
	options.c_cc[VMIN]=0;
	options.c_cc[VTIME]=1;
	options.c_cflag&=~(PARENB|PARODD);
	options.c_cflag&=~CSTOPB;
	options.c_cflag&=~CRTSCTS;

	if(tcsetattr(port,TCSANOW,&options)==-1)
		return serial_close(port);

	if(tcflush(port,TCIFLUSH)==-1||tcdrain(port)==-1)
		return serial_close(port);

	return port;
}

SERIAL msl::serial_close(const SERIAL port)
{
	close(port);
	return SERIAL_ERROR;
}

int msl::serial_available(const SERIAL port,const long time_out)
{
	if(port==SERIAL_ERROR)
		return false;

	int return_value=-1;

	timeval temp={0,0};
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(port,&rfds);
	long time_start=msl::millis();

	do
	{
		return_value=select(1+port,&rfds,NULL,NULL,&temp);

		if(return_value>0)
			break;
	}
	while(msl::millis()-time_start<time_out);

	return return_value;
}

int msl::serial_read(const SERIAL port,void* buffer,const unsigned int size,const long time_out)
{
	if(port==SERIAL_ERROR)
		return -1;

	unsigned int bytes_unread=size;
	long time_start=msl::millis();

	do
	{
		unsigned int bytes_read=read(port,reinterpret_cast<char*>(buffer)+(size-bytes_unread),bytes_unread);

		if(bytes_read>0)
		{
			bytes_unread-=bytes_read;

			if(bytes_unread==0)
				return size;
		}
	}
	while(msl::millis()-time_start<time_out);

	return (size-bytes_unread);
}

int msl::serial_write(const SERIAL port,void* buffer,const unsigned int size,const long time_out)
{
	if(port==SERIAL_ERROR)
		return -1;

	unsigned int bytes_unsent=size;
	long time_start=msl::millis();

	do
	{
		unsigned int bytes_sent=write(port,reinterpret_cast<char*>(buffer)+(size-bytes_unsent),bytes_unsent);

		if(bytes_sent>0)
		{
			bytes_unsent-=bytes_sent;

			if(bytes_unsent==0)
				return size;
		}
	}
	while(msl::millis()-time_start<time_out);

	return (size-bytes_unsent);
}