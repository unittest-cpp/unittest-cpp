#ifndef UNITTEST_PARAMETERIZEDTEST_H
#define UNITTEST_PARAMETERIZEDTEST_H

#include <string>
#include <map>
#include <vector>

#include "Test.h"

namespace UnitTest
{
	using namespace std;

	typedef map<TestDetails const * const, Test*> Details2Test;

	class ParameterizedTestAbstract
	{
	public:
		ParameterizedTestAbstract();
		virtual ~ParameterizedTestAbstract();
		size_t getIteration();

	protected:
		virtual void peekCurrentValue(size_t iteration) = 0;
		virtual size_t valuesSize() const = 0;
		void ensureInitialized();

	private:
		class TestAnchor : public Test
		{
		public:
			TestAnchor(char const* testName, char const* suiteName, ParameterizedTestAbstract & pt);
			virtual void RunImpl() const override;
		private:
			ParameterizedTestAbstract & _pt;
		};

		Test* const retrieveCurrentTest();
		Test* const retrieveTest(TestDetails const * const details);

		bool hasMoreValues(int advance = 0) const;
		void onNewIteration(bool first);

		size_t _iteration;
		Test* _lastTest;
		Test* _nextTestBackup;
		TestAnchor* _testAnchor;
		Details2Test _tests;
	};


	template<class T_Value>
	class ParameterizedTest : public ParameterizedTestAbstract
	{
	public:
		ParameterizedTest(vector<T_Value> values)
			: _values(values)
		{
		}

		T_Value operator()()
		{
			ensureInitialized();
			return _currentValue;
		}

	protected:
		virtual void peekCurrentValue(size_t iteration) override
		{
			_currentValue = _values[iteration];
		}

		virtual size_t valuesSize() const override
		{
			return _values.size();
		}

	private:
		vector<T_Value> _values;
		T_Value _currentValue;
	};
}

#endif