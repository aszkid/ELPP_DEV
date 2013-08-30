#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <mutex>
#include <memory>
#include <chrono>

namespace el
{
	// Useful little macros and functions
	// ------------------------
	#define PTRME (*this)
	inline std::pair<bool,std::string> make_resp(bool err = false, std::string msg = std::string())
	{
		return std::make_pair(err,msg);
	}
	#define CERR(expr) std::cerr << expr << std::endl;
	#define COUT(expr) std::cout << expr << std::endl;

	// Important variables
	// ------------------------
	class Config;

	std::unique_ptr<Config> log_config;

	// Type conventions
	// ------------------------
	typedef unsigned int uint;
	typedef char* cstr;
	typedef std::pair<bool, std::string> resp;

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

	// Macros for easy access to levels
	// ------------------------
	const uint GENERAL = Level::General;
	const uint ERROR = Level::Error;
	const uint WARNING = Level::Warning;
	const uint DEBUG = Level::Debug;
	const uint INFO = Level::Info;

	// UTILS Namespace - helper functions
	// ------------------------
	namespace utils
	{
		namespace time
		{
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

			inline float asMicroseconds(Benchmark& bench) { return bench.get<std::chrono::microseconds>().count(); }
			inline float asMilliseconds(Benchmark& bench) { return bench.get<std::chrono::milliseconds>().count(); }
			inline float asSeconds(Benchmark& bench) { return (asMilliseconds(bench)) / 1000.0f; }
			inline float asMinutes(Benchmark& bench) { return (asSeconds(bench)) / 60.0f; }
		}

		long fileSize(std::ifstream& file)
		{
			auto begin = file.tellg();
			file.seekg(0, std::ios::end);
			auto end = file.tellg();
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
		: toFile(_toFile), toStdOut(_toStdOut),
		enabled(_enabled), appendToFile(_appendToFile),
		format(_format), maxFileSize(_maxFileSize)
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
		class Writer : public utils::NonCopyable
		{
		private:
			std::mutex m;
			std::stringstream stream;
			uint level;
			cstr logger_name;

			template<typename T>
			void doauto(const T& s)
			{
				m.lock();
					stream << s;
				m.unlock();
			}
		public:
			// Operator overloading
			// ------------------------
			Writer& operator<<(const std::string& s) {
				doauto(s);
				return PTRME;
			}
			Writer& operator<<(const int& s) {
				doauto(s);
				return PTRME;
			}
			Writer& operator<<(const float& s) {
				doauto(s);
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
			if(log_config)
				return true;
			else
				return false;
		}
	}

	// Global functions
	// ------------------------
	inline resp initialize()
	{
		if(log_config)
			return make_resp(true, "The logging system is already initialized.");

		log_config = utils::makeUniquePtr<Config>();

		return make_resp(false);
	}
	inline resp addLogConfig(cstr name, LogConfig cfg)
	{
		if(log_config)
		{
		}
		else
		{
			return make_resp(true, "Could not add log config: the logging system is not initialized.");
		}
	}
	inline void setGlobalConfig(cstr filename)
	{
		if(utils::file_exists(filename)) {
			COUT("File " << filename << " does exist.");
		} else {
			COUT("File " << filename << " does not exist.");
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
		CERR("[LE_ERROR]: The logging system is not initialized. Initializing default config.");
		el::initialize();
		return el::internal::write(logger_name, level);
	}
}