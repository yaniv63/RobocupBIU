#include "GoToBall.h"

GoToBall::GoToBall()
{
	m_StateName = State_GoToBall;
}

GoToBall::~GoToBall()
{

}

void GoToBall::Run()
{
	float angleToBall = m_Motion->GetHeadTilt().Pan;
	cout<<"pan is " << angleToBall << endl;
	turnToBall(angleToBall);
	m_Motion->StartWalking();

	while (m_Vision->Ball->Get()->Distance() > 0 && MotionStatus::FALLEN == STANDUP)
	{
		//keep walking towards the ball
	}

	if (MotionStatus::FALLEN != STANDUP)
	{
		m_Motion->GetUp();
		m_stateVariable = "BallLost";
	}
	//Got to ball
	if (m_Vision->Ball->Get()->Distance() == 0)
	{
		m_stateVariable = "InKickDistance";
	}
	m_stateVariable = "InKickDistance";
}

void GoToBall::turnToBall(float angleToBall)
{
	if(angleToBall == 0)
		return;
	else if(angleToBall > 0)
		m_Motion->StartWalking(-5,0,30);
	else if(angleToBall < 0)
	{
		angleToBall = -angleToBall;
		m_Motion->StartWalking(-5,0,-30);
	}

	usleep(factor*angleToBall*30);
	m_Motion->StopWalking();
	m_Motion->SetHeadTilt(HeadTilt(0,0));
}




