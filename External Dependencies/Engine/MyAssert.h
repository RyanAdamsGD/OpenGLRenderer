#ifndef MYASSERT_H
#define MYASSERT_H
#include "Logger.h"
#include <cstdlib>

#define USE_ASSERTS

#if defined USE_ASSERTS && defined USE_LOGGER
#define ASSERT(ex) do{ if(!(ex)) LOGE("Fatal Error") exit(1); } while(0);
#else
#define ASSERT
#endif

#endif