#include "StateMachine.h"
#include "States/Init.h"
#include "States/ChangeSpot.h"
#include "States/LookForBall.h"
#include "States/GoToBall.h"
#include "States/Kick.h"
#include "States/LookForGoal.h"

StateMachine::StateMachine()
{
	m_currentState = new Init();
}

StateMachine::StateMachine(State* state){
	m_currentState = state;
}

StateMachine::~StateMachine()
{
}

void StateMachine::SetCurrent(State* state)
{
	m_currentState = state;
}

void StateMachine::Run()
{
	StateName stateName;
	string stateVariable = "";
	while (m_currentState != NULL)
	{
		if (MotionStatus::FALLEN != STANDUP)
		{
			Motion::GetInstance()->GetUp();
			m_currentState = new LookForBall(); //TODO - check
		}

		m_currentState->Run();
		stateName = m_currentState->Name();
		stateVariable = m_currentState->GetStateVariable();
		cout << stateName << endl;
		switch (stateName)
		{
		case State_Init:
			m_nextState = new LookForBall();
			break;

		case State_LookForGoal:
			if (stateVariable == "GoalFound")
			{
				m_nextState = new LookForBall();
			}
			else if (stateVariable == "GoalNotFound")
			{
				//keep looking
				m_nextState = new ChangeSpot();
			}
			break;

		case State_LookForBall:
			if (stateVariable == "BallFound")
			{
				m_nextState = new GoToBall();
			}
			else if (stateVariable == "BallNotFound")
			{
				//keep looking
				m_nextState =  new ChangeSpot();
			}
			else if(stateVariable == "FellDown")
			{
				m_nextState = new LookForBall();
			}
			break;

		case State_GoToBall:
			if (stateVariable == "InKickDistance")
			{
				m_nextState = new Kick();
			}
			else if (stateVariable == "BallLost")
			{
				m_nextState = new LookForBall();
			}
			break;

		case State_Kick:
			m_nextState = NULL;
			//m_nextState = new LookForBall();
			break;

		case State_ChangeSpot:
			m_nextState = new LookForBall();
			break;

		default:
			m_nextState = NULL;
			break;
		}
		m_currentState = m_nextState;
	}
}
