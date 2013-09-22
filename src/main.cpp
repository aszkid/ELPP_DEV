#include <el_dev.hpp>

#include <vector>



int main()
{
	//el::initialize();
	
	/*std::string format = "[%datetime%] %level% -> %log%";
	std::string final = format;

	std::vector<std::pair<std::string,std::string>> vars;
	vars.push_back(std::make_pair("%datetime%", "25/05/2013 19:25:03"));
	vars.push_back(std::make_pair("%level%", "ERROR"));
	vars.push_back(std::make_pair("%log%", "This is so wrong"));

	for(auto& el : vars)
	{
		replaceAll(final, el.first, el.second);
	}

	std::cout << "FORMAT: " << format << std::endl;
	std::cout << "FINAL: " << final << std::endl;*/

	el::initialize();
	el::setGlobalConfig("../log_config.ini");

	LOG(el::GENERAL) << "Some info you " << 10 << " fuckers.";

	float median = 0;
	int times = 10;


	/*el::utils::time::Benchmark b;


	for(int i = 0; i < times; i++)
	{
		b.lambda([&](){
			LOG(el::INFO) << "This is info logging";
		});

		float time = el::utils::time::asMilliseconds(b);
		median += time;
	}

	median /= times;

	COUT("Took me " << median << " milliseconds to do a simple non-file log.");*/



	std::cout<<"End."<<std::endl;
	std::cin.sync(); std::cin.get();
	return 0;
}