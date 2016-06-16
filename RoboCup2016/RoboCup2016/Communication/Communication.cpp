/*
 * Communication.cpp
 *
 *  Created on: May 26, 2016
 *      Author: laptopyellow
 */
#include "Communication.h"

Communication::Communication(){
	m_udp_listener = UdpListener::GetInstance();
}
