#include "DetectedGoal.h"

DetectedGoal::DetectedGoal(RotatedRect post1, RotatedRect post2)
	:DetectedObject(GetGoalCenter(post1, post2), true)
{
	DetectedPost = POST_BOTH;
	LeftPost = (post1.center.x < post2.center.x) ? post1 : post2;
	RightPost = (post1.center.x > post2.center.x) ? post1 : post2;
}

DetectedGoal::DetectedGoal(RotatedRect post, Post detectedPost)
	:DetectedObject(post.center, true)
{
	DetectedPost = detectedPost;

	if (DetectedPost == POST_LEFT)
	{
		LeftPost = post;
	}
	else
	{
		RightPost = post;
	}
}

DetectedGoal::DetectedGoal()
	:DetectedObject(UnknownLocation, false)
{
	DetectedPost = POST_NONE;
}

DetectedGoal::~DetectedGoal()
{
}

void DetectedGoal::Draw(Mat& image)
{
	if (DetectedPost == POST_BOTH)
	{
		DrawRectangle(image, LeftPost, Scalar(255,0,0), 2);
		DrawRectangle(image, RightPost, Scalar(255,0,0), 2);
		DrawCircle(image, Location, Scalar(255,0,255), 3);
	}
	if (DetectedPost == POST_LEFT)
	{
		DrawRectangle(image, LeftPost, Scalar(255,0,0), 2);
	}
	if (DetectedPost == POST_RIGHT)
	{
		DrawRectangle(image, RightPost, Scalar(255,0,0), 2);
	}

	PrintDetailsOnImage(image);
}

Point2f DetectedGoal::GetGoalCenter(RotatedRect post1, RotatedRect post2)
{
	float centerX = (post1.center.x + post2.center.x) / 2;
	float centerY = (post1.center.y + post2.center.y) / 2;
	Point2f center(centerX, centerY);

	return center;
}

void DetectedGoal::PrintDetailsOnImage(Mat image)
{
	char message[256];
	PrintMessageOnImage(image, "Goal details:", Point(0,30));

	sprintf(message, "IsDetected: %s", (IsDetected ? "true" : "false"));
	PrintMessageOnImage(image, message, Point(0,50));

	sprintf(message, "Location: (%.3f, %.3f)", Location.x, Location.y);
	PrintMessageOnImage(image, message, Point(0,70));

	sprintf(message, "Detected post: %s", PostStrings[DetectedPost]);
	PrintMessageOnImage(image, message, Point(0,90));
}

float DetectedGoal::Distance()
{
	throw VisionException("Distance is not implemented for Goal");
}
