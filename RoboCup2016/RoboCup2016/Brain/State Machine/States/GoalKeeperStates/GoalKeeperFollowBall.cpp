/*
 * GoalKeeperFollowBall.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperFollowBall.h"


GoalKeeperFollowBall::GoalKeeperFollowBall():MIN_MS_TO_JUMP(2500) {
	m_GKStateName = GKState_FollowBall;
	Vision::GetInstance()->StartBallMovementCalc();
}

GoalKeeperFollowBall::~GoalKeeperFollowBall() {
	Vision::GetInstance()->StopBallMovementCalc();
}

void GoalKeeperFollowBall::Run(){
	cout <<"in GoalKeeperFollowBall "<<endl;
	int cnt_try=0;
	BallMovement ballMovement = Vision::GetInstance()->BallMovementCalc.SafeRead();
	while (((ballMovement.DetectionStatus != Valid) || (ballMovement.MsToJump > MIN_MS_TO_JUMP)) && (cnt_try<4)){
		cnt_try++;
		ballMovement = Vision::GetInstance()->BallMovementCalc.SafeRead();
	}

	switch (ballMovement.Direction){
		case LEFT:
			m_GKStateVariable == "JumpLeft";
			break;
		case RIGHT:
			m_GKStateVariable == "JumpRight";
			break;
		case CENTER:
			m_GKStateVariable == "Wave";
			break;
		default: /*also covers if cnt_try is bigger than 3*/
			m_GKStateVariable == "None";
			break;
	}
}
