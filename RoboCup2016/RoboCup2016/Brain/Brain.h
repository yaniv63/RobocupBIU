#include "../Log/Log.h"
#include "State Machine/StateMachine.h"
#include "State Machine/GoalKeeperStateMachine.h"

#pragma once

class Brain
{
public:
	// Singletone
	static Brain* GetInstance();

	~Brain();

	void RunThread();

private:
	Brain();
	static Brain* m_instance;
};

