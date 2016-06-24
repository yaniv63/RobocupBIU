/*
 * GoalKeeperFollowBall.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperFollowBall.h"


GoalKeeperFollowBall::GoalKeeperFollowBall() {
	m_GKStateName = GKState_FollowBall;
	MIN_MS_TO_JUMP = 2500;
}

GoalKeeperFollowBall::GoalKeeperFollowBall(DetectedBall* GKdetectedBall) {
	m_GKStateName = GKState_FollowBall;
	m_GKdetectedBall = GKdetectedBall;
	MIN_MS_TO_JUMP = 2500;
}

GoalKeeperFollowBall::~GoalKeeperFollowBall() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperFollowBall::Run(){

	BallMovement ballMovement = vision.GetBallMovement();
	while ((ballMovement.status != "valid") || (ballMovement.MsToJump > MIN_MS_TO_JUMP)){
		ballMovement = vision.GetBallMovement(m_GKdetectedBall);
	}

	switch (ballMovement.Direction){
		case "Left":
			stateVariable == "JumpLeft";
			break;
		case "Right":
			stateVariable == "JumpRight";
			break;
		case "Wave":
			stateVariable == "Wave";
			break;
		default:
			stateVariable == "None";
			break;
	}
}
