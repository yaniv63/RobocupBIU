/*
 * LineDetector.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: root
 */

#include "LineDetector.h"

LineDetector::LineDetector() {
	// TODO Auto-generated constructor stub

}

LineDetector::~LineDetector() {
	// TODO Auto-generated destructor stub
}

DetectedObject* LineDetector::DetectObject(Mat& inputImageHSV)
{
	Mat onlyWhiteImageLineDetector;
	GetWhiteImage(inputImageHSV, onlyWhiteImageLineDetector);


	ImageShowOnDebug("****Field and Contours - Shepet", "LineDetector", onlyWhiteImageLineDetector);


	return new DetectedLine();
}
