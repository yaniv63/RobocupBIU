#include "LookForBall.h"

LookForBall::LookForBall()
{
	m_StateName = State_LookForBall;
	m_lookDirection = DIR_Straight;
}

LookForBall::~LookForBall()
{
}

void LookForBall::Run()
{
	float pan;
	float tilt;

	tilt = TiltMin;
	pan = PanMaxRight;
	usleep(3000*1000);
	while (!m_Vision->Ball->Get()->IsDetected && MotionStatus::FALLEN == STANDUP)
	{
		m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		usleep(400*1000);
		if (pan > PanMaxLeft)
		{
			pan = PanMaxRight;
			tilt += 20;
			if (tilt > -10)
			{
				m_stateVariable = "BallNotFound";
				return;
			}
		}
		pan += 15;

	}

	if (MotionStatus::FALLEN != STANDUP)
	{
		m_Motion->GetUp();
		m_stateVariable == "FellDown";
	}

	//Ball detected succesfully
	WriteLine("Ball found");
	CenterBall();
	angleToBall = m_Motion->GetHeadTilt().Pan;
	m_stateVariable = "BallFound";
	return;
}

void LookForBall::CenterBall()
{
	WriteLine("Centering ball");
	Point2f location = m_Vision->Ball->Get()->Location;
	float tilt = m_Motion->GetHeadTilt().Tilt;
	float pan = m_Motion->GetHeadTilt().Pan;

	//TODO- check when x==-1 and y==-1

	while (location.x < 600 || location.x > 680 || location.y < 320 || location.y > 400)
	{
		if (location.x < 600)
		{
			//turn head left
			pan -=5;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));

		}
		else if (location.x > 680)
		{
			//turn head right
			pan +=5;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		}

		if (location.y < 320)
		{
			//turn head down
			tilt -=5;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		}
		else if (location.y > 400)
		{
			//turn head up
			tilt +=5;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		}
		location = m_Vision->Ball->Get()->Location;
		cout << "x " << location.x <<endl;
		cout << "y " << location.y <<endl;
		usleep(3000*1000);
	}
}


