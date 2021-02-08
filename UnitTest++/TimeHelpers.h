#ifndef UNITTEST_TIMEHELPERS_H
#define UNITTEST_TIMEHELPERS_H

#include "Config.h"

#if defined UNITTEST_POSIX
   #include "Posix/TimeHelpers.h"
#elif defined UNITTEST_WIN32
   #include "Win32/TimeHelpers.h"
#else
   #include "Default/TimeHelpers.h"
#endif

namespace UnitTest {

   class Timer
   {
#if defined UNITTEST_POSIX
      typedef UnitTest::Detail::TimerImplPosix TimerImpl;
#elif defined UNITTEST_WIN32
      typedef UnitTest::Detail::TimerImplWin32 TimerImpl;
#else
      typedef UnitTest::Detail::TimerImplDefault TimerImpl;
#endif

   public:
      Timer() {}
      void Start() { m_timer.Start(); }
      double GetTimeInMs() const { return m_timer.GetTimeInMs(); }

   private:
      TimerImpl m_timer;
   };

   namespace TimeHelpers {
      static inline void SleepMs(int ms)
      {
#if defined UNITTEST_POSIX
         UnitTest::Detail::SleepMsImplPosix(ms);
#elif defined UNITTEST_WIN32
         UnitTest::Detail::SleepMsImplWin32(ms);
#else
         UnitTest::Detail::SleepMsImplDefault(ms);
#endif
      }
   }
}

#endif
