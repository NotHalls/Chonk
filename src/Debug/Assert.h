#pragma once
#include <iostream>

#define CHK_ASSERT(x, msg)                                                     \
  {                                                                            \
    if(!(x))                                                                   \
    {                                                                          \
      std::cerr << "ASSERTION CALLED: " + std::string(msg);                    \
      throw std::runtime_error("ASSERTION CALLED: " + std::string(msg));       \
    }                                                                          \
  }