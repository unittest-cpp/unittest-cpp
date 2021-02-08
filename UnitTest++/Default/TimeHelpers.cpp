#include "TimeHelpers.h"
#include <unistd.h>

namespace UnitTest {
namespace Detail {

   TimerImplDefault::TimerImplDefault()
   {
   }

   void TimerImplDefault::Start()
   {
   }

   double TimerImplDefault::GetTimeInMs() const
   {
      return 0;
   }

   void SleepMsImplDefault(int ms)
   {
   }

}
}
