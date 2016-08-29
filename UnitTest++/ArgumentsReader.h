#include <string>
#include <vector>

namespace UnitTest {

	using namespace std;

	class ArgumentsReader
	{
	public:
		ArgumentsReader(int argc, char**argv)
		{
			_arguments.reserve(argc);
			for (int i = 1; i < argc; i++)
			{
				_arguments.push_back(argv[i]);
			}
		}
		
		bool findArgumentListIndex(const string & argumentName, int & outFrom, int & outCount)
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

		string getArgument(int index)
		{
			return _arguments[index];
		}

	private:
		vector<string> _arguments;
	};
}