#include "BallDetector.h"
#include <deque>
using namespace std;

//std::queue<Point2f> maxCentersForRecursiveFilter;
//std::deque<Point2f> maxCentersForRecursiveFilter;
//std::deque<Point2f> maxCentersForRecursiveFilter;
//list<Point2f> maxCentersForRecursiveFilter;
BallDetector::BallDetector()
{
	m_BallDistanceCalculator = new BallDistanceCalculator();
}

BallDetector::~BallDetector()
{
	delete m_BallDistanceCalculator;
}

DetectedObject* BallDetector::DetectObject(Mat& inputImageHSV)
{
	Mat field; // B&W image represent the field.
	if (!FindField(inputImageHSV, field))
	{
		// Cannot find field - Ball not found.
		return new DetectedBall();
	}

	Mat onlyGreenImage;
	inRange(inputImageHSV, Calibration::GetInstance()->MinGreenHSV, Calibration::GetInstance()->MaxGreenHSV, onlyGreenImage);
	Mat nonGreenImage;
	bitwise_not(onlyGreenImage, nonGreenImage);

	Mat holesInField;
	bitwise_and(nonGreenImage, field, holesInField);

	// Smooth image with blur and closing
	medianBlur(holesInField, holesInField, 15);
	CloseImage(holesInField, holesInField);

	ImageShowOnDebug("Holes in field", "BallDetector", holesInField);
	vector< vector<Point> > contoursVector = FindContours(holesInField, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if (contoursVector.size() == 0)
	{
		return new DetectedBall();
	}

	vector<Point2f> centersVector;
	centersVector.resize(contoursVector.size());
	vector<float> radiusesVector;
	radiusesVector.resize(contoursVector.size());

	float maxRatio = -1.0f;
	int maxRatioIndex = -1;

	// Get the contour with the maximum ratio between its area to the area of its enclosing circle.
	for (size_t i = 0; i < contoursVector.size(); i++)
	{
		minEnclosingCircle(contoursVector[i], centersVector[i], radiusesVector[i]);
		circle(holesInField, centersVector[i], radiusesVector[i], Colors::Blue, 2);

		double holeArea = contourArea(contoursVector[i]);
		double enclosingCircleArea = M_PI * pow(radiusesVector[i], 2);
		double ratio = holeArea / enclosingCircleArea;

		const float rationThreshold = 0.7;
		const float radiusThreshold = 10;
		if (ratio < rationThreshold || radiusesVector[i] < radiusThreshold)
		{
			continue;
		}

		if (maxRatio < ratio)
		{
			maxRatio = ratio;
			maxRatioIndex = i;
		}
	}
	// TODO Auto-generated destructor stub
	if (maxRatioIndex == -1)
	{
		return new DetectedBall();
	}

	if (radiusesVector[maxRatioIndex] < 0.0f || radiusesVector[maxRatioIndex] > 10000.0f)
	{
		printf("Radius is corrupt in BallDetector.FindBall()");
	}

	ImageShowOnDebug("HolesInField", "BallDetector", holesInField);

	float tilt = Motion::GetInstance()->GetHeadTilt().Tilt;
	float distance = m_BallDistanceCalculator->CalculateDistance(centersVector[maxRatioIndex], radiusesVector[maxRatioIndex], (int)tilt);
	return new DetectedBall(centersVector[maxRatioIndex], radiusesVector[maxRatioIndex], distance);


//	if (maxCentersForRecursiveFilter.size() < DEQUE_SIZE) {
//		maxCentersForRecursiveFilter.push_back(max_center);

//	} else {
//		int sumOfxAxis = 0;
//		int sumOfyAxis = 0;
//		maxCentersForRecursiveFilter.pop_front();
//		maxCentersForRecursiveFilter.push_back(max_center);
//		std::deque<Point2f>::iterator it = maxCentersForRecursiveFilter.begin();
//		int i = DEQUE_SIZE;
//		while (it != maxCentersForRecursiveFilter.end()) {
//			sumOfxAxis = (it._M_cur->x + sumOfxAxis) / i;
//			sumOfyAxis = (it._M_cur->y + sumOfyAxis) / i;
//			it++;
//			i++;
//		}
//		max_center.x = sumOfxAxis;
//		max_center.y = sumOfyAxis;
//		maxCentersForRecursiveFilter.pop_front();
//		maxCentersForRecursiveFilter.push_back(max_center);
//	}
}
