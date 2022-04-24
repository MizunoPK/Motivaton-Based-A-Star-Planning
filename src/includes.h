#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Boost free logging, NOT thread safe.
inline int LOGGING_LEVEL;
#define FATAL   if (LOGGING_LEVEL > -1) std::cout << "FATAL: "
#define ERROR   if (LOGGING_LEVEL > 0) std::cout  << "ERROR: "
#define WARNING if (LOGGING_LEVEL > 1) std::cout  << "WARNING: "
#define INFO    if (LOGGING_LEVEL > 2) std::cout  << "INFO: "
#define DEBUG   if (LOGGING_LEVEL > 3) std::cout  << "DEBUG: "
#define TRACE   if (LOGGING_LEVEL > 4) std::cout  << "TRACE: "
#define DEEP_TRACE   if (LOGGING_LEVEL > 5) std::cout  << "DEEP_TRACE: "
#define A_STAR_TRACE   if (LOGGING_LEVEL > 6) std::cout  << "A_STAR_TRACE: "
#define FUNCTION_TRACE   if (LOGGING_LEVEL > 7) std::cout  << "FUNCTION_TRACE: "
#define ENDL  " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl;
#define NEWL  std::cout << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl;