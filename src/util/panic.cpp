// Local
#include <util/panic.hpp>

namespace vast::util
{
	[[noreturn]] void panic(std::string const& msg, const char* file, int line)
	{
		std::cout << "[PANIC on line " << line << " in " << (std::strrchr(file, '/') ? std::strrchr(file, '/') + 1 : file) << "] " << msg << std::endl;
		std::exit(1);
	}
}
