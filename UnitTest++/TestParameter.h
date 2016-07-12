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
		virtual void peekCurrent(TestDetails const * const details, size_t index) = 0;
		virtual size_t valuesCount() const = 0;

	private:
		bool hasMoreValues(int advance = 0) const;
		void nextIndex(bool first);

		string _name;
		size_t _index;
	};


	template<class T>
	class TestParameter : public TestParameterAbstract
	{
	public:
		struct IParameterListener
		{
			virtual void onNext(TestDetails const * const details, T currentValue, size_t currentIndex) = 0;
		};

		TestParameter(const string & name, vector<T> values, IParameterListener* const listener = nullptr)
			: TestParameterAbstract(name),
			_values(values),
			_listener(listener)
		{
		}

		T operator()()
		{
			return _values[getCurrentIndex()];
		}

		const vector<T> & values() const
		{
			return _values;
		}

		TestParameter<T> & ignoreIndex(size_t index)
		{
			TestParameterAbstract::ignoreIndex(index);
			return *this;
		}

		TestParameter<T> & ignore(T p)
		{
			vector<T>::iterator it = find(_values.begin(), _values.end(), p);
			if (it == _values.end())
			{
				return *this;
			}
			return ignoreIndex(it - _values.begin());
		}

	protected:
		virtual void peekCurrent(TestDetails const * const details, size_t index) override
		{
			if (_listener != nullptr)
			{
				_listener->onNext(details, _values[index], index);
			}
		}

		virtual size_t valuesCount() const override
		{
			return _values.size();
		}

	private:
		vector<T> _values;
		IParameterListener* const _listener;
	};
}

#endif