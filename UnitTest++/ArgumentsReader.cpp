#include "ArgumentsReader.h"

using namespace UnitTest;

ArgumentsReader::ArgumentsReader(int argc, char**argv)
{
	_arguments.reserve(argc);
	for (int i = 0; i < argc; i++)
	{
		_arguments.push_back(argv[i]);
	}
}

bool ArgumentsReader::findArgumentListIndex(const string & argumentName, int & outFrom, int & outCount)
{
	if (_arguments.empty())
	{
		return false;
	}
	outCount = 0;
	if (argumentName.empty())
	{
		outFrom = 1;
	}
	else
	{
		outFrom = 0;
		for (size_t i = 1; i < _arguments.size(); i++)
		{
			if (argumentName == _arguments[i])
			{
				outFrom = i + 1;
				break;
			}
		}
		if (outFrom == 0)
		{
			return false;
		}
	}

	for (size_t i = outFrom; i < _arguments.size(); i++)
	{
		string value = _arguments[i];
		if (value.size() >= 2 && value.substr(0, 2) == "--")
		{
			break;
		}
		outCount++;
	}

	return true;
}


vector<string> ArgumentsReader::extractValues(const string & argumentName)
{
	int from, count;
	if (!findArgumentListIndex(argumentName, from, count))
	{
		return vector<string>();
	}
	vector<string> values;
	for (int i = from; i < from + count; i++)
	{
		values.push_back(getArgument(i));
	}
	return values;
}


string ArgumentsReader::getArgument(int index)
{
	return _arguments[index];
}
