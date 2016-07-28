#include "TestRunner.h"
#include "TestResults.h"
#include "TestReporter.h"
#include "TestReporterStdout.h"
#include "TimeHelpers.h"
#include "MemoryOutStream.h"
#include "SuitePredicate.h"
#include "ParameterizedManager.h"

#include <cstring>


namespace UnitTest {

   int RunAllTests()
   {
      TestReporterStdout reporter;
      TestRunner runner(reporter);
      return runner.RunTestsIf(Test::GetTestList(), NULL, True(), 0);
   }

   bool findArgumentListIndex(int argc, char**argv, char const* argument, int & outFrom, int & outCount)
   {
      if (argc <= 1)
      {
         return false;
      }

      outFrom = -1;
      for (int i = 1; i < argc; i++)
      {
         if (strcmp(argument, argv[i]) == 0)
         {
            outFrom = i;
            break;
         }
      }
      if (outFrom == -1)
      {
         return false;
      }
      outFrom++;
      outCount = 0;
      for (int i = outFrom; i < argc; i++)
      {
         char* value = argv[i];
         if (strlen(value) >= 2 && value[0] == '-'&& value[1] == '-')
         {
            break;
         }
         outCount++;
      }

      return true;
   }

   bool readSuiteArgument(SuitePredicate & predicate, int argc, char**argv, char const* argument)
   {
      int from, count;
      if (!findArgumentListIndex(argc, argv, argument, from, count))
      {
         return false;
      }
      for (int i = from; i < from + count; i++)
      {
         predicate.addSuite(argv[i]);
      }
      return true;
   }

   bool readTestArgument(SuitePredicate & predicate, int argc, char**argv, char const* argument)
   {
      int from, count;
      if (!findArgumentListIndex(argc, argv, argument, from, count))
      {
         return false;
      }
      for (int i = from; i < from + count; i++)
      {
         predicate.addTest(argv[i]);
      }
      return true;
   }

   bool readIgnoreParamArgument(int argc, char**argv, char const* argument)
   {
	   int from, count;
	   if (!findArgumentListIndex(argc, argv, argument, from, count))
	   {
		   return false;
	   }
	   for (int i = from; i < from + count; i++)
	   {
		   ParameterizedManager::getInstance().ignoreIndexes(argv[i]);
	   }
	   return true;
   }

   int RunTestsCmd(int argc, char**argv, char const* suiteArgument, char const* testArgument, char const* ignoreParamArgument)
   {
      SuitePredicate predicate;

      bool specific = false;
      specific |= readSuiteArgument(predicate, argc, argv, suiteArgument);
      specific |= readTestArgument(predicate, argc, argv, testArgument);

	  readIgnoreParamArgument(argc, argv, ignoreParamArgument);

      if (!specific)
      {
         predicate.addAll();
      }

      //run selected test(s) only
      TestReporterStdout reporter;
      TestRunner runner(reporter);

      return runner.RunTestsIf(Test::GetTestList(), 0, predicate, 0);
   }

   TestRunner::TestRunner(TestReporter& reporter)
      : m_reporter(&reporter)
      , m_result(new TestResults(&reporter))
      , m_timer(new Timer)
   {
      m_timer->Start();
   }

   TestRunner::~TestRunner()
   {
      delete m_result;
      delete m_timer;
   }

   TestResults* TestRunner::GetTestResults()
   {
      return m_result;
   }

   int TestRunner::Finish() const
   {
      float const secondsElapsed = static_cast<float>(m_timer->GetTimeInMs() / 1000.0);
      m_reporter->ReportSummary(m_result->GetTotalTestCount(),
                                m_result->GetFailedTestCount(),
                                m_result->GetFailureCount(),
                                secondsElapsed);

      return m_result->GetFailureCount();
   }

   bool TestRunner::IsTestInSuite(const Test* const curTest, char const* suiteName) const
   {
      using namespace std;
      return (suiteName == NULL) || !strcmp(curTest->m_details.suiteName, suiteName);
   }

   void TestRunner::RunTest(TestResults* const result, Test* const curTest, int const maxTestTimeInMs) const
   {
      if (curTest->m_isMockTest == false)
         CurrentTest::Results() = result;

      Timer testTimer;
      testTimer.Start();

      result->OnTestStart(curTest->m_details);

      curTest->Run();

      double const testTimeInMs = testTimer.GetTimeInMs();
      if (maxTestTimeInMs > 0 && testTimeInMs > maxTestTimeInMs && !curTest->m_details.timeConstraintExempt)
      {
         MemoryOutStream stream;
         stream << "Global time constraint failed. Expected under " << maxTestTimeInMs <<
            "ms but took " << testTimeInMs << "ms.";

         result->OnTestFailure(curTest->m_details, stream.GetText());
      }

      result->OnTestFinish(curTest->m_details, static_cast< float >(testTimeInMs / 1000.0));
   }

}
