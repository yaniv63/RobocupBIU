/*
 * DistanceDatabase.cpp
 *
 *  Created on: May 4, 2016
 *      Author: root
 */

#include "DatabaseWrapper.h"

DatabaseWrapper::DatabaseWrapper(string filePath)
{
	m_DatabaseMap = LoadMapFromFile(filePath);
}

DatabaseWrapper::~DatabaseWrapper()
{
	// TODO Auto-generated destructor stub
}

int DatabaseWrapper::GetValueFromKey(int key)
{
	int closestKey = FindClosestKey(key);
	return m_DatabaseMap[closestKey];
}

map<int,int> DatabaseWrapper::LoadMapFromFile(string filePath)
{
	map<int,int> resultMap;

	string line;
	ifstream myfile(filePath.c_str());

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			vector<string> splittedLine = StringSplit(line, ",");
			resultMap[atoi(splittedLine[0].c_str())] = atoi(splittedLine[1].c_str());
		}
		myfile.close();
	}

	else
	{
		cout << "Unable to open distance DB file: " << filePath << endl;
	}

	return resultMap;
}

vector<string> DatabaseWrapper::StringSplit(const string& str, const string& delimiter)
{
    vector <string> tokens;

    string::size_type lastPos = 0;
    string::size_type pos = str.find(delimiter, lastPos);

    while (string::npos != pos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = pos + delimiter.size();
        pos = str.find(delimiter, lastPos);
    }

    tokens.push_back(str.substr(lastPos, str.size() - lastPos));
    return tokens;
}

int DatabaseWrapper::FindClosestKey(int key)
{
	vector<int> mapKeys;
	for (map<int, int>::iterator it = m_DatabaseMap.begin();
			it != m_DatabaseMap.end(); ++it)
	{
		mapKeys.push_back(it->first);
	}

	int closestKey = mapKeys.front();
	for (int i = 0; i < (int) mapKeys.size(); i++)
	{
		if (abs(mapKeys[i] - key) < abs(closestKey - key))
		{
			closestKey = mapKeys[i];
		}
	}

	return closestKey;
}
