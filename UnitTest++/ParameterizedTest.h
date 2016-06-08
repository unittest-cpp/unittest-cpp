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
		friend class ParameterizedManager;
	public:
		ParameterizedTestAbstract(const string & name);
		virtual ~ParameterizedTestAbstract();
		size_t getIteration();
		const string & getName() const { return _name; }
		string getNameCurrent() const;

	protected:
		void updateIteration();
		virtual void peekCurrentParameter(TestDetails const * const details, size_t iteration) = 0;
		virtual size_t parametersCount() const = 0;

	private:
		bool hasMoreParameters(int advance = 0) const;
		void onNewIteration(bool first);

		string _name;
		size_t _iteration;
	};


	template<class T_Parameter>
	class ParameterizedTest : public ParameterizedTestAbstract
	{
	public:
		struct IParameterizedTestListener
		{
			virtual void onNextIteration(TestDetails const * const details, T_Parameter current, size_t iteration) = 0;
		};

		ParameterizedTest(vector<T_Parameter> parameters, const string & name = "", IParameterizedTestListener* const listener = nullptr)
			: ParameterizedTestAbstract(name),
			_parameters(parameters),
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
		virtual void peekCurrentParameter(TestDetails const * const details, size_t iteration) override
		{
			if (_listener != nullptr)
			{
				_listener->onNextIteration(details, _parameters[iteration], iteration);
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