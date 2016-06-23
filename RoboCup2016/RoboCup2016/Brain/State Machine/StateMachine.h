#ifndef _StateMachine_H_
#define _StateMachine_H_

#include "State.h"

class State;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void SetCurrent(State* state);

	void Run();

private:
	State* m_currentState;
	State* m_nextState;

	bool IsNewRefereeMsg();
};

#endif

