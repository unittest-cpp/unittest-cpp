#include <string>
#include <vector>

namespace UnitTest {

	using namespace std;

	class ArgumentsReader
	{
	public:
		ArgumentsReader(int argc, char**argv);
		bool findArgumentListIndex(const string & argumentName, int & outFrom, int & outCount);
		vector<string> extractValues(const string & argumentName);
		string getArgument(int index);

	private:
		vector<string> _arguments;
	};
}