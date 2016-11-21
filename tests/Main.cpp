#include "UnitTest++/UnitTestPP.h"
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
	#include <float.h>
#endif

int main(int, char const *[])
{
#ifdef __BORLANDC__
   // This is to allow the check test to pass when using Nan (invalid float numbers), otherwise, they throw an exception
   // and the test fails (CheckCloseWithNaNFails and CheckCloseWithNaNAgainstItselfFails)
   _control87(MCW_EM, MCW_EM);
#endif
   return UnitTest::RunAllTests();
}
