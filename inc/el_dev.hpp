#pragma once

#include <iostream>
#include <fstream>
#include <map>

namespace el
{
	// Logger level definitions
	enum class Level : unsigned short {
		Info = 1,
		Debug = 2,
		Warning = 4,
		Error = 8,
		General = 16
	};

	// HELP Namespace - helper functions, used by EL
	namespace help
	{
		inline bool file_exists(const char* filename)
		{
			std::ifstream f(filename);
			return (!f ? false : true);
		}
	}
	// INTERNAL Namespace - should not be used by the user
	namespace internal
	{
		class Config
		{
		public:
			Config(bool default = true)
			{
				if(default) {
					// Apply default config
				}
				else {
					// Apply non-default config
				}
			}
		private:

		};

		class Writer
		{
			
		};

		class Logger
		{
		public:
			Logger()
			{}
			Config cfg;
			
		};

		class Manager
		{
		private:
			Logger yeah;
			std::map<char*,Logger> loggers;
		public:
			Logger& get(char* logger_name)
			{
				std::cout << "Looks like it's working..." << std::endl;
				return loggers[logger_name];
			}
		};
	}

	// Global variables, that will hold logger configuration
	internal::Manager log_manager = internal::Manager();
	
	inline el::internal::Logger& _LOG(char* logger_name = "_default", el::Level level = el::Level::General)
	{
		return el::log_manager.get(logger_name);
	}

#define LERROR(name) _LOG(name, el::Level::Error)
#define LINFO(name) _LOG(name, el::Level::Info)
#define LDEBUG(name) _LOG(name, el::Level::Debug)
#define LWARNING(name) _LOG(name, el::Level::Warning)
#define LGENERAL(name) _LOG(name, el::Level::General)

}

#define LOG(name, level) el::L##level(name)
#define LOG(name, name2, level) el::L##level(name); el::L##level(name2)
#define LOG(name, name2, name3, level) el::L##level(name); el::L##level(name2); el::L##level(name3)
