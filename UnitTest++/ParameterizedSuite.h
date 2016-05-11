#ifndef UNITTEST_PARAMETERIZEDSUITE_H
#define UNITTEST_PARAMETERIZEDSUITE_H

#include <vector>
#include <iostream>

#include "Test.h"

namespace UnitTest
{
	using namespace std;

	class ParameterizedSuiteAbstract
	{
	public:

		ParameterizedSuiteAbstract(const string & suiteName)
			: _suiteName(suiteName),
			_testName(suiteName + "ParameterizedSuite_starter"),
			_testAnchor(nullptr) // Important, even if defined just above (please read comment)
		{
			// WARNING: this is pointer because of memory problems with suiteName/testName.c_str(),
			// the constructor does not initialize them in the right order
			_testAnchor = new TestAnchor(*this);
		}

		virtual ~ParameterizedSuiteAbstract()
		{
			if (_testAnchor != nullptr)
			{
				delete _testAnchor;
				_testAnchor = nullptr;
			}
		}

		size_t getIteration()
		{
			return _iteration;
		}

	protected:

		virtual void peekCurrentValue() = 0;
		virtual size_t valuesSize() = 0;

		void ensureInitialized()
		{
			tryPeekFirstValue();
			tryBranchUniqueTest();
		}

		bool hasMoreValues(int advance = 0)
		{
			return (_iteration + advance < (int)valuesSize());
		}

	private:

		class TestAnchor : public Test
		{
		public:
			TestAnchor(ParameterizedSuiteAbstract & parameterized)
				: Test(parameterized._testName.c_str(), parameterized._suiteName.c_str()),
				_parameterized(parameterized)
			{
				Test::GetTestList().Add(this);
			}
			virtual void RunImpl() const override
			{
				_parameterized.onNewIteration();
			}
		private:
			ParameterizedSuiteAbstract & _parameterized;
		};

		bool tryPeekFirstValue()
		{
			if (_iterationBranched)
			{
				return false;
			}

			peekCurrentValue();
			return false;
		}

		bool tryBranchUniqueTest()
		{
			if (_iterationBranched)
			{
				return false;
			}

			//TODO find a way to allow single parameterized test execution
			static bool messageDisplayed = false;
			if (!messageDisplayed)
			{
				messageDisplayed = true;
				cout << "WARNING: test of parameterized suite " << _suiteName << " can not be executed alone, test will be executed with first value only.";
			}

			return true;
		}

		bool branchTestsForIteration()
		{
			if (_iterationBranched)
			{
				return false;
			}

			for (Test* iTest = _testAnchor; iTest != nullptr; iTest = iTest->m_nextTest)
			{
				bool inSameSuite = (strcmp(iTest->m_details.suiteName, _suiteName.c_str()) == 0);
				bool ownAnchor = (iTest == _testAnchor);
				bool isOtherParameterizedSuite = (iTest != _testAnchor && dynamic_cast<TestAnchor*>(iTest) != nullptr);
				if (!inSameSuite || isOtherParameterizedSuite)
				{
					_firstOuterTest = iTest;
					break;
				}
				_lastOfSuite = iTest;
			}

			_lastOfSuite->m_nextTest = _testAnchor;

			_iterationBranched = true;
			return true;
		}

		bool unbranchIterationIfLast()
		{
			if (!_iterationBranched)
			{
				return false;
			}

			if (hasMoreValues(1))
			{
				return false;
			}

			_lastOfSuite->m_nextTest = _firstOuterTest;
			return true;
		}

		void onNewIteration()
		{
			if (_iterationBranched)
			{
				_iteration++;
			}

			bool justBranched = branchTestsForIteration();
			bool justUnbranched = unbranchIterationIfLast();

			if (justBranched && justUnbranched)
			{
				// If no values to test, skip all tests
				if (valuesSize() == 0)
				{
					_testAnchor->m_nextTest = _firstOuterTest;
					return;
				}
			}

			peekCurrentValue();
		}

		volatile bool _iterationBranched = false;
		size_t _iteration = 0;
		const string _suiteName;
		const string _testName;
		Test* _firstOuterTest = nullptr;
		Test* _lastOfSuite = nullptr;
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

		virtual size_t valuesSize() override
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