#include "Detectors/BallDetector.h"
#include "Detectors/GateDetector.h"
#include "Detectors/LineDetector.h"
#include "../Log/Log.h"

#pragma once

void RunVision();

class Vision
{
public:
	// Singletone
	static Vision* GetInstance();

	void Run();
	void TerminateThread();

	~Vision();

	Detector* Gate;
	Detector* Ball;
	Detector* Line;

	bool IsVisionThreadRunning;

	void ProcessCurrentFrame(Mat& frame);
private:
	VideoCapture m_VideoCapture;
	Log* m_log;
	string m_logCategory;

	// Whether to save a video of the current run.
	bool m_SaveVideo;
	// The video writer that saves the video if m_SaveVideo is true
	VideoWriter m_OutputVideo;

	static Vision* m_instance;
	Vision();
};

