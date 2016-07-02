#ifndef UNITTEST_TESTPARAMETER_H
#define UNITTEST_TESTPARAMETER_H

#include <string>
#include <vector>
#include <algorithm>

#include "Test.h"
#include "TestList.h"

namespace UnitTest
{
	using namespace std;

	class TestParameterAbstract
	{
		friend class ParameterizedManager;
	public:
		TestParameterAbstract(const string & name);
		virtual ~TestParameterAbstract();
		size_t getCurrentIndex();
		const string & getName() const { return _name; }
		string getNameCurrent() const;
		TestParameterAbstract & ignoreIndex(size_t index);

	protected:
		void updateCurrentIndex();
		virtual void peekCurrentParameter(TestDetails const * const details, size_t index) = 0;
		virtual size_t parametersCount() const = 0;

	private:
		bool hasMoreParameters(int advance = 0) const;
		void nextIndex(bool first);

		string _name;
		size_t _index;
	};


	template<class T_Parameter>
	class TestParameter : public TestParameterAbstract
	{
	public:
		struct IParameterListener
		{
			virtual void onNext(TestDetails const * const details, T_Parameter current, size_t index) = 0;
		};

		TestParameter(const string & name, vector<T_Parameter> parameters, IParameterListener* const listener = nullptr)
			: TestParameterAbstract(name),
			_parameters(parameters),
			_listener(listener)
		{
		}

		T_Parameter operator()()
		{
			return _parameters[getCurrentIndex()];
		}

		const vector<T_Parameter> & parameters() const
		{
			return _parameters;
		}

		TestParameter<T_Parameter> & ignoreIndex(size_t index)
		{
			TestParameterAbstract::ignoreIndex(index);
			return *this;
		}

		TestParameter<T_Parameter> & ignore(T_Parameter p)
		{
			vector<T_Parameter>::iterator it = find(_parameters.begin(), _parameters.end(), p);
			if (it == _parameters.end())
			{
				return *this;
			}
			return ignoreIndex(it - _parameters.begin());
		}

	protected:
		virtual void peekCurrentParameter(TestDetails const * const details, size_t index) override
		{
			if (_listener != nullptr)
			{
				_listener->onNext(details, _parameters[index], index);
			}
		}

		virtual size_t parametersCount() const override
		{
			return _parameters.size();
		}

	private:
		vector<T_Parameter> _parameters;
		IParameterListener* const _listener;
	};
}

#endif