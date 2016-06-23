/*
 * TimedDetectedBall.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#include "TimedDetectedBall.h"

TimedDetectedBall::TimedDetectedBall()
	:DetectedBall()
{
	usDetectionTime = -1;
}

TimedDetectedBall::TimedDetectedBall(DetectedBall& detectedBall)
	: DetectedBall(detectedBall)
{
	timeval timev;
	gettimeofday(&timev, 0);
	usDetectionTime = timev.tv_usec;
}

TimedDetectedBall::~TimedDetectedBall()
{
	// TODO Auto-generated destructor stub
}

void TimedDetectedBall::PrintDetailsOnImage(Mat image)
{
	char message[256];
	DetectedBall::PrintDetailsOnImage(image);
	sprintf(message, "Detection time: %d", usDetectionTime);
	PrintMessageOnImage(image, message, Point(0,210));

}
