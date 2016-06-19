/*
 * DistanceDBBuilder.cpp
 *
 *  Created on: May 11, 2016
 *      Author: root
 */

#include "DistanceDBBuilder.h"

DistanceDBBuilder::DistanceDBBuilder()
{
	m_angleToZeroPixelDistance = new DatabaseWrapper(ZeroPixelsDistanceDatabasePath);
}

DistanceDBBuilder::~DistanceDBBuilder()
{
	delete m_angleToZeroPixelDistance;
}

void DistanceDBBuilder::CreateDatabaseForTilt(Mat whiteImage, int angle)
{
	int zeroPixelDistance = m_angleToZeroPixelDistance->GetValueFromKey(angle);
	map<int,int> database = CreateDatabse(whiteImage, zeroPixelDistance);

	string filePath = GetDatabaseFilePath(angle);
	WriteDatabaseToFile(database, filePath);
}

string DistanceDBBuilder::GetDatabaseFilePath(int angle)
{
	string databaseFolder = DistanceDatabaseFolderPath;
	string fileName = intToString(angle);
	return databaseFolder + fileName;
}

string DistanceDBBuilder::intToString(int number)
{
	ostringstream convert;  // stream used for the conversion
	convert << number; 		// insert the textual representation of 'Number' in the characters in the stream
	return convert.str();
}

map<int,int> DistanceDBBuilder::CreateDatabse(Mat whiteImage, int zeroPixelDistance)
{
	vector<int> distances = { 10, 20, 30 };

	ofstream myfile("a.txt");

	map<int, int> result;
	int middleColumn = 640 / 2;

	int blockNumber = 0;
	int pixelsCount = 0;
	for (int row = 480 - 1;
			row > 0 && static_cast<int>(distances.size()) > blockNumber;
			row--) {
		int currentPixel = (int) whiteImage.at<uchar>(row, middleColumn);
		int nextPixel = (int) whiteImage.at<uchar>(row - 1, middleColumn);

		myfile << "Row: " << FRAME_HEIGHT - row << ", Pixel: "
				<< (currentPixel == 255 ? "W" : "B") << endl;

		if (nextPixel != currentPixel) {
			result[distances[blockNumber]] = pixelsCount;
			blockNumber++;
		}

		pixelsCount++;
	}

	for (map<int, int>::const_iterator it = result.begin(); it != result.end();
			++it) {
		myfile << "Distance: " << it->first << ", Pixels: " << it->second
				<< endl;
	}

	return result;
}

void DistanceDBBuilder::WriteDatabaseToFile(map<int,int> database, string filePath)
{
	ofstream out(filePath.c_str());

	typedef map<int,int>::iterator mapIterator;
	for(mapIterator iterator = database.begin(); iterator != database.end(); iterator++)
	{
		out << iterator->first << "," << iterator->second << endl;
	}

	out.close();
}
