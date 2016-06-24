/*
 * BallMovementCalculator.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#include "BallMovementCalculator.h"

BallMovementCalculator::BallMovementCalculator() {
	// TODO Auto-generated constructor stub

}

BallMovementCalculator::~BallMovementCalculator() {
	// TODO Auto-generated destructor stub
}

BallMovement BallMovementCalculator::CalculateBallMovement(TimedDetectedBall timedDetectedBall)
{
	Mat image(480,640, CV_8UC3, Scalar(0,0,0));
	if(DEBUG_MODE)
	{
		PrintCalculationData(image);
	}

	if (!timedDetectedBall.IsDetected)
	{
		return BallMovement::BallNotDetected();
	}

	if (IsNewSampleRelevant(timedDetectedBall))
	{
		PushDetectedBallToQueue(timedDetectedBall);
	}

	if (!IsEnoughSamples())
	{
		return BallMovement::NotEnoughSamples();
	}

	if (!IsTimeDiffValid())
	{
		return BallMovement::InvalidTimeDiff();
	}
//
//	if (!IsLocationDiffValid())
//	{
//		return BallMovement::InvalidLocationDiff();
//	}

	LeastSquareApprox* ballMovementApprox = LineApprox::Approx(GetYValues(), GetXValues());
	JumpDirection jumpDirection = CalculateDirection(ballMovementApprox);

	LeastSquareApprox* timingApprox = ParabolaApprox::Approx(GetYValues(), GetTimes());
	float msToJump = CalculateJumpingTime(timingApprox);

	BallMovement ballMovement(jumpDirection, msToJump);

	if(DEBUG_MODE)
	{
		PrintCalculationData(image);
		ballMovement.PrintDetailsOnImage(image, Point(0, (NUM_OF_SAMPLES + 1)*20));
	}
	imshow("Ball movement calc", image);

	return ballMovement;
}

void BallMovementCalculator::PrintCalculationData(Mat& image)
{
	char message[256];
	for (int i = 0 ; i < NUM_OF_SAMPLES ; i++)
	{
		TimedDetectedBall currentBall = m_SamplesArray[i];
		circle(image, currentBall.Location, 5, Scalar(255,0,0), 2);
		sprintf(message, "%d", i);
		PrintMessageOnImage(image, message, Point(currentBall.Location.x,currentBall.Location.y - 10), 0.9);
		sprintf(message, "%d - (%.2f,%.2f) - %f", i, currentBall.Location.x, currentBall.Location.y, GetRelativeTimeToFirstSample(currentBall.DetectionTime));
		PrintMessageOnImage(image, message, Point(0,(i+1)*20), 0.9);
	}
	ImageShowOnDebug("BallMove", "Vision", image);
}

void BallMovementCalculator::PushDetectedBallToQueue(TimedDetectedBall timedDetectedBall)
{
	for (int i = NUM_OF_SAMPLES - 1; i > 0 ; i--)
	{
		m_SamplesArray[i] = m_SamplesArray[i-1];
	}

	m_SamplesArray[0] = timedDetectedBall;
}

double BallMovementCalculator::GetRelativeTimeToFirstSample(double detectionTime)
{
	return (detectionTime - m_SamplesArray[NUM_OF_SAMPLES-1].DetectionTime)*1000;
}

bool BallMovementCalculator::IsNewSampleRelevant(TimedDetectedBall timedDetectedBall)
{
	TimedDetectedBall newestSample = m_SamplesArray[0];
	if(!newestSample.IsDetected)
	{
		return true;
	}

	if (abs(timedDetectedBall.Location.y - newestSample.Location.y) < MIN_LOCATION_DIFF)
	{
		return false;
	}

	if (timedDetectedBall.Location.y - newestSample.Location.y < MIN_LOCATION_DIFF
		&& timedDetectedBall.DetectionTime - newestSample.DetectionTime < 2)
	{
		return false;
	}

	return true;
}

bool BallMovementCalculator::IsTimeDiffValid()
{
	for (int i = 0 ; i < NUM_OF_SAMPLES_TO_APPROX ; i++)
	{
		if (m_SamplesArray[i].DetectionTime
				- m_SamplesArray[i+1].DetectionTime > MAX_TIME_DIFF)
		{
			return false;
		}
	}

	return true;
}

bool BallMovementCalculator::IsEnoughSamples()
{
	for (int i = 0 ; i < NUM_OF_SAMPLES ; i++)
	{
		if (!m_SamplesArray[i].IsDetected)
		{
			return false;
		}
	}

	return true;
}

bool BallMovementCalculator::IsLocationDiffValid()
{
	if (m_SamplesArray[0].IsDetected == false ||
		m_SamplesArray[0].Location.y - m_SamplesArray[NUM_OF_SAMPLES - 1].Location.y < MIN_LOCATION_DIFF)
	{
		return false;
	}

	return true;
}

vector<double> BallMovementCalculator::GetYValues()
{
	vector<double> result;
	for (int i = 0 ; i < NUM_OF_SAMPLES_TO_APPROX ; i++)
		{
			result.push_back(m_SamplesArray[i].Location.y);
		}
	return result;
}

vector<double> BallMovementCalculator::GetXValues()
{
	vector<double> result;
		for (int i = 0 ; i < NUM_OF_SAMPLES_TO_APPROX ; i++)
			{
				result.push_back(m_SamplesArray[i].Location.x);
			}
		return result;
}

vector<double> BallMovementCalculator::GetTimes()
{
	vector<double> result;
	for (int i = 0; i < NUM_OF_SAMPLES_TO_APPROX; i++)
	{
		result.push_back(GetRelativeTimeToFirstSample(m_SamplesArray[i].DetectionTime));
	}
	return result;
}

JumpDirection BallMovementCalculator::CalculateDirection(LeastSquareApprox* directionApprox)
{
	// The x value that the ball will cross the bottom boundary of the frame will determine the jump direction.
	double approxX = directionApprox->GetB(FRAME_HEIGHT - 1);

	if (approxX > FRAME_WIDTH * 2 || approxX < -FRAME_WIDTH)
	{
		return JumpDirection::NONE;
	}
	if (approxX > (FRAME_WIDTH/2 + CENTER_MARGIN))
	{
		return JumpDirection::RIGHT;
	}
	if (approxX < (FRAME_WIDTH/2 - CENTER_MARGIN))
	{
		return JumpDirection::LEFT;
	}
	return JumpDirection::CENTER;
}

double BallMovementCalculator::CalculateJumpingTime(LeastSquareApprox* timingApprox)
{
	return timingApprox->GetB(FRAME_HEIGHT - 1) - GetRelativeTimeToFirstSample(m_SamplesArray[0].DetectionTime);
}
