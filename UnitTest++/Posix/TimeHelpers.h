#ifndef UNITTEST_POSIX_TIMEHELPERS_H
#define UNITTEST_POSIX_TIMEHELPERS_H

#include <sys/time.h>

namespace UnitTest {
namespace Detail {

   class TimerImplPosix
   {
   public:
      TimerImplPosix();
      void Start();
      double GetTimeInMs() const;

   private:
      struct timeval m_startTime;
   };

   void SleepMsImplPosix(int ms);
}
}

#endif
