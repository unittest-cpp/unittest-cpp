#include "Config.h"

#if defined(UNITTEST_POSIX) || defined(UNITTEST_CYGWIN)
   #include "Posix/TimeHelpers.h"
#else
   #include "Win32/TimeHelpers.h"
#endif
