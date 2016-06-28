#ifndef _State_H_
#define _State_H_

#include "../../Motion/Motion.h"
#include "../../Vision/Vision.h"
#include "../../Log/Log.h"

enum StateName
{
	State_Init,
	State_GoToBall,
	State_ChangeSpot,
	State_Kick,
	State_LookForBall,
	State_LookForGoal,
	State_Finished
};


enum StatePageNum
{
	Page_Init,
	Page_GoForBall,
	Page_ChangeSpot,
	Page_Kick = 4,
	Page_LookForBall,
	Page_LookForGoal,
	Page_JumpToTheRight,
	Page_JumpToTheLeft,
	Page_Wave
};

enum LookDirection
{
	DIR_Straight,
	DIR_Down,
	DIR_Left,
	DIR_Right
};

static const int TiltMax = 10;
static const int TiltMin = -55;
static const int PanMaxLeft = 70;
static const int PanMaxRight = -70;

class State
{
public:
	virtual ~State();

	virtual void Run() = 0;

	virtual string NameToString();

	StateName Name();

	virtual string GetStateVariable();

	string StateNamesStrings[6] = {"Init",
								"GoToBall",
								"ChangeSpot",
								"Kick",
								"LookForBall",
								"LookForGoal"};

protected:
	State();

	Motion* m_Motion;
	Vision* m_Vision;

	void WriteLine(string message);
	StateName m_StateName;
	string m_stateVariable;

private:
	Log* Logger;
};

#endif


