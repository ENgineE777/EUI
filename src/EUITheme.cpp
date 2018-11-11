
#include "EUITheme.h"
#include "UTFConv.h"

EUITheme::EUITheme()
{
	themePath[0] = 0;
}

void EUITheme::Init(const char* set_theme_path, const char* name)
{
	strcpy(themePath, set_theme_path);
	
	{
		uint32_t size = 0;
		uint8_t* buffer = LoadFile("low2hi.dat", size);

		JSONParser parser(buffer);
		UTFConv::Init(parser);
	}

	uint32_t size = 0;
	uint8_t* buffer = LoadFile(name, size);

	JSONParser reader(buffer);

	ReadTheme(reader);
}

uint8_t* EUITheme::LoadFile(const char* name, uint32_t& size)
{
	std::string file_name = themePath;
	file_name += name;

	FILE* file = fopen(file_name.c_str(), "rb");

	uint8_t* buffer = nullptr;
	size = 0;

	if (file)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);

		buffer = (uint8_t*)malloc(size + 1);
		fread(buffer, size, 1, file);

		buffer[size] = 0;

		fclose(file);
	}

	return buffer;
}