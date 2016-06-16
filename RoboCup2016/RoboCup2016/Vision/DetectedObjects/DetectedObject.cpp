/*
 * DetectedObject.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: laptopyellow
 */

#include "../DetectedObjects/DetectedObject.h"

Point2f DetectedObject::UnknownLocation = Point2f(-1,-1);

DetectedObject::DetectedObject(Point2f location, bool isFound)
	:Location(location), IsDetected(isFound)
{
}

DetectedObject::~DetectedObject() {
	// TODO Auto-generated destructor stub
}

