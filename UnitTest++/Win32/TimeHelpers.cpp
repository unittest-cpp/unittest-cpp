#include "TimeHelpers.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace UnitTest {
namespace Detail {

   TimerImplWin32::TimerImplWin32()
      : m_threadHandle(::GetCurrentThread())
      , m_startTime(0)
   {
#if defined(UNITTEST_WIN32) && (_MSC_VER == 1200) // VC6 doesn't have DWORD_PTR
      typedef unsigned long DWORD_PTR;
#endif

      DWORD_PTR systemMask;
      ::GetProcessAffinityMask(GetCurrentProcess(), &m_processAffinityMask, &systemMask);
      ::SetThreadAffinityMask(m_threadHandle, 1);
      ::QueryPerformanceFrequency(reinterpret_cast< LARGE_INTEGER* >(&m_frequency));
      ::SetThreadAffinityMask(m_threadHandle, m_processAffinityMask);
   }

   void TimerImplWin32::Start()
   {
      m_startTime = GetTime();
   }

   double TimerImplWin32::GetTimeInMs() const
   {
      __int64 const elapsedTime = GetTime() - m_startTime;
      double const seconds = double(elapsedTime) / double(m_frequency);
      return seconds * 1000.0;
   }

   __int64 TimerImplWin32::GetTime() const
   {
      LARGE_INTEGER curTime;
      ::SetThreadAffinityMask(m_threadHandle, 1);
      ::QueryPerformanceCounter(&curTime);
      ::SetThreadAffinityMask(m_threadHandle, m_processAffinityMask);
      return curTime.QuadPart;
   }

   void SleepMsImplWin32(int ms)
   {
      ::Sleep(ms);
   }

}
}
