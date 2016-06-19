/*
 * Communication.cpp
 *
 *  Created on: May 26, 2016
 *      Author: laptopyellow
 */
#include "Communication.h"

Communication::Communication(){
	m_udp_listener = UdpListener::GetInstance();
	//m_udp_sender = UdpSender::GetInstance();
}

Communication* Communication::m_instance = NULL;
Communication* Communication::GetInstance()
{
    if(m_instance == NULL)
    {
    	m_instance = new Communication();
        return m_instance;
    }
    else
    {
        return m_instance;
    }
}

Communication::~Communication()
{
	Communication::m_instance = NULL;
}

RoboCupGameControlData* Communication::ReadAndClearData(){
	RoboCupGameControlData* returnData = NULL;
	if(m_shared_is_data_changed.SafeRead()){
		returnData = m_shared_data.SafeRead();
		bool clearFlag = false;
		m_shared_is_data_changed.SafeWrite(&clearFlag);
	}
	return returnData;
}

void Communication::RaiseDataChanged(){
	bool raise = true;
	m_shared_is_data_changed.SafeWrite(&raise);
}
