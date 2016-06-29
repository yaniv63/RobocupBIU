#include "Kick.h"

Kick::Kick(bool isGoalFound)
{
	m_StateName = State_Kick;
	m_isGoalFound = isGoalFound;
}

Kick::~Kick()
{
}

void Kick::Run()
{
	//If goal is found
	if (m_isGoalFound)
		turnToGoal(); //turn around the ball to face the goal
	//kick

	m_Motion->RunAction(ActionPage::RightKick);
}

void Kick::turnToGoal()
{/*
	///// FACING ENEMY GOAL ////////
	if((DegreeBetweenRobotAndGoal >= 0 && DegreeBetweenRobotAndGoal <= 90))
	{
		//Right circle around the ball - Left turn of -Degree
	}
	else if((DegreeBetweenRobotAndGoal < 0 && DegreeBetweenRobotAndGoal >= -90))
	{
		//Left circle around the ball - Right turn of Degree
	}

	///// FACING OWN GOAL ///////
	else if((DegreeBetweenRobotAndGoal > 90 && DegreeBetweenRobotAndGoal <= 180))
	{
		//Right circle around the ball - Left turn of -Degree
	}
	else if((DegreeBetweenRobotAndGoal < -90 && DegreeBetweenRobotAndGoal >= -180))
	{
		//Left circle around the ball - Right turn of Degree

	}*/


}





