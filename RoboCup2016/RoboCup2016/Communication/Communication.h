#ifndef COMM_H
#define COMM_H

#include "UdpSender.h"
#include "../Vision/SharedMemory.h"

class UdpListener;
struct RoboCupGameControlData;

class Communication {
public:
	static Communication* GetInstance();
	~Communication();
	RoboCupGameControlData* ReadAndClearData();
	void RaiseDataChanged();
	void WriteGameData(RoboCupGameControlData* data);
private:
	UdpListener* m_udp_listener;
	UdpSender* m_udp_sender;
	static Communication* m_instance;
	Communication();
	SharedMemory<RoboCupGameControlData> m_shared_data;
	SharedMemory<bool> m_shared_is_data_changed;
};

#endif
