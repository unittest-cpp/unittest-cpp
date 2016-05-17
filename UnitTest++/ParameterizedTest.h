#ifndef UNITTEST_PARAMETERIZEDTEST_H
#define UNITTEST_PARAMETERIZEDTEST_H

#include <string>
#include <map>
#include <vector>

#include "Test.h"
#include "TestList.h"

namespace UnitTest
{
	using namespace std;

	typedef map<TestDetails const * const, TestListNode*> Details2Test;

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
		Test* const getLastTest() const;

	private:
		class TestAnchor : public Test
		{
		public:
			TestAnchor(char const* testName, char const* suiteName, ParameterizedTestAbstract & pt);
			virtual void RunImpl() const override;
		private:
			ParameterizedTestAbstract & _pt;
		};

		TestListNode* const retrieveCurrentTest();
		TestListNode* const retrieveTest(TestDetails const * const details);

		bool hasMoreValues(int advance = 0) const;
		void onNewIteration(bool first);

		size_t _iteration;
		TestListNode* _lastTest;
		TestListNode* _nextTestBackup;
		TestListNode* _testAnchorNode;
		TestAnchor* _testAnchor;
		Details2Test _tests;
	};


	template<class T_Value>
	class ParameterizedTest : public ParameterizedTestAbstract
	{
	public:
		struct IParameterizedTestListener
		{
			virtual void onNextIteration(Test* const test, T_Value current, size_t iteration) = 0;
		};

		ParameterizedTest(vector<T_Value> values, IParameterizedTestListener* const listener = nullptr)
			: _values(values),
			_listener(listener)
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

			if (_listener != nullptr)
			{
				_listener->onNextIteration(getLastTest(), _currentValue, iteration);
			}
		}

		virtual size_t valuesSize() const override
		{
			return _values.size();
		}

	private:
		vector<T_Value> _values;
		T_Value _currentValue;
		IParameterizedTestListener* const _listener;
	};
}

#endif