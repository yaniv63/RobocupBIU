/*
 * GoalKeeperStateMachine.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#include "GoalKeeperStateMachine.h"

GoalKeeperStateMachine::GoalKeeperStateMachine() {
	m_GKCurrentState = new GoalKeeperInit();
}

GoalKeeperStateMachine::~GoalKeeperStateMachine() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperStateMachine::Run()
{
	StateName stateName;
	string stateVariable = "";
	while (m_GKCurrentState != NULL)
	{
		if (MotionStatus::FALLEN != STANDUP){
			Motion::GetInstance()->GetUp();
			m_GKCurrentState = new GoalKeeperInit(); //TODO - check
		}

		m_GKCurrentState->Run();
		stateName = m_GKCurrentState->Name();
		stateVariable = m_GKCurrentState->GetStateVariable();

		switch (stateName)
		{
		case GKState_Init:
			m_GKNextState = new GoalKeeperLookForBall();
			break;

		case GKState_LooKForBall:
			if (stateVariable == "BallFound")
			{   GoalKeeperLookForBall* lookForballPtr = dynamic_cast<GoalKeeperLookForBall*> (m_GKCurrentState);
				DetectedBall* ball  = lookForballPtr ->GetDetectedBall();
				m_GKNextState = new GoalKeeperFollowBall(ball);
			}
			break;

		case GKState_FollowBall:
			if 		 (stateVariable == "JumpLeft")	m_GKNextState = new GoalKeeperJumpLeft();
			else if  (stateVariable == "JumpRight")	m_GKNextState = new GoalKeeperJumpRight();
			else if	 (stateVariable == "Wave")		m_GKNextState = new GoalKeeperWave();
			else /*None*/ m_GKNextState = new GoalKeeperLookForBall();
			break;

		case GKState_JumpRight:
			/*Motion jump to right - complete*/
			m_GKNextState = new GoalKeeperGetUpFromRight();
			break;

		case GKState_JumpLeft:
			/*Motion jump to left - complete*/
			m_GKNextState = new GoalKeeperGetUpFromLeft();
			break;

		case GKState_Wave:
			/*Motion wave or bend/sit - need to complete*/
			m_GKNextState = new GoalKeeperGetUpStraight();
			break;

		case GKState_GetUpFromRight:
			/*Motion get up + return to goal center- need to complete*/
			m_GKNextState = new GoalKeeperLookForBall();
			break;

		case GKState_GetUpFromLeft:
			/*Motion get up + return to goal center- need to complete*/
			m_GKNextState = new GoalKeeperLookForBall();
			break;

		case GKState_GetUpStraight:
			/*Motion get up - need to complete*/
			m_GKNextState = new GoalKeeperLookForBall();
			break;

		default:
			m_GKNextState = NULL;
			break;
		}
		if (stateVariable == "FellDown"){
			m_GKNextState = new GoalKeeperLookForBall();
		}

		if (m_GKCurrentState!=NULL)
					delete m_GKCurrentState;
		m_GKCurrentState = m_GKNextState;
	}
}
