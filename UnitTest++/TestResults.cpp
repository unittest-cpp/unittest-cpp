#include "TestResults.h"
#include "TestReporter.h"

#include "TestDetails.h"
#include "MemoryOutStream.h"
#include "ParameterizedManager.h"

namespace UnitTest {

   TestResults::TestResults(TestReporter* testReporter)
      : m_testReporter(testReporter)
      , m_totalTestCount(0)
      , m_failedTestCount(0)
      , m_failureCount(0)
      , m_currentTestFailed(false)
   {}

   void TestResults::OnTestStart(TestDetails const& test)
   {
      ++m_totalTestCount;
      m_currentTestFailed = false;
      if (m_testReporter)
         m_testReporter->ReportTestStart(test);
   }

   void TestResults::OnTestFailure(TestDetails const& test, char const* failure)
   {
      ++m_failureCount;
      if (!m_currentTestFailed)
      {
         ++m_failedTestCount;
         m_currentTestFailed = true;
      }

      if (m_testReporter)
	  {
         MemoryOutStream stream;
         stream << failure;
         vector<TestParameterAbstract*> parameters = ParameterizedManager::getInstance().getStack(&test);
         if (!parameters.empty())
         {
            stream << " Parameters: ";
            for (size_t i = 0; i < parameters.size(); i++)
            {
               if (i != 0)
               {
                  stream << ", ";
               }
               stream << parameters[i]->getNameCurrent();
            }
         }
         m_testReporter->ReportFailure(test, stream.GetText());
      }
   }

   void TestResults::OnTestFinish(TestDetails const& test, float secondsElapsed)
   {
      if (m_testReporter)
         m_testReporter->ReportTestFinish(test, secondsElapsed);
   }

   int TestResults::GetTotalTestCount() const
   {
      return m_totalTestCount;
   }

   int TestResults::GetFailedTestCount() const
   {
      return m_failedTestCount;
   }

   int TestResults::GetFailureCount() const
   {
      return m_failureCount;
   }


}
