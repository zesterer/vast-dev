#ifndef VAST_HPP_UTIL_PANIC
#define VAST_HPP_UTIL_PANIC

// Std
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>

namespace vast::util
{
	[[noreturn]] void panic(std::string const& msg, const char* file = __FILE__, int line = __LINE__);
}

#endif
