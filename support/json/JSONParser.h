
#pragma once

#include "json.h"
#include <string>

class JSONParser
{
	block_allocator allocator;
	json_value* root;

	uint8_t*  buffer;

	int curDepth;
	json_value* nodes[50];
	json_value* curNode;

public:

	JSONParser();

	bool Parse(const char* name);

	bool EnterBlock(const char* name);
	void LeaveBlock();

	bool Read(const char* name, char* val, int val_len);
	bool Read(const char* name, std::string& val);
	bool Read(const char* name, bool& val);
	bool Read(const char* name, float& val);
	bool Read(const char* name, int& val);
	bool Read(const char* name, int64_t& val);

	void Release();

protected:

	json_value* FindValue(const char* name);
};



