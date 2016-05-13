#ifndef UNITTEST_PARAMETERIZEDSUITE_H
#define UNITTEST_PARAMETERIZEDSUITE_H

#include <vector>
#include "Test.h"

namespace UnitTest
{
	using namespace std;

	class ParameterizedSuiteAbstract
	{
	public:
		ParameterizedSuiteAbstract(const string & suiteName);
		virtual ~ParameterizedSuiteAbstract();
		size_t getIteration() const;

	protected:
		virtual void peekCurrentValue() = 0;
		virtual size_t valuesSize() const = 0;
		void ensureInitialized();
		bool hasMoreValues(int advance = 0) const;

	private:
		class TestAnchor : public Test
		{
		public:
			TestAnchor(ParameterizedSuiteAbstract & parameterized);
			virtual void RunImpl() const override;
		private:
			ParameterizedSuiteAbstract & _parameterized;
		};

		bool tryPeekFirstValue();
		bool tryBranchUniqueTest();
		bool branchTestsForIteration();
		bool unbranchIterationIfLast();
		void onNewIteration();

		volatile bool _iterationBranched;
		size_t _iteration;
		const string _suiteName;
		const string _testName;
		Test* _firstOuterTest;
		Test* _lastOfSuite;
		TestAnchor* _testAnchor;
	};


	/**
	 * @brief Repeat full test suite and gives different values, like a for(:) loop
	 * TODO: iteration should be over tests instead over suite
	 *
	 * @code
	 * SUITE(Testing) {
	 *   vector<int> suitesParams { 1, 5, 10 };
	 *   ParameterizedSuite<int> parameters(UnitTestSuite::GetSuiteName(), suitesParams);
	 *   TEST(Show)
	 *   {
	 *     cout << parameters.getCurrent() << endl;
	 *   }
	 * }
	 * @endcode
	 */
	template<class T_Value>
	class ParameterizedSuite : public ParameterizedSuiteAbstract
	{
	public:
		struct ISuiteIterationListener
		{
			virtual void onNextIteration(T_Value current, size_t iteration) = 0;
		};

		ParameterizedSuite(const string & suiteName, vector<T_Value> values, ISuiteIterationListener* const suiteIterationListener = nullptr)
			: ParameterizedSuiteAbstract(suiteName),
			_values(values),
			_suiteIterationListener(suiteIterationListener)
		{
		}

		T_Value getCurrent()
		{
			ensureInitialized();
			return _currentValue;
		}

		T_Value operator()()
		{
			return getCurrent();
		}

	protected:

		virtual void peekCurrentValue() override
		{
			_currentValue = _values[getIteration()];
			if (_suiteIterationListener != nullptr)
			{
				_suiteIterationListener->onNextIteration(_currentValue, getIteration());
			}
		}

		virtual size_t valuesSize() const override
		{
			return _values.size();
		}

	private:
		ISuiteIterationListener* const _suiteIterationListener;
		vector<T_Value> _values;
		T_Value _currentValue;
	};
}

#endif