#include "BrainData.h"
#include <math.h>
#include "State Machine/States/LookForBall.h"

BrainData* BrainData::m_instance = NULL;

BrainData* BrainData::GetInstance()
{
    if(m_instance == NULL)
    {
    	m_instance = new BrainData();
        return m_instance;
    }
    else
    {
        return m_instance;
    }
}

BrainData::BrainData()
{
	DistanceToGoal = DEFAULT_DISTANCE_TO_GOAL; //center player
	posX = 0;
	posY = 0;
	DecideInitPos();
};

void BrainData::DecideInitPos()
{
	LookForBall* lookForBall = new LookForBall();
	lookForBall->Run();
	delete lookForBall;

	float pan = Motion::GetInstance()->GetHeadTilt().Pan;

	if (pan <5 || pan >-5)
	{
		AngleFromGoal = 0;
	}
	else if (pan >5)
	{
		CalcAngleToGoal(100,90); //right player
	}
	else
	{
		CalcDistanceToGoal(100,-90);//left player
	}
	DefaultDistanceToGoal = DistanceToGoal;
}

void BrainData::UpdatePos(double distanceWalked,double angleWalked)
{
	posY += cos(angleWalked)*distanceWalked;
	posX += sin(angleWalked)*distanceWalked;
}

double BrainData::CalcWalkedDistance(double timeWalked)
{
	return timeWalked * WALK_SPEED;
}

void BrainData::CalcAngleToGoal(double distanceWalked,double angleWalked)
{
	double originDistanceToGoal = DistanceToGoal;
	CalcDistanceToGoal(distanceWalked,angleWalked);
	AngleFromGoal = asin(originDistanceToGoal*sin(angleWalked)/DistanceToGoal);
	if (AngleFromGoal > 90)
	{
		AngleFromGoal = 180 - AngleFromGoal;
	}
	else if (AngleFromGoal < -90)
	{
		AngleFromGoal = -180 - AngleFromGoal;
	}
}

void BrainData::CalcDistanceToGoal(double distanceWalked,double angleWalked)
{
	double powDistance = pow(DistanceToGoal,2) + pow(distanceWalked,2) -2*DistanceToGoal*distanceWalked*cos(angleWalked);
	DistanceToGoal = sqrt(powDistance);
}

void BrainData::UpdateAll(double timeWalked, double angleWalked)
{
	double walkedDistance = CalcWalkedDistance(timeWalked);
	UpdatePos(walkedDistance,angleWalked);
	CalcAngleToGoal(walkedDistance, angleWalked);
}

void BrainData::CalcWayBackToDefaultPos()
{
	DistanceToDefaultPos = sqrt(pow(posX,2) + pow(posY, 2));
	double a = DistanceToGoal;
	double b = DistanceToDefaultPos;
	double c = DefaultDistanceToGoal;
	double angleToDefaultPos_C = acos((pow(a,2) + pow(b,2) - pow(c,2))/(2*a*b));
	if (posX > 0)
	{
		AngleToDefaultPos = AngleFromGoal + angleToDefaultPos_C;
	}
	else
	{
		AngleToDefaultPos = AngleFromGoal - angleToDefaultPos_C;
	}
}

void BrainData::Reset()
{
	DistanceToGoal = DEFAULT_DISTANCE_TO_GOAL; //center player
	posX = 0;
	posY = 0;
	DecideInitPos();
}




