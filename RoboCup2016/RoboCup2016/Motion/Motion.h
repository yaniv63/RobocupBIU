#include "MotionIncludes.h"


#define MOTION_FILE_PATH    "/home/robot/RoboCup2016/RoboCup2016/HROS5-Framework/Data/motion_4096.bin"
#define INI_FILE_PATH    	"/home/robot/RoboCup2016/RoboCup2016/HROS5-Framework/Data/config.ini"

#pragma once

class HeadTilt
{
public:
	// Tilt is up-down. Pan is right-left.
	float Tilt;
	float Pan;

	HeadTilt(float tilt, float pan)
	{
		Tilt = tilt;
		Pan = pan;
	}


};

class Motion
{
public:
	// Singletone
	static Motion* GetInstance();

	~Motion();

	void WalkStraight(int cmToWalk);
	void StartWalking();
	void StartWalking(int x_amp, int y_amp, int a_amp);
	void StopWalking();
	void GetUp();
	
	void SetHeadTilt(HeadTilt headTilt);
	HeadTilt GetHeadTilt();

	void RunAction(ActionPage actionPage);
	void WaitForActionFinish();

	void FreeAllEngines();
	void StartEngines();

private:
	Motion();
	static Motion* m_instance;

	// Returns true as long as any motion is being done
	bool IsActionRunning();
};


