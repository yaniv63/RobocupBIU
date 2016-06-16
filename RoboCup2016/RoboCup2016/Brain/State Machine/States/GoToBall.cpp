#include "GoToBall.h"

GoToBall::GoToBall(float angleToBall)
{
	m_StateName = State_GoToBall;
	m_angleToBall = angleToBall;
}

GoToBall::~GoToBall()
{

}

void GoToBall::Run()
{
	m_Motion->StartWalking();
	while (m_Vision->Ball->Get()->Distance() > 0 && MotionStatus::FALLEN == STANDUP)
	{
		//keep walking towards the ball
	}
	if (MotionStatus::FALLEN != STANDUP)
	{
		m_Motion->GetUp();
		m_stateVariable == "BallLost";
	}
	//Got to ball
	if (m_Vision->Ball->Get()->Distance() == 0)
	{
		m_stateVariable == "InKickDistance";
	}

}




