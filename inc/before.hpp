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
	namespace WriteDest
	{
		enum : unsigned short {
			StdOut = 1 << 0,
			File = 1 << 1
		};
	}

	// UTILS Namespace - helper functions
	// ------------------------
	namespace utils
	{
		template<class T>
		inline std::unique_ptr<T> makeUniquePtr()
		{
			return std::unique_ptr<T>(new T());
		}
		template<class T>
		inline std::unique_ptr<T> makeUniquePtr(const T& data)
		{
			return std::unique_ptr<T>(new T(data));
		}

		inline bool file_exists(const cstr filename)
		{
			std::ifstream f(filename);
			return (!f ? false : true);
		}
	}
	
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

	// INTERNAL Namespace - should not be used by the user
	// ------------------------
	namespace internal
	{
		class Writer
		{
		private:
			std::stringstream stream;

		public:
			Writer& operator<<(const std::string& s) {
				stream << s;
				return PTRME;
			}
		};

		/*class Logger
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
			std::map<cstr,Logger> loggers;
		public:
			Writer& write(cstr logger_name, el::uint level)
			{
				return loggers[logger_name].getWriter();
			}
			Manager()
			{}
			~Manager()
			{}
		};*/
	}

	// Important variables
	// ------------------------
	/*std::unique_ptr<internal::Manager> log_manager;
	std::unique_ptr<Config> log_config;*/
	//std::unique_ptr<internal::Writer> writer = utils::makeUniquePtr<internal::Writer>();

	class Logger
	{
	public:
		internal::Writer writer;
	};

	class Manager
	{
	private:
		std::map<cstr, std::unique_ptr<Logger>> loggers;
	public:
		internal::Writer& getWriter(cstr logger)
		{
			return loggers[logger]->writer;
		}
	};

	std::unique_ptr<Manager> manager = utils::makeUniquePtr<Manager>();

	// Global functions
	// ------------------------
	/*inline std::pair<bool, cstr> initialize(Config cfg)
	{
		if(!log_manager) {
			log_manager = utils::makeUniquePtr<internal::Manager>();
		}

		return std::make_pair(false, cstr());
	}
	inline void reconfigure(Config cfg)
	{
		log_config.release();
		log_config = utils::makeUniquePtr<Config>(cfg);
	}*/


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
	/*if(el::log_manager)
	{
		return el::log_manager->write(logger_name, level);
	}
	else
	{
		throw std::exception("FATAL: The logging manager has not been initialized.");
	}*/
	return el::manager->getWriter(logger_name);
}