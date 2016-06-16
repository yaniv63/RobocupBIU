//============================================================================
// Name        : DistanceCalculator.cpp
// Author      : Hen Shoob & Assaf Rabinowitz
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "DistanceDBBuilder.h"
#include "../RoboCup2016/Motion/Motion.h"

void WaitForKey();
void BackupCurrentDatabase();

int main()
{
//	WaitForKey();
//	BackupCurrentDatabase();

	Calibration::ReadFromFile();
	Calibration* calibration = Calibration::GetInstance();

	DistanceDBBuilder distanceDBBuilder;

	VideoCapture videoCapture;
	videoCapture.open(0);
	namedWindow("Output", CV_WINDOW_AUTOSIZE);
	int tilt = MinTilt;

	cout << "Press \"s\" to continue. Current tilt: " << tilt << endl;
	while(true)
		{
			Mat currentFrame;
			videoCapture >> currentFrame;
			imshow("Output", currentFrame);

			DrawMiddleColumn();

			cvtColor(currentFrame, currentFrame, CV_BGR2HSV);
			Mat whiteImage;
			inRange(currentFrame, calibration->MinWhiteHSV, calibration->MaxWhiteHSV, whiteImage);

			int morph_size = 2;
			Mat element = getStructuringElement(MORPH_RECT,
					Size(2 * morph_size + 1, 2 * morph_size + 1),
					Point(morph_size, morph_size));
			morphologyEx(whiteImage, whiteImage, MORPH_OPEN, element);

			imshow("White", whiteImage);

			char exitKey = waitKey(1);
			if(exitKey == 'q')
			{
				break;
			}
			if(exitKey == 's')
			{
				// Motion::GetInstance()->SetHeadTilt(HeadTilt(0, tilt));
				distanceDBBuilder.CreateDatabaseForTilt(currentFrame, -55);
				tilt += TiltDelta;
				cout << "Press \"s\" to continue. Current tilt: " << tilt << endl;
			}
		}

	return 0;
}

void WaitForKey()
{
	cout << "Press any key to start the database creation" << endl;
	getchar();
}

void BackupCurrentDatabase()
{
	// TODO
}

