#include <chrono>
#include <thread>
#include <stdint.h>

extern "C" {
	uint64_t timenow()
	{
		return ((std::chrono::duration_cast<std::chrono::milliseconds> 
		((std::chrono::time_point_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now())).time_since_epoch())).count()) ;
	}
}
