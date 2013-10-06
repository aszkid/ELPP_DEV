#include <el_dev.hpp>

#include <iostream>

std::string getCompiler()
{
#ifdef __clang__
	return "CLANG";
#elif __GNUG__
	return "GCC";
#elif _MSC_VER
	return "VC++";
#else
	return "UNKOWN";
#endif
}

int main()
{
	el::initialize();
	LOG(el::DEBUG) << "USING COMPILER: " << getCompiler();

	/*float median = 0;
	int times = 20;
	
	el::utils::time::Benchmark b;

	for(int i = 0; i < times; i++)
	{
		b.lambda([&](){
			LOG(el::INFO) << "This is info logging";
		});
		median += el::utils::time::asMilliseconds(b);
	}

	median /= times;

	COUT("Took me " << median << " milliseconds.");*/
	
	return 0;
}
