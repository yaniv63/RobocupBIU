#include "LookForBall.h"
#include "../../BrainData.h"

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
	//usleep(3000*1000);
	while (!m_Vision->Ball->Get()->IsDetected && MotionStatus::FALLEN == STANDUP)
	{
		m_Motion->SetHeadTilt(HeadTilt(tilt, pan));

		usleep(400*1000);
		if (pan > PanMaxLeft)
		{
			pan = PanMaxRight;
			tilt += 22.5;
			if (tilt > -10)
			{
				m_stateVariable = "BallNotFound";
				return;
			}
		}
		pan += 25;

	}

	if (MotionStatus::FALLEN != STANDUP)
	{
		m_Motion->GetUp();
		m_stateVariable == "FellDown";
	}

	//Ball detected successfully
	WriteLine("Ball found");
	CenterBall();
	m_stateVariable = "BallFound";
	return;
}

void LookForBall::CenterBall()
{
	WriteLine("Centering ball");
	usleep(3000*1000);
	Point2f location = m_Vision->Ball->Get()->Location;
	float tilt = m_Motion->GetHeadTilt().Tilt;
	float pan = m_Motion->GetHeadTilt().Pan;

	//TODO- check when x==-1 and y==-1

	while (location.x < 300 || location.x > 340 || location.y < 220 || location.y > 260)
	{
		if (location.x < 300)
		{
			//turn head right
			pan +=1;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));

		}
		else if (location.x > 340)
		{
			//turn head left
			pan -=1;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		}

		if (location.y < 220)
		{
			//turn head up
			tilt +=1;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		}
		else if (location.y > 260)
		{
			//turn head down
			tilt -=1;
			m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		}
		usleep(200*1000);
		location = m_Vision->Ball->Get()->Location;
		cout << "x " << location.x <<endl;
		cout << "y " << location.y <<endl;

	}
}
