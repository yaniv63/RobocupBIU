/*
 * Calibration.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: root
 */

#include "Calibration.h"

Calibration* Calibration::m_instance = NULL;

Calibration::Calibration()
{

}

Calibration* Calibration::GetInstance()
{
    if(m_instance == NULL)
    {
    	m_instance = new Calibration();
    }
 	return m_instance;
}

Calibration* Calibration::ReadFromFile()
{
	string line;
	ifstream myfile(CalibrationFilePath);

	if (myfile.is_open()) {
		while (getline(myfile, line))
		{
			ParseLineFromCalibrationFile(line);
		}
		myfile.close();
	}

	else {
		cout << "Unable to open color config file";
	}

	return Calibration::GetInstance();
}

void Calibration::ParseLineFromCalibrationFile(string line)
{
	string key = line.substr(0, 12);
	size_t keyStart = line.find('[');
	size_t keyEnd = line.find(']');
	int value = atoi(line.substr(keyStart + 1, keyEnd - keyStart).c_str());

	Calibration* calibration = Calibration::GetInstance();

	if (key == "White: H_min") calibration->MinWhiteHSV[0] = value;
	if (key == "White: S_min") calibration->MinWhiteHSV[1] = value;
	if (key == "White: V_min") calibration->MinWhiteHSV[2] = value;

	if (key == "White: H_max") calibration->MaxWhiteHSV[0] = value;
	if (key == "White: S_max") calibration->MaxWhiteHSV[1] = value;
	if (key == "White: V_max") calibration->MaxWhiteHSV[2] = value;

	if (key == "Green: H_min") calibration->MinGreenHSV[0] = value;
	if (key == "Green: S_min") calibration->MinGreenHSV[1] = value;
	if (key == "Green: V_min") calibration->MinGreenHSV[2] = value;

	if (key == "Green: H_max") calibration->MaxGreenHSV[0] = value;
	if (key == "Green: S_max") calibration->MaxGreenHSV[1] = value;
	if (key == "Green: V_max") calibration->MaxGreenHSV[2] = value;
}

void Calibration::WriteCalibrationToFile()
{
	ofstream out(CalibrationFilePath);

	out << "White: H_min[" << MinWhiteHSV[0] << "]\n";
	out << "White: S_min[" << MinWhiteHSV[1] << "]\n";
	out << "White: V_min[" << MinWhiteHSV[2] << "]\n";

	out << "\n";

	out << "White: H_max[" << MaxWhiteHSV[0] << "]\n";
	out << "White: S_max[" << MaxWhiteHSV[1] << "]\n";
	out << "White: V_max[" << MaxWhiteHSV[2] << "]\n";

	out << "\n";

	out << "Green: H_min[" << MinGreenHSV[0] << "]\n";
	out << "Green: S_min[" << MinGreenHSV[1] << "]\n";
	out << "Green: V_min[" << MinGreenHSV[2] << "]\n";

	out << "\n";

	out << "Green: H_max[" << MaxGreenHSV[0] << "]\n";
	out << "Green: S_max[" << MaxGreenHSV[1] << "]\n";
	out << "Green: V_max[" << MaxGreenHSV[2] << "]\n";

	out.close();
}
