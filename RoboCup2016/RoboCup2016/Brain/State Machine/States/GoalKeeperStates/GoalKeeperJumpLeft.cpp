/*
 * GoalKeeperJumpLeft.cpp
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#include "GoalKeeperJumpLeft.h"

GoalKeeperJumpLeft::GoalKeeperJumpLeft() {
	m_GKStateName = GKState_JumpLeft;
}

GoalKeeperJumpLeft::~GoalKeeperJumpLeft() {
	// TODO Auto-generated destructor stub
}

void GoalKeeperJumpLeft::Run(){
	cout <<"in GoalKeeperJumpLeft Run "<<endl;
	//Action::GetInstance()->Start(Page_JumpToTheLeft);
	//m_Motion->WaitForActionFinish();
}
