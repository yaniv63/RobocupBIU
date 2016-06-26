#include "ChangeSpot.h"

ChangeSpot::ChangeSpot()
{
	m_StateName = State_ChangeSpot;
}

ChangeSpot::~ChangeSpot()
{

}

void ChangeSpot::Run()
{
	cout << "changed spot" << endl;
//	writeToLog(logFile, "State == ChangeSpot");

	int degree = 45;

	//turn 45 deg to the left
	m_Motion->StartWalking(-5,0,-24);
	usleep(1388.89*degree*24);
	m_Motion->StopWalking();
	m_Motion->SetHeadTilt(HeadTilt(0,0));
}






