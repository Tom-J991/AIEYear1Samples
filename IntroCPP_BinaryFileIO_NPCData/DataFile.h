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
	int recordOffset = sizeof(int);
	int recordCount;

public:
	DataFile();
	~DataFile();

	Record* AddRecord(string imageFilename, string name, int age);
	Record* GetRecord(string filename, int index);

	int GetRecordCount() { return recordCount; };

	void Save(string filename);
	Record* Load(string filename, int index);

private:
	void Clear();

};

