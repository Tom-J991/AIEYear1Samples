#pragma once

#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

class DataFile
{
public:
	struct Record {
		Image image;
		string name;
		int age;
	};

private:
	int *indexPositions; // Dynamic array to store the index positon of each record.
	int recordCount; // Amount of records in the data file.

public:
	DataFile();
	~DataFile();

	int GetRecordCount() { return recordCount; };

	Record* Load(string filename, int index);

	void Clear(Record* record);

};

