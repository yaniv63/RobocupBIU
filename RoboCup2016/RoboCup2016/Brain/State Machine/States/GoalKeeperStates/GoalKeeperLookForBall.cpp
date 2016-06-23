/*
 * GoalKeeperLookForBall.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#include "GoalKeeperLookForBall.h"

GoalKeeperLookForBall::GoalKeeperLookForBall() {
	m_GKStateName = GKState_LooKForBall;
}

GoalKeeperLookForBall::~GoalKeeperLookForBall() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperLookForBall::Run(){
	float pan;
	float tilt;

	tilt = -15;
	pan = 0;
	usleep(3000*1000);
	while (!m_Vision->Ball->Get()->IsDetected && MotionStatus::FALLEN == STANDUP)
	{
		m_Motion->SetHeadTilt(HeadTilt(tilt, pan));
		usleep(400*1000);
		if (pan > PanMaxLeft){
			pan = PanMaxRight;
		}
		else pan += 15;
	}

	if (MotionStatus::FALLEN != STANDUP){
		m_Motion->GetUp();
		m_GKStateVariable == "FellDown";
	}
	else{
		//Ball detected succesfully
		//WriteLine("Ball found");
		//LookForBall::CenterBall();
		m_GKStateVariable = "BallFound";
		m_GKdetectedBall = (DetectedBall*)m_Vision->Ball->Get();
	}
	return;
}

DetectedBall* GoalKeeperLookForBall::GetDetectedBall() {
	return m_GKdetectedBall;
}