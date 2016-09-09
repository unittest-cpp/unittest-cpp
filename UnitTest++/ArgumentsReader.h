#include <string>
#include <vector>

namespace UnitTest {

	using namespace std;

	class ArgumentsReader
	{
	public:
		ArgumentsReader(int argc, char**argv);
		virtual ~ArgumentsReader();

		bool findArgumentListIndex(const string & argumentName, int & outFrom, int & outCount) const;
		vector<string> extractValues(const string & argumentName) const;
		string getArgument(int index) const;

	private:
		vector<string> _arguments;
	};
}