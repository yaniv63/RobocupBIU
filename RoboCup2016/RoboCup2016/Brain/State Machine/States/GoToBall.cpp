#include "GoToBall.h"
#include <cstdio>
#include <ctime>
#include "../../BrainData.h"

using namespace std;

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
	m_Motion->TurnByAngle(angleToBall); //turn to ball
	//start timing
	clock_t startingTime;
	double duration = 0;
	startingTime = clock();

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

	if (m_Vision->Ball->Get()->Distance() == -1)
	{
		//pause timing
		duration += (clock() - startingTime ) / (double) CLOCKS_PER_SEC;

		m_Motion->StopWalking();

		m_Motion->RunAction(ActionPage::BendToBall);
		if (m_Vision->Ball->Get()->IsDetected)
		{
			//restart timing
			startingTime = clock();
			m_Motion->StartWalking(DISTANCE_WHEN_LOSING_BALL);
			m_Motion->StopWalking();
			m_stateVariable = "InKickDistance";
		}
		else
		{
			m_stateVariable = "BallLost";
		}
	}

	//stop timing
	duration += (clock() - startingTime) / (double)CLOCKS_PER_SEC;
	BrainData::GetInstance()->UpdateAll(duration, angleToBall);
}




