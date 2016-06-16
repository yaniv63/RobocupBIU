#ifndef COMM_H
#define COMM_H

#include "UdpListener.h"

class Communication {

public:
	Communication();
	~Communication();
private:
	UdpListener* m_udp_listener;

};

#endif
