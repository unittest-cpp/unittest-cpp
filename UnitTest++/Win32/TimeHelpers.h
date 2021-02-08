#ifndef UNITTEST_WIN32_TIMEHELPERS_H
#define UNITTEST_WIN32_TIMEHELPERS_H

#include "../Config.h"
#include "../HelperMacros.h"

#ifdef UNITTEST_MINGW
#ifndef __int64
#define __int64 long long
#endif
#endif

namespace UnitTest {
namespace Detail {

   class UNITTEST_LINKAGE TimerImplWin32
   {
   public:
      TimerImplWin32();
      void Start();
      double GetTimeInMs() const;

   private:
      __int64 GetTime() const;

      void* m_threadHandle;

#if defined(_WIN64)
      unsigned __int64 m_processAffinityMask;
#else
      unsigned long m_processAffinityMask;
#endif

      __int64 m_startTime;
      __int64 m_frequency;
   };

   UNITTEST_LINKAGE void SleepMsImplWin32(int ms);
}
}

#endif
