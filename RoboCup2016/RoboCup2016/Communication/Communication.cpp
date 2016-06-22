/*
 * Communication.cpp
 *
 *  Created on: May 26, 2016
 *      Author: laptopyellow
 */
#include "Communication.h"

Communication::Communication(){
	mUdpListener = UdpListener::GetInstance();
	//m_udp_sender = UdpSender::GetInstance();
}

Communication* Communication::mInstance = NULL;
Communication* Communication::GetInstance()
{
    if(mInstance == NULL)
    {
    	mInstance = new Communication();
        return mInstance;
    }
    else
    {
        return mInstance;
    }
}

Communication::~Communication()
{
	Communication::mInstance = NULL;
}

RoboCupGameControlData Communication::ReadAndClearFlag(){
	RoboCupGameControlData returnData = mUdpListener->GetData().SafeRead();
	bool clearFlag = false;
	mUdpListener->GetIsChangedFlag().SafeWrite(&clearFlag);
	return returnData;
}

bool Communication::isDataChanged(){
	return mUdpListener->GetIsChangedFlag().SafeRead();
}


