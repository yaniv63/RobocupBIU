/*
 * GoalKeeperFollowBall.h
 *
 *  Created on: Jun 23, 2016
 *      Author: root
 */

#ifndef BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERFOLLOWBALL_H_
#define BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERFOLLOWBALL_H_

#include "../../GoalKeeperState.h"


class GoalKeeperFollowBall: public GoalKeeperState
{
public:
	GoalKeeperFollowBall();
	virtual ~GoalKeeperFollowBall();
	virtual void Run();
};


#endif /* BRAIN_STATE_MACHINE_STATES_GOALKEEPERSTATES_GOALKEEPERFOLLOWBALL_H_ */
