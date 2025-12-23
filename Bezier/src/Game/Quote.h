#pragma once
#include <string>
class Quote
{
public:
	Quote(std::string t, bool r);
	std::string text;
	bool repeated;
};

