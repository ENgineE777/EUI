
#include "JSONParser.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>

JSONParser::JSONParser(uint8_t* set_buffer) : allocator(1 << 10)
{
	buffer = set_buffer;

	if (buffer)
	{
		char *errorPos = 0;
		char *errorDesc = 0;
		int errorLine = 0;

		root = json_parse((char*)buffer, &errorPos, &errorDesc, &errorLine, &allocator);

		if (root)
		{
			nodes[curDepth] = root;
			curNode = nodes[curDepth];
		}
	}
}

JSONParser::~JSONParser()
{
	if (buffer)
	{
		free(buffer);
	}

	allocator.free();
}


bool JSONParser::EnterBlock(const char* name)
{
	nodes[curDepth + 1] = FindValue(name);

	if (nodes[curDepth + 1])
	{
		if (nodes[curDepth + 1]->type == JSON_OBJECT)
		{
			curDepth++;
			curNode = nodes[curDepth];

			return true;
		}
		else
		if (nodes[curDepth + 1]->type == JSON_ARRAY)
		{
			if (nodes[curDepth + 1]->first_child)
			{
				curDepth++;
				nodes[curDepth] = nodes[curDepth]->first_child;
				curNode = nodes[curDepth];

				return true;
			}
		}
	}

	return false;
}

void JSONParser::LeaveBlock()
{
	if (curDepth == 0) return;

	if (nodes[curDepth]->parent->type == JSON_ARRAY)
	{
		nodes[curDepth]->parent->first_child = nodes[curDepth]->next_sibling;
	}
	
	curDepth--;
	curNode = nodes[curDepth];
}

bool JSONParser::Read(const char* name, char* val, int val_len)
{
	json_value* node = FindValue(name);
	
	if (node)
	{
		if (node->type == JSON_STRING)
		{
			strcpy(val, node->string_value);
			return true;
		}
		else
		if (node->type == JSON_BOOL || node->type == JSON_INT)
		{
			sprintf(val, "%i", node->int_value);
			return true;
		}
		else
		if (node->type == JSON_FLOAT)
		{
			sprintf(val, "%4.4f", node->float_value);
			return true;
		}
		else
		if (node->type == JSON_NULL)
		{
			sprintf(val, "");
			return true;
		}
	}

	return false;
}

bool JSONParser::Read(const char* name, std::string& val)
{
	json_value* node = FindValue(name);

	if (node)
	{
		if (node->type == JSON_STRING)
		{
			val = node->string_value;
			return true;
		}
		else
			if (node->type == JSON_BOOL || node->type == JSON_INT)
			{
				char tmp[128];
				sprintf(tmp, "%i", node->int_value);
				val = tmp;

				return true;
			}
			else
				if (node->type == JSON_FLOAT)
				{
					char tmp[128];
					sprintf(tmp, "%4.5f", node->float_value);
					val = tmp;

					return true;
				}
				else
					if (node->type == JSON_NULL)
					{
						val = "";
						return true;
					}
	}

	return false;
}

bool JSONParser::Read(const char* name, bool& val)
{
	json_value* node = FindValue(name);

	if (node)
	{
		if (node->type == JSON_BOOL)
		{
			if (node->int_value > 0)
			{
				val = true;
			}
			else
			{
				val = false;
			}

			return true;
		}
		else
		if (node->type == JSON_STRING)
		{
			if (atoi(node->string_value) > 0)
			{
				val = true;
			}
			else
			{
				val = false;
			}

			return true;
		}
		else
		if (node->type == JSON_NULL)
		{
			val = false;

			return true;
		}
	}

	return false;
}

bool JSONParser::Read(const char* name, float& val)
{
	json_value* node = FindValue(name);

	if (node)
	{
		if (node->type == JSON_FLOAT)
		{
			val = node->float_value;
			return true;
		}		
		else
		if (node->type == JSON_INT)
		{
			val = (float)node->int_value;
			return true;
		}
		else
		if (node->type == JSON_STRING)
		{
			val = (float)atof(node->string_value);
			return true;
		}
	}

	return false;
}

bool JSONParser::Read(const char* name, int& val)
{
	json_value* node = FindValue(name);

	if (node)
	{
		if (node->type == JSON_INT)
		{
			val = node->int_value;

			return true;
		}
		else
		if (node->type == JSON_NULL)
		{
			val = 0;

			return true;
		}
		else
		if (node->type == JSON_STRING)
		{
			val = atoi(node->string_value);
			return true;
		}
	}

	return false;
}

bool JSONParser::Read(const char* name, int64_t& val)
{
	json_value* node = FindValue(name);

	if (node)
	{
		if (node->type == JSON_INT)
		{
			val = node->int64_value;
			return true;
		}
		else
		if (node->type == JSON_NULL)
		{
			val = 0;

			return true;
		}
		else
		if (node->type == JSON_STRING)
		{
			val = atol(node->string_value);
			return true;
		}
	}

	return false;
}

json_value* JSONParser::FindValue(const char* name)
{
	if (!root)
	{
		return nullptr;
	}

	if (!name[0])
	{
		return curNode;
	}

	for (json_value *it = curNode->first_child; it; it = it->next_sibling)
	{
		if (_stricmp(name, it->name) == 0)
		{
			return it;
		}
	}

	return nullptr;
}