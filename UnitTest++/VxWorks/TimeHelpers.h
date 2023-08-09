#ifndef UNITTEST_TIMEHELPERS_H
#define UNITTEST_TIMEHELPERS_H

#include <timers.h>

namespace UnitTest {

   class Timer
   {
   public:
      Timer();
      void Start();
      double GetTimeInMs() const;

   private:
      struct timespec m_startTime;
   };


   namespace TimeHelpers
   {
      void SleepMs(int ms);
   }


}

#endif
