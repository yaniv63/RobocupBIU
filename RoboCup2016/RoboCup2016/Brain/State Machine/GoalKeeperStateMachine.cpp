/*
 * GoalKeeperStateMachine.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#include "GoalKeeperStateMachine.h"

GoalKeeperStateMachine::GoalKeeperStateMachine() {
	m_currentState = new GoalKeeperInit();
}

GoalKeeperStateMachine::~GoalKeeperStateMachine() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperStateMachine::Run()
{
	StateName stateName;
	string stateVariable = "";
	while (m_currentState != NULL)
	{
		if (MotionStatus::FALLEN != STANDUP){
			Motion::GetInstance()->GetUp();
			m_currentState = new GoalKeeperInit(); //TODO - check
		}

		m_currentState->Run();
		stateName = m_currentState->Name();
		stateVariable = m_currentState->GetStateVariable();
		if (m_currentState!=NULL)
			delete m_currentState;

		switch (stateName)
		{
		case GKState_Init:
			m_nextState = new GoalKeeperLookForBall();
			break;

		case GKState_LooKForBall:
			if (stateVariable == "BallFound")
			{
				//m_nextState = new GoalKeeperFollow();
			}
			break;

		default:
			m_nextState = NULL;
			break;
		}
		if (stateVariable == "FellDown"){
			m_nextState = new GoalKeeperLookForBall();
		}

		m_currentState = m_nextState;
	}
}
