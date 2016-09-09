#include "TestDetails.h"
#include <cstring> // "c" version required for gcc

namespace UnitTest {

   TestDetails::TestDetails(char const* testName_, char const* suiteName_, char const* filename_, int lineNumber_)
      : suiteName(suiteName_)
      , testName(testName_)
      , filename(filename_)
      , lineNumber(lineNumber_)
      , timeConstraintExempt(false)
   {}

   TestDetails::TestDetails(const TestDetails& details, int lineNumber_)
      : suiteName(details.suiteName)
      , testName(details.testName)
      , filename(details.filename)
      , lineNumber(lineNumber_)
      , timeConstraintExempt(details.timeConstraintExempt)
   {}

   bool TestDetails::sameTest(const TestDetails & details) const
   {
      if (&details == this)
      {
         return true;
      }

      // Fast pointer comparison
      if (details.suiteName == suiteName &&
         details.testName == testName &&
         details.filename == filename)
      {
         return true;
      }

      // Long string comparison
      if (!strcmp(details.suiteName, suiteName) &&
         !strcmp(details.testName, testName) &&
         !strcmp(details.filename, filename))
      {
         return true;
      }

      return false;
   }
}
