#ifndef DETECTEDOBJECT_H_
#define DETECTEDOBJECT_H_

#include "../VisionUtils.h"

// Abstract class
class DetectedObject
{
public:
	Point2f Location;
	bool IsDetected;

	virtual float Distance() = 0;
	virtual void Draw(Mat& image) = 0;

	DetectedObject(Point2f center, bool isFound);
	virtual ~DetectedObject();

	static Point2f UnknownLocation;
};

#endif /* DETECTEDOBJECT_H_ */
