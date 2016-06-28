/*
 * BallMovementCalculator.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPER_BALLMOVEMENTCALCULATOR_H_
#define BRAIN_GOALKEEPER_BALLMOVEMENTCALCULATOR_H_

#define MAX_TIME_DIFF 1000000
#define MIN_TIME_DIFF 1000
#define MIN_LOCATION_DIFF 10
#define CENTER_MARGIN 10

#include "BallMovement.h"
#include "../../Vision/DetectedObjects/TimedDetectedBall.h"
#include "LeastSquareApproximation/ParabolaApprox.h"
#include "LeastSquareApproximation/LineApprox.h"

class BallMovementCalculator {
public:
	BallMovementCalculator();
	virtual ~BallMovementCalculator();

	BallMovement CalculateBallMovement(TimedDetectedBall timedDetectedBall);
	void ResetSamples();

private:
	void PushDetectedBallToQueue(TimedDetectedBall timedDetectedBall);
	JumpDirection CalculateDirection(LeastSquareApprox* directionApprox);
	double CalculateJumpingTime(LeastSquareApprox* timingApprox);

	bool IsNewSampleRelevant(TimedDetectedBall timedDetectedBall);
	bool IsTimeDiffValid();
	bool IsLocationDiffValid();
	bool IsEnoughSamples();

	double GetRelativeTimeToFirstSample(double detectionTime);

	TimedDetectedBall m_SamplesArray[NUM_OF_SAMPLES];

	vector<double> GetYValues();
	vector<double> GetXValues();
	vector<double> GetTimes();

	void PrintCalculationData(Mat& image);
};

#endif /* BRAIN_GOALKEEPER_BALLMOVEMENTCALCULATOR_H_ */
