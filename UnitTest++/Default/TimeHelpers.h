#ifndef UNITTEST_DEFAULT_TIMEHELPERS_H
#define UNITTEST_DEFAULT_TIMEHELPERS_H

#include <sys/time.h>

// An implementation on platforms that are not supported

namespace UnitTest {
namespace Detail {

   class TimerImplDefault
   {
   public:
      TimerImplDefault();
      void Start();
      double GetTimeInMs() const;

   private:
   };

   void SleepMsImplDefault(int ms);
}
}

#endif
