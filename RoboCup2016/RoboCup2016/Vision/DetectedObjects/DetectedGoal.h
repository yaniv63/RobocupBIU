#include "DetectedObject.h"

#pragma once

enum Post
{
	POST_NONE,
	POST_LEFT,
	POST_RIGHT,
	POST_BOTH
};

static const char* PostStrings[] = { "None", "Left", "Right", "Both" };

class DetectedGoal : public DetectedObject
{
public:
	RotatedRect LeftPost;
	RotatedRect RightPost;
	Post DetectedPost;

	virtual float Distance();
	virtual void Draw(Mat& image);

	DetectedGoal();
	~DetectedGoal();

	DetectedGoal(RotatedRect post1, RotatedRect post2);
	DetectedGoal(RotatedRect post, Post detectedPost);

private:
	Point2f GetGoalCenter(RotatedRect post1, RotatedRect post2);
	void PrintDetailsOnImage(Mat image);
};



