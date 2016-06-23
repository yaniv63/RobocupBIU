/*
 * GoalKeeperFollowBall.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperFollowBall.h"


GoalKeeperFollowBall::GoalKeeperFollowBall() {
	m_GKStateName = GKState_FollowBall;
}

GoalKeeperFollowBall::GoalKeeperFollowBall(DetectedBall* GKdetectedBall) {
	m_GKStateName = GKState_FollowBall;
	m_GKdetectedBall = GKdetectedBall;
}

GoalKeeperFollowBall::~GoalKeeperFollowBall() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperFollowBall::Run(){

}
