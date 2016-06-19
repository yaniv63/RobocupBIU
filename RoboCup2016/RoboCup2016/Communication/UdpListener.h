/*
 * UdpListener.h
 *
 *  Created on: May 26, 2016
 *      Author: laptopyellow
 */

#ifndef UDPLISTENER_H_
#define UDPLISTENER_H_

#include "CommUtils.h"
#include "message_enum.h"
#include "RoboCupGameControlData.h"
#include "UdpSender.h"
#include "PlayerInfo.h"
#include <thread>
#include "UdpUtils.h"

class Communication;

class UdpListener {

public:
	static UdpListener* GetInstance();
	void inline stopThread();

private:
	void Listen();
	UdpListener();
	~UdpListener();

	void Init();
	int Receive();
	HeaderType GetHeader();
	HeaderType ParseHeader();
	bool is_data_changed(RoboCupGameControlData* old_data, RoboCupGameControlData* new_data);

	enum{ BUFFER_LENGTH = 1000,HEADER_LENGTH=4};
	const char* m_port_number;
	int m_socket_fd;
	char m_byte_buffer[BUFFER_LENGTH];
	char m_header[HEADER_LENGTH+1];
	const int m_desired_version;

	sockaddr_storage m_their_addr;
	socklen_t m_addr_len;

	bool m_stop;
	std::thread m_listen_thread;
	static UdpListener* m_instance;
};



#endif /* UDPLISTENER_H_ */
