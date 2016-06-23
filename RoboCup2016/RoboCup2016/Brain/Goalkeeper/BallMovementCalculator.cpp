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
	if(!timedDetectedBall.IsDetected)
	{
		return BallMovement::BallNotDetected();
	}

	PushDetectedBallToQueue(timedDetectedBall);

	if (!IsTimeDiffValid())
	{
		return BallMovement::InvalidTimeDiff();
	}

	LineApprox ballMovementApprox = LeastSquaresApproximation(GetTValues(), GetBValues());

	JumpDirection jumpDirection = CalculateDirection(ballMovementApprox);
	int msToJump = CalculateJumpingTime();

	BallMovement ballMovement(jumpDirection, msToJump);
	return ballMovement;
}

void BallMovementCalculator::PushDetectedBallToQueue(TimedDetectedBall timedDetectedBall)
{
	for (int i = NUM_OF_SAMPLES - 1; i > 0 ; i--)
	{
		m_SamplesArray[i] = m_SamplesArray[i-1];
	}

	m_SamplesArray[0] = timedDetectedBall;
}

bool BallMovementCalculator::IsTimeDiffValid()
{
	for (int i = 0 ; i < NUM_OF_SAMPLES - 1 ; i++)
	{
		if (m_SamplesArray[i].usDetectionTime == -1
				|| m_SamplesArray[i].usDetectionTime
				- m_SamplesArray[i+1].usDetectionTime > MAX_TIME_DIFF/NUM_OF_SAMPLES)
		{
			return false;
		}
	}

	return true;
}

vector<float> BallMovementCalculator::GetTValues()
{
	vector<float> result;
	for (int i = 0 ; i < NUM_OF_SAMPLES ; i++)
		{
			result.push_back(m_SamplesArray[i].Location.y);
		}
	return result;
}

vector<float> BallMovementCalculator::GetBValues()
{
	vector<float> result;
		for (int i = 0 ; i < NUM_OF_SAMPLES ; i++)
			{
				result.push_back(m_SamplesArray[i].Location.x);
			}
		return result;
}

float BallMovementCalculator::SumVector(vector<float> vector)
{
	float result = 0;
	for (float i : vector)
	{
		result += i;
	}
	return result;
}

float BallMovementCalculator::SumVectorPower2(vector<float> vector)
{
	float result = 0;
	for (float i : vector)
	{
		result += pow(i,2);
	}
	return result;
}

float BallMovementCalculator::SumMultiplyVectors(vector<float> v1, vector<float> v2)
{
	float result = 0;
	for (int i = 0 ; i < NUM_OF_SAMPLES ; i++)
	{
		result += v1[i] * v2[i];
	}
	return result;
}

LineApprox BallMovementCalculator::LeastSquaresApproximation(vector<float> tVector, vector<float> bVector)
{
	float sumTi = SumVector(tVector);
	float sumTi2 = SumVectorPower2(tVector);
	float sumBi = SumVector(bVector);
	float sumTiBi = SumMultiplyVectors(tVector, bVector);

	float AAtData[4] = {NUM_OF_SAMPLES, sumTi, sumTi, sumTi2};
	Mat AAt(2,2,CV_32F,AAtData);
	//cout << "AAt: " << AAt << endl;

	float AtBData[2] = {sumBi, sumTiBi};
	Mat AtB(2,1,CV_32F,AtBData);
	//cout << "AtB: " << AtB << endl;

	Mat AAtInv = AAt.inv();
	//cout << "AAt inverse: " << AAtInv << endl;

	Mat CD = AAtInv * AtB;
	cout <<  "CD: " << CD << endl;

	return LineApprox(CD.at<float>(0), CD.at<float>(1));
}

JumpDirection BallMovementCalculator::CalculateDirection(LineApprox ballMovementApprox)
{
	// The x value that the ball will cross the bottom boundary of the frame will determine the jump direction.
	float approxX = ballMovementApprox.GetFit(FRAME_HEIGHT - 1);
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

int BallMovementCalculator::CalculateJumpingTime()
{

}
