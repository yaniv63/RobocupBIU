/*
 * BallMovement.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPER_BALLMOVEMENT_H_
#define BRAIN_GOALKEEPER_BALLMOVEMENT_H_

#include "../../Vision/VisionUtils.h"

enum JumpDirection {
	NONE,
	LEFT,
	RIGHT,
	CENTER
};

static const char* JumpDirectionStrings[] = { "None", "Left", "Right", "Center" };

enum Status {
	Valid,
	BallNotDetected,
	InvalidTimeDiff
};

static const char* StatusStrings[] = { "Valid", "BallNotDetected", "InvalidTimeDiff" };

class BallMovement
{
public:
	BallMovement(Status status);
	BallMovement(JumpDirection direction, int msToJump);
	void PrintDetailsOnImage(Mat image);

	JumpDirection Direction;
	int MsToJump;
	Status DetectionStatus;

	static BallMovement BallNotDetected()
	{
		BallMovement ballMovement(Status::BallNotDetected);
		return ballMovement;
	}

	static BallMovement InvalidTimeDiff()
	{
		BallMovement ballMovement(Status::InvalidTimeDiff);
		return ballMovement;
	}
};

#endif /* BRAIN_GOALKEEPER_BALLMOVEMENT_H_ */
