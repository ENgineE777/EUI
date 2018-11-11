
#pragma once

#include "json.h"
#include <string>

class JSONParser
{
	block_allocator allocator;
	json_value* root = nullptr;

	uint8_t* buffer = nullptr;

	int curDepth = 0;
	json_value* nodes[50];
	json_value* curNode = nullptr;

public:

	JSONParser(uint8_t* set_buffer);
	~JSONParser();

	bool EnterBlock(const char* name);
	void LeaveBlock();

	bool Read(const char* name, char* val, int val_len);
	bool Read(const char* name, std::string& val);
	bool Read(const char* name, bool& val);
	bool Read(const char* name, float& val);
	bool Read(const char* name, int& val);
	bool Read(const char* name, int64_t& val);

protected:

	json_value* FindValue(const char* name);
};



