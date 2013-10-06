#include <el_dev.hpp>

int main()
{
	el::initialize();
	//el::setGlobalConfig("mother");
	
	LOG() << "Yeah, this even works well: " << 32;

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
