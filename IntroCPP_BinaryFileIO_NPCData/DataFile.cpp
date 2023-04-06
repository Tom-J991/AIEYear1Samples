#include "DataFile.h"
#include <fstream>

using namespace std;

DataFile::DataFile()
	: recordCount{ 0 }
	, recordOffset{ 0 }
	, lastIndex{ 0 }
	, fileSize{ 0 }
{ }
DataFile::~DataFile()
{ }

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

int seek = 0;
DataFile::Record* DataFile::Load(string filename, int index)
{
	ifstream infile(filename, ios::in | ios::binary);

	if (!infile.is_open())
		return nullptr;

	// Get seeking direction (Delta index)
	int dIndex = index - lastIndex;
	
	// Find file size
	fileSize = 0;
	infile.seekg(0, ios::end);
	fileSize = infile.tellg();
	infile.seekg(0, ios::beg);

	// Get record count
	infile.read((char *)&recordCount, sizeof(int));

	// Skip record count bytes only if at first record.
	int tmpOffset = 0;
	if (index == 0)
		tmpOffset = sizeof(int);

	// Goto record position.
	if (dIndex > 0) // Seeking forwards
		seek = tmpOffset + recordOffset;
	else if (dIndex < 0) // Seeking backwards
		seek = recordOffset;
	else
		seek = tmpOffset;
	infile.seekg(seek, ios::beg);

	// Read file
	int nameSize = 0;
	int ageSize = 0;
	int width = 0, height = 0, format = 0, imageSize = 0;

	// Get image dimensions
	infile.read((char*)&width, sizeof(int));
	infile.read((char*)&height, sizeof(int));

	imageSize = sizeof(Color) * width * height; // Size of image data

	// Get size of NPC name and age.
	infile.read((char*)&nameSize, sizeof(int));
	infile.read((char*)&ageSize, sizeof(int));

	// Read and create Image
	char* imgdata = new char[imageSize];
	infile.read(imgdata, imageSize);
	Image img = LoadImageEx((Color*)imgdata, width, height);

	// Read NPC name and age.
	char* name = new char[nameSize+1];
	int age = 0;

	infile.read((char*)name, nameSize);
	name[nameSize] = '\0'; // Add null terminator to avoid garbage.

	infile.read((char*)&age, ageSize);

	// Create record.
	Record* r = new Record();
	r->image = img;
	r->name = string(name);
	r->age = age;

	// Clean up and return.
	lastIndex = index;
	recordOffset = infile.tellg();

	delete[] imgdata;
	delete[] name;
	infile.close();

	return r;
}

void DataFile::Clear(Record *record)
{
	if (record == nullptr)
		return;

	// Unload unused images and clear memory.
	UnloadImage(record->image);

	delete record;
	record = nullptr;
}
