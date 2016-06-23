/*
 * BallMovement.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#include "BallMovement.h"

BallMovement::BallMovement(JumpDirection direction, int msToJump)
	: Direction(direction), MsToJump(msToJump), DetectionStatus(Status::Valid)
{
}

BallMovement::BallMovement(Status status)
	: Direction(JumpDirection::NONE), MsToJump(-1), DetectionStatus(status)
{

}

void BallMovement::PrintDetailsOnImage(Mat image)
{
	char message[256];
	PrintMessageOnImage(image, "Ball Movement details:", Point(300,30));

	sprintf(message, "Direction: %s", JumpDirectionStrings[Direction]);
	PrintMessageOnImage(image, message, Point(300,50));

	sprintf(message, "Time to jump: %f", MsToJump);
	PrintMessageOnImage(image, message, Point(300,70));

	sprintf(message, "Status: %s", StatusStrings[DetectionStatus]);
	PrintMessageOnImage(image, message, Point(300,90));
}
