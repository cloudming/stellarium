/*
Stellarium telescope control
Copyright (C) 2006 Johannes Gajdosik
Copyright (C) 2011 Bogdan Marinov (Ultima2000 modifications)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef _SERIAL_PORT_ULTIMA_2000_HPP_
#define _SERIAL_PORT_ULTIMA_2000_HPP_

#include "Connection.hpp"

#ifdef Q_OS_WIN32
  #include <windows.h>
#else
  #include <termios.h>
#endif

//! Serial interface connection, tailored for Ultima 2000.
//! A separate class is used until SerialPort can handle serial connection
//! parameters.
//! \todo Merge with SerialPort
class SerialPortUltima2000 : public Connection
{
public:
	//! Class constructor.
	//! @param serial_device A string containing the name of a serial port.
	//! On UNIX systems, this should be something like "/dev/ttyS0".
	//! On Microsoft Windows systems, this should be something like "COM1:".
	SerialPortUltima2000(Server &server, const char *serial_device);
	~SerialPortUltima2000();
	//! Returns true if the connection is closed.
	//! This method has different platform-dependent implementations.
	virtual bool isClosed() const
	{
	#ifdef Q_OS_WIN32
		return (handle == INVALID_HANDLE_VALUE);
	#else
		return IS_INVALID_SOCKET(fd);
	#endif
	}
	
protected:
	void prepareSelectFds(fd_set&, fd_set&, int&);
	
private:
	//! Returns false, as SerialPort implements a serial port connection.
	bool isTcpConnection() const {return false;}
	//! Returns true, as SerialPort implements a serial port connection.
	bool isAsciiConnection() const {return true;}
	
private:
#ifdef Q_OS_WIN32
	int readNonblocking(char *buf, int count);
	int writeNonblocking(const char *buf, int count);
	void handleSelectFds(const fd_set&, const fd_set&) {}
	HANDLE handle;
	DCB dcb_original;
#else
	struct termios termios_original;
#endif
};

#endif
