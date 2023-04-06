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
	int lastIndex;
	int recordOffset;
	int recordCount;
	int fileSize;

public:
	DataFile();
	~DataFile();

	Record* GetRecord(string filename, int index);

	int GetRecordCount() { return recordCount; };

	void Save(string filename);
	Record* Load(string filename, int index);

	void Clear(Record* record);

};

