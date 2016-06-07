#ifndef UNITTEST_PARAMETERIZEDMANAGER_H
#define UNITTEST_PARAMETERIZEDMANAGER_H

namespace UnitTest
{
	using namespace std;

	class ParameterizedManager
	{
	public:
		static ParameterizedManager & getInstance();

	private:
		ParameterizedManager();
		virtual ~ParameterizedManager();
	};
}

#endif