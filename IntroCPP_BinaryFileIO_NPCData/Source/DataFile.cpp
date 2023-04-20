#include "DataFile.h"
#include <fstream>

DataFile::DataFile()
	: recordCount{ 0 }
{ }
DataFile::~DataFile()
{ }

DataFile::Record* DataFile::Load(string filename, int index)
{
	std::fstream infile(filename, ios::in | ios::binary);

	if (!infile.is_open())
		return nullptr; // Return null pointer if failed to open.

	// Get record count.
	infile.read((char *)&recordCount, sizeof(int));

	// Create and setup dynamic array only if it is currently null.
	if (indexPositions == nullptr)
	{
		indexPositions = new int[recordCount];
		indexPositions[0] = infile.tellg(); // No need to do this everytime a record is loaded.
	}

	// Goto record at given index position.
	infile.seekg(indexPositions[index], ios::beg);

	// Read record data.
	int nameSize = 0;
	int ageSize = 0;
	int width = 0, height = 0, format = 0, imageSize = 0;

	// Get image dimensions.
	infile.read((char*)&width, sizeof(int));
	infile.read((char*)&height, sizeof(int));

	imageSize = sizeof(Color) * width * height; // Calculates size of image data.

	// Get size of NPC name and age.
	infile.read((char*)&nameSize, sizeof(int));
	infile.read((char*)&ageSize, sizeof(int));

	// Read and create Image from data.
	char* imgdata = new char[imageSize];
	infile.read(imgdata, imageSize);
	Image img = LoadImageEx((Color*)imgdata, width, height);

	// Read NPC name and age.
	char* name = new char[nameSize+1];
	int age = 0;

	infile.read((char*)name, nameSize);
	name[nameSize] = '\0'; // Add null terminator to end of array to avoid random garbage.

	infile.read((char*)&age, ageSize);

	// Create record.
	Record* r = new Record();
	r->image = img;
	r->name = string(name);
	r->age = age;

	// Add next index to array.
	if (index+1 < recordCount) // Don't add if out of range.
		indexPositions[index+1] = infile.tellg();

	// Clean up and return.
	delete[] imgdata;
	delete[] name;
	infile.close();

	return r;
}

void DataFile::Clear(Record *record)
{
	if (record == nullptr) // Safety check.
		return;

	// Unload unused images and clear memory.
	UnloadImage(record->image);

	delete record;
	record = nullptr;
}
