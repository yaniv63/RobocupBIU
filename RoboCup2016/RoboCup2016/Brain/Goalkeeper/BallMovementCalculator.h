/*
 * BallMovementCalculator.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPER_BALLMOVEMENTCALCULATOR_H_
#define BRAIN_GOALKEEPER_BALLMOVEMENTCALCULATOR_H_

#define NUM_OF_SAMPLES 3
#define MAX_TIME_DIFF 1000000
#define CENTER_MARGIN 10

#include "BallMovement.h"
#include "Includes.h"
#include "../../Vision/DetectedObjects/TimedDetectedBall.h"
#include "LineApprox.h"

class BallMovementCalculator {
public:
	BallMovementCalculator();
	virtual ~BallMovementCalculator();

	BallMovement CalculateBallMovement(TimedDetectedBall timedDetectedBall);

private:
	void PushDetectedBallToQueue(TimedDetectedBall timedDetectedBall);
	JumpDirection CalculateDirection(LineApprox ballMovementApprox);
	int CalculateJumpingTime();
	bool IsTimeDiffValid();

	TimedDetectedBall m_SamplesArray[NUM_OF_SAMPLES];

	vector<float> GetTValues();
	vector<float> GetBValues();
	static float SumVector(vector<float> vector);
	static float SumVectorPower2(vector<float> vector);
	static float SumMultiplyVectors(vector<float> tVector, vector<float> bVector);

	static LineApprox LeastSquaresApproximation(vector<float> t, vector<float> b);
};

#endif /* BRAIN_GOALKEEPER_BALLMOVEMENTCALCULATOR_H_ */
