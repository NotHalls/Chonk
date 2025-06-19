#pragma once

#include <iostream>

#ifdef CHK_DEBUG
#define CHK_ASSERT(expr, msg)                                                  \
  {                                                                            \
    if(!(expr))                                                                \
    {                                                                          \
      std::cerr << "ASSERTION CALLED: " + std::string(msg);                    \
      throw std::runtime_error("ASSERTION CALLED: " + std::string(msg));       \
    }                                                                          \
  }
#else
#define CHK_ASSERT(expr, msg)
#endif