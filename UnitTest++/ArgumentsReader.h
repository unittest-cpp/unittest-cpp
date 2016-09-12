#include <string>
#include <vector>

namespace UnitTest {

	using namespace std;

	class ArgumentsReader
	{
	public:
		ArgumentsReader(int argc, char**argv);
		virtual ~ArgumentsReader();

		bool findArgumentListIndex(const string & argumentName, size_t & outFrom, size_t & outCount) const;
		vector<string> extractValues(const string & argumentName) const;
		string getArgument(size_t index) const;

	private:
		vector<string> _arguments;
	};
}