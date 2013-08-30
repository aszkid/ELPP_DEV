#include <el_dev.hpp>

#include <vector>

void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
	if(from.empty())
		return;
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

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

	el::utils::time::Benchmark b;

	float median = 0;
	int times = 50;

	for(int i = 0; i < times; i++)
	{
		b.lambda([&](){
			LOG(el::INFO) << "This is info logging";
		});

		float time = el::utils::time::asMilliseconds(b);
		median += time;
	}

	median /= times;

	std::cout << "Took me " << median << " milliseconds." << std::endl;



	std::cout<<"End."<<std::endl;
	std::cin.sync(); std::cin.get();
	return 0;
}