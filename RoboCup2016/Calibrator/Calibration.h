/*
 * Calibration.h
 *
 *  Created on: Jun 15, 2016
 *      Author: root
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "Includes.h"
#include "Constants.h"

static char* CalibrationFilePath = "/home/robot/src/RobocupBIU/RoboCup2016/RoboCup2016/calibration.txt";

class Calibration {
public:
	// Singletone
	static Calibration* GetInstance();

	Scalar MinWhiteHSV;
	Scalar MaxWhiteHSV;
	Scalar MinGreenHSV;
	Scalar MaxGreenHSV;

	static Calibration* ReadFromFile();
	void WriteCalibrationToFile();

private:
	static Calibration* m_instance;
	Calibration();
	static void ParseLineFromCalibrationFile(string line);
};

#endif /* CALIBRATION_H_ */
