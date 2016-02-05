#include "RequiredCheckTestReporter.h"

#include "CurrentTest.h"
#include "TestResults.h"

namespace UnitTest {

   RequiredCheckTestReporter::RequiredCheckTestReporter(TestResults* results)
      : m_results(results)
      , m_throwingReporter(0)
      , m_continue(0)
   {
      if(m_results)
      {
         m_throwingReporter.setDecorated(m_results->m_testReporter);
         m_results->m_testReporter = &m_throwingReporter;
      }
   }

   RequiredCheckTestReporter::~RequiredCheckTestReporter()
   {
      if(m_results) m_results->m_testReporter = m_throwingReporter.getDecorated();
   }

   bool RequiredCheckTestReporter::next()
   {
      return m_continue++ == 0;
   }
}