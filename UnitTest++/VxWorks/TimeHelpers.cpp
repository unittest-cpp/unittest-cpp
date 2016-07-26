#include "TimeHelpers.h"

namespace UnitTest {

   Timer::Timer()
   {
      m_startTime.tv_sec = 0;
      m_startTime.tv_nsec = 0;
   }

   void Timer::Start()
   {
      clock_gettime(CLOCK_REALTIME, &m_startTime);
   }

   double Timer::GetTimeInMs() const
   {
      struct timespec currentTime;
      clock_gettime(CLOCK_REALTIME, &currentTime);

      double const dsecs = currentTime.tv_sec - m_startTime.tv_sec;
      double const dns = currentTime.tv_nsec - m_startTime.tv_nsec;

      return (dsecs * 1000.0) + (dns / 1000000.0);
   }

   void TimeHelpers::SleepMs(int ms)
   {
      struct timespec sleepTime;
      sleepTime.tv_sec = 0;
      sleepTime.tv_nsec = ms * 1000000;
      
      nanosleep(&sleepTime, NULL);
   }

}
