#include <el_dev.hpp>

int main()
{
	Benchmark b;

	b.lambda([&](){
		LOG(el::INFO) << "I'm " << 20 << " years old.";
	});
	
	LOG(el::DEBUG) << "A simple info log took: " << TIME_AS_MICRO(b) << " milliseconds.";

	std::cin.sync(); std::cin.get();
	return 0;
}