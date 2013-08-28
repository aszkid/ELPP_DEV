#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <sstream>
#include <string>
#include <mutex>
#include <memory>

namespace el
{
	// Useful little macros
	// ------------------------
	#define PTRME (*this)

	// Type conventions
	// ------------------------
	typedef unsigned int uint;
	typedef char* cstr;

	// Logger level definitions
	// ------------------------
	namespace Level
	{
		enum : unsigned short {
			Info = 1 << 0,
			Debug = 1 << 1,
			Warning = 1 << 2,
			Error = 1 << 3,
			General = 1 << 4
		};
	}

	// UTILS Namespace - helper functions
	// ------------------------
	namespace utils
	{
		inline bool cast(int a) {
			return (a == 1 ? true : false);
		}

		inline bool file_exists(const cstr filename)
		{
			std::ifstream f(filename);
			return (!f ? false : true);
		}
	}
	// INTERNAL Namespace - should not be used by the user
	// ------------------------
	namespace internal
	{
		// Smart mutex - adds lambda function for auto lock-unlock
		// ------------------------
		class smart_mutex : public std::mutex
		{
		public:
			template<typename functor>
			void lambda(functor func) {
				lock();
					func();
				unlock();
			}
		};

		class Writer
		{
		public:
			smart_mutex w_Mutex;
			std::stringstream stream;

			inline Writer& operator<<(const std::string& s) {
				w_Mutex.lambda([&](){
					stream << s;
				});
				return PTRME;
			}
		};

		class Logger
		{
		public:
			Logger()
			{}
			Writer& getWriter()
			{
				return writer;
			}
		private:
			Writer writer;
		};

		class Manager
		{
		private:
			Logger yeah;
			std::map<cstr,Logger> loggers;
		public:
			Writer& write(cstr logger_name)
			{
				return loggers[logger_name].getWriter();
			}
			Manager()
			{
				loggers["_default"] = Logger();
			}
			~Manager()
			{}
		};
	}
	
	// ------------------------
	// Configuration class
	// ------------------------
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
		bool toFile;
	};

	// Important variables
	// ------------------------
	std::unique_ptr<internal::Manager> log_manager;
	std::unique_ptr<Config> manager_config;

	// Initialization function
	// ------------------------
	inline bool initialize(Config cfg)
	{
		
	}

	// Macros for easy access to levels
	// ------------------------
	#define GENERAL Level::General
	#define ERROR Level::Error
	#define WARNING Level::Warning
	#define DEBUG Level::Debug
	#define INFO Level::Info
}


inline el::internal::Writer& LOG(el::uint level = el::Level::General, el::cstr logger_name = "_default")
{
	if(el::log_manager)
	{
		if(level & el::GENERAL) {
		
		}
		if(level & el::ERROR) {
	
		}

		return el::log_manager->write(logger_name);
	}
	else
	{
		throw std::exception("FATAL: The logging manager has not been initialized.");
	}
}