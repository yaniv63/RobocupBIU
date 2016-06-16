#include "Calibrator.h"

Mat currentFrame;
const int trackbarValueMax = 1;
int trackbarValue;

Calibrator* Calibrator::m_instance = NULL;

Calibrator::Calibrator()
{
	m_Calibration = Calibration::GetInstance();
}

Calibrator* Calibrator::GetInstance()
{
    if(m_instance == NULL)
    {
    	m_instance = new Calibrator();
    }
 	return m_instance;
}

void Calibrator::Calibrate()
{
	try
	{
		InitializeWindow();
	}
	catch (Exception& ex)
	{
		cout << "Couldn't initialize window" << endl;
		exit(1);
	}

	m_Calibration = Calibration::ReadFromFile();

	VideoCapture videoCapture;
	if (!videoCapture.open(0))
	{
		exit(1);
	}

	while (true)
	{
		try
		{
			videoCapture >> currentFrame;
			if (currentFrame.empty())
			{
				break; // end of video stream
			}
		}
		catch (Exception& ex)
		{
			cout << "Couldn't read image: " << ex.what() << endl;
		}

		ShowCalibration(currentFrame);

		// Close the calibrator & save settings by clicking 'q'.
		// Close without saving by clicking 'z'
		char exitKey = waitKey(20);
		if (exitKey == 'q')
		{
			m_Calibration->WriteCalibrationToFile();
			destroyAllWindows();
			return;
		}
		if (exitKey == 'z')
		{
			destroyAllWindows();
			return;
		}
	}	
}

void onMouse(int event, int mouseX, int mouseY, int flag, void* param)
{
	if (event != CV_EVENT_LBUTTONDOWN && event != CV_EVENT_RBUTTONDOWN)
	{
		return;
	}

	Calibrator* calibrator = Calibrator::GetInstance();

	Point2i mousePosition(mouseX,mouseY);
	if (event == CV_EVENT_RBUTTONDOWN)
	{
		calibrator->ResetValuesToClick(currentFrame, mousePosition);
	}
	
	else
	{
		if (calibrator->IsGreenCalibration())
		{
			calibrator->CalibrateGreen(currentFrame, mousePosition);
		}
		else
		{
			calibrator->CalibrateWhite(currentFrame, mousePosition);
		}
	}
}

void Calibrator::ResetValuesToClick(Mat image, Point2i position)
{
	if (IsGreenCalibration())
	{
		m_Calibration->MinGreenHSV = MinAreaScalar(image, position);
		m_Calibration->MaxGreenHSV = MaxAreaScalar(image, position);
	}
	else
	{
		m_Calibration->MinWhiteHSV = MinAreaScalar(image, position);
		m_Calibration->MaxWhiteHSV = MaxAreaScalar(image, position);
	}
}

void Calibrator::ShowCalibration(Mat image)
{
	imshow(ImageWindow, currentFrame);
	cvtColor(currentFrame, currentFrame, CV_BGR2HSV);

	Mat threshold;

	if (IsGreenCalibration())
	{
		inRange(image, m_Calibration->MinGreenHSV, m_Calibration->MaxGreenHSV, threshold);
	}
	else
	{
		inRange(image, m_Calibration->MinWhiteHSV, m_Calibration->MaxWhiteHSV, threshold);
	}

	imshow(CalibrationWindow, threshold);
}

bool Calibrator::IsGreenCalibration()
{
	return getTrackbarPos(TrackbarGreenOrWhite, ImageWindow) == 1;
}

void Calibrator::CalibrateGreen(Mat image, Point2i position)
{
	m_Calibration->MinGreenHSV = Min(MinAreaScalar(image, position), m_Calibration->MinGreenHSV);
	m_Calibration->MaxGreenHSV = Max(MaxAreaScalar(image, position), m_Calibration->MaxGreenHSV);
}

void Calibrator::CalibrateWhite(Mat image, Point2i position)
{
	m_Calibration->MinWhiteHSV = Min(MinAreaScalar(image, position), m_Calibration->MinWhiteHSV);
	m_Calibration->MaxWhiteHSV = Max(MaxAreaScalar(image, position), m_Calibration->MaxWhiteHSV);
}

Scalar Calibrator::Min(Scalar a, Scalar b)
{
	Scalar minimum;
	for (int i = 0; i < 3; i++)
	{
		minimum[i] = MIN(a[i], b[i]);
	}

	return minimum;
}

Scalar Calibrator::Max(Scalar a, Scalar b)
{
	Scalar minimum;
	for (int i = 0; i < 3; i++)
	{
		minimum[i] = MAX(a[i], b[i]);
	}

	return minimum;
}



bool Calibrator::IsInImageBorders(int x, int y)
{
	return 0 <= x && x < FRAME_WIDTH && 0 <= y && y < FRAME_HEIGHT;
}

Scalar Calibrator::MinAreaScalar(Mat image, Point2i point)
{
	Scalar minScalar(255, 255, 255);

	for (int i = point.x - 1; i <= point.x + 1; i++)
	{
		for (int j = point.y - 1; j <= point.y + 1; j++)
		{
			if (IsInImageBorders(i, j))
			{
				Vec3b currentPixel = image.at<Vec3b>(j, i);
				for (int k = 0; k < 3; k++)
				{
					minScalar[k] = MIN(minScalar[k], currentPixel[k]);
				}
			}
		}
	}

	return minScalar;
}

Scalar Calibrator::MaxAreaScalar(Mat image, Point2i point)
{
	Scalar maxScalar(0, 0, 0);

	for (int i = point.x - 1; i <= point.x + 1; i++)
	{
		for (int j = point.y - 1; j <= point.y + 1; j++)
		{
			if (IsInImageBorders(i, j))
			{
				Vec3b currentPixel = image.at<Vec3b>(j, i);
				for (int k = 0; k < 3; k++)
				{
					maxScalar[k] = MAX(maxScalar[k], currentPixel[k]);
				}
			}
		}
	}

	return maxScalar;
}

void Calibrator::InitializeWindow()
{
	namedWindow(ImageWindow, CV_WINDOW_NORMAL);
	namedWindow(CalibrationWindow, CV_WINDOW_NORMAL);
	createTrackbar(TrackbarGreenOrWhite, ImageWindow, &trackbarValue, trackbarValueMax);
	setMouseCallback(ImageWindow, onMouse, 0);
}
