#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <sstream>
#include <string>
#include <mutex>
#include <memory>
#include <chrono>

#define TIME_AS_MILLI(bench) float(bench.get<std::chrono::milliseconds>().count())
#define TIME_AS_SEC(bench) TIME_AS_MILLI(bench)/1000.0f
#define TIME_AS_MIN(bench) TIME_AS_SEC(bench)/60.0f
#define TIME_AS_MICRO(bench) float(bench.get<std::chrono::microseconds>().count())

class Benchmark
{
private:
	std::chrono::system_clock::time_point t0;
	std::chrono::system_clock::time_point t1;
public:
	void start()
	{
		t0 = std::chrono::high_resolution_clock::now();
	}
	void stop()
	{
		t1 = std::chrono::high_resolution_clock::now();
	}
	void restart()
	{stop();start();}
			
	template<typename t>
	t get()
	{
		return std::chrono::duration_cast<t>(t1 - t0);
	}

	template<typename Functor>
	void lambda(Functor func)
	{
		start();
		func();
		stop();
	}
};

namespace el
{
	// Useful little macros
	// ------------------------
	#define PTRME (*this)

	// Macros for easy access to levels
	// ------------------------
	#define GENERAL Level::General
	#define ERROR Level::Error
	#define WARNING Level::Warning
	#define DEBUG Level::Debug
	#define INFO Level::Info

	// Important variables
	// ------------------------
	class Config;

	std::unique_ptr<Config> log_config;

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
		long fileSize(std::ifstream& file)
		{
			long begin = file.tellg();
			file.seekg(0, std::ios::end);
			long end = file.tellg();
			file.seekg(begin);
			return (end-begin);
		}

		cstr levelToStr(uint level)
		{
			switch(level)
			{
			case GENERAL:
				return "GENERAL";
			case ERROR:
				return "ERROR";
			case WARNING:
				return "WARNING";
			case DEBUG:
				return "DEBUG";
			case INFO:
				return "INFO";
			default:
				return "GENERAL";
			}
		}

		class NonCopyable
		{
		protected:
			NonCopyable(void) {}
		private:
			NonCopyable(const NonCopyable&);
			NonCopyable& operator=(const NonCopyable&);
		};

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
	}
	
	class LogConfig
	{
	public:
		LogConfig(
			bool _toFile = false, 
			bool _toStdOut = true,
			bool _enabled = true,
			bool _appendToFile = true,
			std::string _format = "[%datetime%] %level% -> %log%",
			long _maxFileSize = 2 * (1024^2) // 2 Megabytes (in bytes)
		)
		{}

		bool toFile;
		bool toStdOut;
		bool enabled;
		bool appendToFile;
		std::string format;
		long maxFileSize;
	};

	// Configuration class
	// ------------------------
	class Config
	{
	public:
		Config(){}
		std::map<cstr,LogConfig> cfgs;
	};

	// INTERNAL Namespace - should not be used by the user
	// ------------------------
	namespace internal
	{
		void dispatch(std::string str, uint level)
		{
			std::cout << "[" << utils::levelToStr(level) << "] " << str << std::endl;
		}

		// Writer class
		// ------------------------
		class Writer : private utils::NonCopyable
		{
		private:
			std::stringstream stream;
			uint level;
			cstr logger_name;
		public:
			// Operator overloading
			// ------------------------
			Writer& operator<<(const std::string& s) {
				stream << s;
				return PTRME;
			}
			Writer& operator<<(const int& i) {
				stream << i;
				return PTRME;
			}
			// -----------------------
			Writer(cstr _logger_name, uint _level)
				: logger_name(_logger_name), level(_level)
			{}
			virtual ~Writer() {
				dispatch(stream.str(), level);
			}
		};

		Writer write(cstr logger_name, uint level)
		{
			return Writer(logger_name, level);
		}

		bool ready()
		{
			return true;
		}
	}

	// Global functions
	// ------------------------
	inline std::pair<bool, cstr> initialize()
	{
		if(log_config)
			log_config.release();

		log_config = utils::makeUniquePtr<Config>();

		return std::make_pair(false, cstr());
	}
	inline void addLogConfig(LogConfig logcfg)
	{

	}
	inline void setGlobalConfig(cstr filename)
	{
		if(utils::file_exists(filename)) {
		
		} else {
			
		}
	}
}


inline el::internal::Writer LOG(el::uint level = el::Level::General, el::cstr logger_name = "default")
{
	if(el::internal::ready())
	{
		return el::internal::write(logger_name, level);
	}
	else
	{
		throw std::exception("FATAL: The logging manager has not been initialized.");
	}
}