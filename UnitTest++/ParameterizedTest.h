#ifndef UNITTEST_PARAMETERIZEDTEST_H
#define UNITTEST_PARAMETERIZEDTEST_H

#include <string>
#include <vector>

#include "Test.h"
#include "TestList.h"

namespace UnitTest
{
	using namespace std;

	class ParameterizedTestAbstract
	{
	public:
		ParameterizedTestAbstract();
		virtual ~ParameterizedTestAbstract();
		size_t getIteration();

	protected:
		virtual void peekCurrentParameter(size_t iteration) = 0;
		virtual size_t parametersCount() const = 0;
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

		bool hasMoreParameters(int advance = 0) const;
		void onNewIteration(bool first);
		void newAnchor();

		size_t _iteration;
		TestListNode* _lastTest;
		TestListNode* _nextTestBackup;
		TestListNode* _testAnchorNode;
		TestAnchor* _testAnchor;
	};


	template<class T_Parameter>
	class ParameterizedTest : public ParameterizedTestAbstract
	{
	public:
		struct IParameterizedTestListener
		{
			virtual void onNextIteration(Test* const test, T_Parameter current, size_t iteration) = 0;
		};

		ParameterizedTest(vector<T_Parameter> parameters, IParameterizedTestListener* const listener = nullptr)
			: _parameters(parameters),
			_listener(listener)
		{
		}

		T_Parameter operator()()
		{
			return _parameters[getIteration()];
		}

		const vector<T_Parameter> & parameters() const
		{
			return _parameters;
		}

	protected:
		virtual void peekCurrentParameter(size_t iteration) override
		{
			if (_listener != nullptr)
			{
				_listener->onNextIteration(getLastTest(), _parameters[iteration], iteration);
			}
		}

		virtual size_t parametersCount() const override
		{
			return _parameters.size();
		}

	private:
		vector<T_Parameter> _parameters;
		IParameterizedTestListener* const _listener;
	};
}

#endif