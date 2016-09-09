#ifndef UNITTEST_TESTRUNNER_H
#define UNITTEST_TESTRUNNER_H

#include "Test.h"
#include "TestList.h"
#include "CurrentTest.h"

namespace UnitTest {

   class TestReporter;
   class TestResults;
   class Timer;

   /**
    * Run tests/suites using custom selection from command line.
	*
	* Commands:
	*  --test One or multiple test names to execute, can be combined with --suite
	*  --suite One or multiple suite names to execute, can be combined with --test
	*
	* Special feature: You do not have to specify explicitely --test and --suite, you
	* can mix suite names and test names and the cmd will find the way. The
	* constraint is that it must have no arguments beginning with --.
	* For disabling this feature, set allowImplicitArgs=false
	*
	* Usage examples:
	*  Explicit: myTests.exe --suite MySuite1 MyOtherSuite --test MySpecialTest MyOtherTest
	*  Implicit: myTests.exe MySpecialTest MyOtherTest MySuite1
	*/
   UNITTEST_LINKAGE int RunTestsCmd(int argc, char**argv, bool allowImplicitArgs = true);
   
   UNITTEST_LINKAGE int RunAllTests();

   struct True
   {
      bool operator()(const Test* const) const
      {
         return true;
      }
   };

   class UNITTEST_LINKAGE TestRunner
   {
   public:
      explicit TestRunner(TestReporter& reporter);
      ~TestRunner();

      template< class Predicate >
      int RunTestsIf(TestList const& list, char const* suiteName,
                     const Predicate& predicate, int maxTestTimeInMs) const
      {
         Test* curTest = list.GetHead();

         while (curTest != 0)
         {
            if (IsTestInSuite(curTest, suiteName) && predicate(curTest))
               RunTest(m_result, curTest, maxTestTimeInMs);

            curTest = curTest->m_nextTest;
         }

         return Finish();
      }

      TestResults* GetTestResults();

   private:
      TestReporter* m_reporter;
      TestResults* m_result;
      Timer* m_timer;

      int Finish() const;
      bool IsTestInSuite(const Test* const curTest, char const* suiteName) const;
      void RunTest(TestResults* const result, Test* const curTest, int const maxTestTimeInMs) const;
   };

}

#endif
