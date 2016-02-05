#include "ThrowingTestReporter.h"
#include "AssertException.h"

namespace UnitTest {

   ThrowingTestReporter::ThrowingTestReporter(TestReporter* decoratedReporter)
      : m_decoratedReporter(decoratedReporter)
   {}

   //virtual
   ThrowingTestReporter::~ThrowingTestReporter()
   {}

   //virtual
   void ThrowingTestReporter::ReportTestStart(TestDetails const& test)
   {
      if(m_decoratedReporter) m_decoratedReporter->ReportTestStart(test);
   }

   //virtual
   void ThrowingTestReporter::ReportFailure(TestDetails const& test, char const* failure)
   {
      if(m_decoratedReporter) m_decoratedReporter->ReportFailure(test, failure);
      throw AssertException();
   }

   //virtual
   void ThrowingTestReporter::ReportTestFinish(TestDetails const& test, float secondsElapsed)
   {
      if(m_decoratedReporter) m_decoratedReporter->ReportTestFinish(test, secondsElapsed);
   }

   //virtual
   void ThrowingTestReporter::ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed)
   {
      if(m_decoratedReporter) m_decoratedReporter->ReportSummary(totalTestCount, failedTestCount, failureCount, secondsElapsed);
   }

   TestReporter* ThrowingTestReporter::getDecorated() const
   {
      return m_decoratedReporter;
   }

   void ThrowingTestReporter::setDecorated(TestReporter* reporter)
   {
      m_decoratedReporter = reporter;
   }

}
