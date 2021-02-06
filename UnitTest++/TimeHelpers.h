#include "Config.h"

#if defined UNITTEST_POSIX
   #include "Posix/TimeHelpers.h"
#else
   #if defined UNITTEST_VXWORKS
      #include "VxWorks/TimeHelpers.h"
   #else
      #include "Win32/TimeHelpers.h"
   #endif
#endif
