#include "../DetectedObjects/DetectedBall.h"
#include "Detector.h"

#ifndef BALLDETECTOR_H_
#define BALLDETECTOR_H_

static const int DEQUE_SIZE = 10;

class BallDetector : public Detector
{
public:
	BallDetector();
	~BallDetector();

	virtual DetectedObject* DetectObject(Mat& inputImage);

	static inline DetectedObject* GetDefault() {return new DetectedBall();}

private:
	BallDistanceCalculator* m_BallDistanceCalculator;
};

#endif /* BALLDETECTOR_H_ */
