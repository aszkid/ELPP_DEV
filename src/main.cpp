#include <el_dev.hpp>


int main()
{
	LOG(el::INFO | el::DEBUG) << "Logging!";

	std::cin.sync(); std::cin.get();
	return 0;
}