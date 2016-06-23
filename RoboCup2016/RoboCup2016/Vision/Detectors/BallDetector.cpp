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
	//Mat field; // B&W image represent the field.
	//if (!FindField(inputImageHSV, field))
	//{
	//	// Cannot find field - Ball not found.
	//	return new DetectedBall();
	//}
    //
	Mat houghCircleInput, houghCircleInputGray, greenMatrixImage, originalBGRImage;
	vector<Vec3f> circles;
	cvtColor(inputImageHSV, greenMatrixImage, CV_HSV2BGR);
	cvtColor(greenMatrixImage, houghCircleInputGray, CV_BGR2GRAY);
	vector<Mat> splittedMat;
	split(greenMatrixImage,splittedMat);

	//imshow("spl1",splittedMat[0]);
	//imshow("spl2",splittedMat[1]);
	//imshow("spl3",splittedMat[2]);

	//imshow("inputImageHSV", inputImageHSV);
	//waitKey(0);
	//cvtColor(inputImageHSV, houghCircleInput, CV_HSV2BGR);
	//imshow("houghCircleInput", houghCircleInput);
	//waitKey(0);
	//cvtColor(houghCircleInput, houghCircleInputGray, CV_BGR2GRAY);
	//imshow("houghCircleInputGray", houghCircleInputGray);
	//waitKey(0);
	//splittedMat[0] = splittedMat[1] - splittedMat[2];
	//HoughCircles(splittedMat[1], circles, CV_HOUGH_GRADIENT,2, splittedMat[1].rows/4, 200, 100);
	HoughCircles(houghCircleInputGray, circles, CV_HOUGH_GRADIENT,6, houghCircleInputGray.rows/4, 200, 100);
	cout<<"circles size: %d"<<circles.size();
	//Mat greenMatrixImage;
	Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
	int radius = cvRound(circles[0][2]);
    for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         //circle(greenMatrixImage, center, (float)radius, Colors::Blue, 2);
         circle( greenMatrixImage, center, 3, Scalar(0,255,0), -1, 8, 0 );
         circle( greenMatrixImage, center, radius, Scalar(0,0,255), 3, 8, 0 );
         // draw the circle outline
    }
    imshow("BallDetector: greenMatrixImage", greenMatrixImage);
    waitKey(10);
	//imshow("BallDetector: field", field);

	//Mat onlyGreenImage;
	//inRange(inputImageHSV, Calibration::GetInstance()->MinGreenHSV, Calibration::GetInstance()->MaxGreenHSV, onlyGreenImage);
	////imshow("BallDetector: onlyGreenImage", onlyGreenImage);
	////waitKey(0);
	//Mat nonGreenImage;
	//bitwise_not(onlyGreenImage, nonGreenImage);
	////imshow("BallDetector: nonGreenImage", nonGreenImage);
	////waitKey(0);
	//Mat holesInField;
	//bitwise_and(nonGreenImage, field, holesInField);
	////imshow("BallDetector: holesInField", holesInField);
	////waitKey(0);
	//// Smooth image with blur and closing
	//medianBlur(holesInField, holesInField, 15);
	//CloseImage(holesInField, holesInField);
	////imshow("BallDetector: holesInField after CloseImage", holesInField);
	////waitKey(0);
	//ImageShowOnDebug("Holes in field", "BallDetector", holesInField);
    //
	////vector< vector<Point> > contoursVector = FindContours(holesInField, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//vector< vector<Point> > contoursVector = FindContours(holesInField, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	//if (contoursVector.size() == 0)
	//{
	//	return new DetectedBall();
	//}
    //
	//vector<Point2f> centersVector;
	//centersVector.resize(contoursVector.size());
	//vector<float> radiusesVector;
	//radiusesVector.resize(contoursVector.size());
    //
	//float maxRatio = -1.0f;
	//int maxRatioIndex = -1;
    //
	//// Get the contour with the maximum ratio between its area to the area of its enclosing circle.
	//for (size_t i = 0; i < contoursVector.size(); i++)
	//{
	//	minEnclosingCircle(contoursVector[i], centersVector[i], radiusesVector[i]);
	//	circle(holesInField, centersVector[i], radiusesVector[i], Colors::Blue, 2);
    //
	//	double holeArea = contourArea(contoursVector[i]);
	//	double enclosingCircleArea = M_PI * pow(radiusesVector[i], 2);
	//	double ratio = holeArea / enclosingCircleArea;
    //
	//	const float rationThreshold = 0.7;
	//	const float radiusThreshold = 10;
	//	if (ratio < rationThreshold || radiusesVector[i] < radiusThreshold)
	//	{
	//		continue;
	//	}
    //
	//	if (maxRatio < ratio)
	//	{
	//		maxRatio = ratio;
	//		maxRatioIndex = i;
	//	}
	//}
	//// TODO Auto-generated destructor stub
	//if (maxRatioIndex == -1)
	//{
	//	return new DetectedBall();
	//}
    //
	//if (radiusesVector[maxRatioIndex] < 0.0f || radiusesVector[maxRatioIndex] > 10000.0f)
	//{
	//	printf("Radius is corrupt in BallDetector.FindBall()");
	//}
    //
	//ImageShowOnDebug("HolesInField", "BallDetector", holesInField);
    //
	//float tilt = Motion::GetInstance()->GetHeadTilt().Tilt;
	//float distance =  0;//m_BallDistanceCalculator->CalculateDistance(centersVector[maxRatioIndex], (int)tilt);
	//return new DetectedBall(centersVector[maxRatioIndex], radiusesVector[maxRatioIndex], distance);
	//circle(splittedMat[1], center, (float)radius, Colors::Blue, 2);
    circle(houghCircleInputGray, center, (float)radius, Colors::Blue, 2);

	return new DetectedBall(center, (float)radius, 0);
	//return new DetectedBall();


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
