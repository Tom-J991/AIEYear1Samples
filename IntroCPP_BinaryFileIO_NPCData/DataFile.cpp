#include "DataFile.h"
#include <fstream>
using namespace std;

DataFile::DataFile()
{
	recordCount = 0;
}

DataFile::~DataFile()
{
	Clear();
}

DataFile::Record* DataFile::AddRecord(string imageFilename, string name, int age)
{
	Image i = LoadImage(imageFilename.c_str());

	Record* r = new Record;
	r->image = i;
	r->name = name;
	r->age = age;

	recordCount++;

	return r;
}

DataFile::Record* DataFile::GetRecord(string filename, int index)
{
	return Load(filename, index);
}

void DataFile::Save(string filename)
{
	/*ofstream outfile(filename, ios::binary);

	int recordCount = records.size();
	outfile.write((char*)&recordCount, sizeof(int));

	for (int i = 0; i < recordCount; i++)
	{		
		Color* imgdata = GetImageData(records[i]->image);
				
		int imageSize = sizeof(Color) * records[i]->image.width * records[i]->image.height;
		int nameSize = records[i]->name.length();
		int ageSize = sizeof(int);

		outfile.write((char*)&records[i]->image.width, sizeof(int));
		outfile.write((char*)&records[i]->image.height, sizeof(int));
		
		outfile.write((char*)&nameSize, sizeof(int));
		outfile.write((char*)&ageSize, sizeof(int));

		outfile.write((char*)imgdata, imageSize);
		outfile.write((char*)records[i]->name.c_str(), nameSize);
		outfile.write((char*)&records[i]->age, ageSize);
	}

	outfile.close();*/
}

DataFile::Record* DataFile::Load(string filename, int index)
{
	ifstream infile(filename, ios::in | ios::binary);

	if (index == 0)
	{
		recordCount = 0;
		infile.seekg(0, ios::beg);
		infile.read((char *)&recordCount, sizeof(int));
	}

	infile.seekg(recordOffset, ios::beg);

	int nameSize = 0;
	int ageSize = 0;
	int width = 0, height = 0, format = 0, imageSize = 0;

	infile.read((char*)&width, sizeof(int));
	infile.read((char*)&height, sizeof(int));

	imageSize = sizeof(Color) * width * height;

	infile.read((char*)&nameSize, sizeof(int));
	infile.read((char*)&ageSize, sizeof(int));

	char* imgdata = new char[imageSize];
	infile.read(imgdata, imageSize);

	Image img = LoadImageEx((Color*)imgdata, width, height);
	char* name = new char[nameSize];
	int age = 0;
				
	infile.read((char*)name, nameSize);
	infile.read((char*)&age, ageSize);
	
	recordOffset += infile.tellg();

	Record* r = new Record();
	r->image = img;
	r->name = string(name);
	r->age = age;

	delete[] imgdata;
	delete[] name;

	infile.close();

	return r;
}

void DataFile::Clear()
{
	

	recordCount = 0;
}