//	------------------------------------------------------------------------------
//	MIT LICENSED WORK
//
//	Copyright (c) 2013 Pol Gómez, https://github.com/aszkid
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
//	------------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <mutex>
#include <memory>
#include <chrono>
#include <vector>
#include <ctime>
#include <iomanip>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace el
{
	// Useful little macros and functions
	// ------------------------
	#define PTRME (*this)
	inline std::pair<bool,std::string> make_resp(bool err = false, std::string msg = std::string())
	{
		return std::make_pair(err,msg);
	}
	#define CERR(expr) std::cerr << "[EL_ERROR]: " << expr << std::endl;
	#define COUT(expr) std::cout << "[EL_INFO]: " << expr << std::endl;

	// Important variables
	// ------------------------
	class Config;

	std::unique_ptr<Config> log_config;
	std::mutex mutex;
	std::unique_ptr<std::stringstream> stream;
	

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
			typedef std::chrono::time_point<std::chrono::system_clock>  system_time_point;
		
			std::time_t systemtime_now()
			{
				system_time_point system_now = std::chrono::system_clock::now();
				return std::chrono::system_clock::to_time_t(system_now);
			}
		
			tm localtime(const std::time_t& time)
			{
			std::tm tm_snapshot;
			#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
				localtime_s(&tm_snapshot, &time);
			#else
				localtime_r(&time, &tm_snapshot); // POSIX
			#endif
			return tm_snapshot;
			}
		
			std::string put_time(const std::tm* date_time, const char* c_time_format)
			{
				#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
					std::ostringstream oss;
					oss << std::put_time(const_cast<std::tm*>(date_time), c_time_format);
					return oss.str();
				#else
					const size_t size = 1024;
					char buffer[size]; 
					auto success = std::strftime(buffer, size, c_time_format, date_time); 
					if (0 == success)
						return c_time_format; 
					return buffer; 
				#endif
			}
		
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

		inline void replaceAll(std::string& str, const std::string& from, const std::string& to)
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

		long fileSize(std::ifstream& file)
		{
			auto begin = file.tellg();
			file.seekg(0, std::ios::end);
			auto end = file.tellg();
			file.seekg(begin);
			return (end-begin);
		}

		std::string levelToStr(uint level)
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

		inline bool file_exists(const std::string filename)
		{
			std::ifstream f(filename);
			return (!f ? false : true);
		}

		inline std::pair<std::string,std::string> splitInTwo(const std::string by, const std::string str)
		{
			size_t f;
			std::pair<std::string,std::string> pair;
			if((f = str.find(by)) != std::string::npos)
			{
				pair.first = str.substr(0,f);
				pair.second = str.substr(f+1,std::string::npos);
			}
			return pair;
		}
		
		inline std::string dateTime()
		{
			std::tm lt = time::localtime(time::systemtime_now());
			return time::put_time(&lt, "%Y-%m-%d - %H:%M:%S");
		}
	}
	
	struct ConfigHolder
	{
		ConfigHolder(
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

	class LogConfig
	{
	public:
		ConfigHolder general;
		ConfigHolder warning;
		ConfigHolder error;
		ConfigHolder debug;
		ConfigHolder info;
	};

	// Configuration class
	// ------------------------
	class Config
	{
	public:
		Config(){}

		LogConfig& get(const std::string logger)
		{
			return cfgs[logger];
		}

		std::map<std::string,LogConfig> cfgs;
		boost::property_tree::ptree pt;
	};

	// INTERNAL Namespace - should not be used by the user
	// ------------------------
	namespace internal
	{
		void dispatch(uint level, std::string logger)
		{
			
			ConfigHolder *cfg;

			if(level == GENERAL)
				cfg = &log_config->get(logger).general;
			else if(level == ERROR)
				cfg = &log_config->get(logger).error;
			else if(level == WARNING)
				cfg = &log_config->get(logger).warning;
			else if(level == INFO)
				cfg = &log_config->get(logger).info;
			else if(level == DEBUG)
				cfg = &log_config->get(logger).debug;
			else
			{
				level = GENERAL;
				cfg = &log_config->get(logger).general;
			}
			
			std::string final = cfg->format;
		
			std::vector<std::pair<std::string,std::string>> vars;
			vars.push_back(std::make_pair("%datetime%", utils::dateTime()));
			vars.push_back(std::make_pair("%level%", utils::levelToStr(level)));
			vars.push_back(std::make_pair("%log%", stream->str()));

			for(auto& el : vars)
				utils::replaceAll(final, el.first, el.second);
			
			std::cout << final << std::endl;
			stream->str(std::string());
		}

		// Writer class
		// ------------------------
		class Writer
		{
		private:
			uint level;
			std::string logger_name;

			template<typename T>
			void doauto(const T& s)
			{
				mutex.lock();
					(*stream) << s;
				mutex.unlock();
			}
		public:
			// Operator overloading - basic C/C++ types
			// ---------------------------------------
			Writer& operator<<(const std::string& s) {
				doauto<std::string>(s);
				return PTRME;
			}
			Writer& operator<<(const int& s) {
				doauto<int>(s);
				return PTRME;
			}
			Writer& operator<<(const float& s) {
				doauto<float>(s);
				return PTRME;
			}
			Writer& operator<<(const char& s) {
				doauto<char>(s);
				return PTRME;
			}
			Writer& operator<<(const double& s) {
				doauto<double>(s);
				return PTRME;
			}
			Writer& operator<<(const wchar_t& s) {
				doauto<wchar_t>(s);
				return PTRME;
			}

			// -----------------------
			Writer(std::string _logger_name, uint _level)
				: level(_level), logger_name(_logger_name)
			{}
			~Writer() {
				dispatch(level, logger_name);
			}
		};

		Writer write(std::string logger_name, uint level)
		{
			return Writer(logger_name, level);
		}

		bool ready()
		{
			if(log_config && stream)
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
			return el::make_resp(true, "The logging system is already initialized.");

		log_config = utils::makeUniquePtr<el::Config>();
		stream = utils::makeUniquePtr<std::stringstream>();

		return make_resp(false);
	}
	inline resp addLogConfig(cstr name, LogConfig cfg)
	{
		if(!log_config)
		{
			return make_resp(true, "Could not add log config: the logging system is not initialized.");
		}
		
		return make_resp(false);
	}
	inline void setGlobalConfig(std::string filename)
	{
		if(log_config)
			log_config.release();

		log_config = utils::makeUniquePtr<Config>();

		if(utils::file_exists(filename))
		{
			boost::property_tree::ini_parser::read_ini(filename, log_config->pt);

			for(auto it = log_config->pt.begin(); it != log_config->pt.end(); it++)
			{
				if(it->first == "globalRules")
				{
					// Fetch global info data
				}
				else
				{
					auto p = utils::splitInTwo(":", it->first);
					if(!p.first.empty())
					{
						if(p.second.empty())
							p.second = "GENERAL";
						// Fetch log info data
						bool toFile = boost::optional<bool>(it->second.get_optional<bool>("ToFile")).get_value_or(false);
						std::string fileName = boost::optional<std::string>(it->second.get_optional<std::string>("Filename")).get_value_or("out.log");
						bool toStdOut = boost::optional<bool>(it->second.get_optional<bool>("ToStdOut")).get_value_or(true);
						bool enabled = boost::optional<bool>(it->second.get_optional<bool>("Enabled")).get_value_or(true);
						bool appendToFile = boost::optional<bool>(it->second.get_optional<bool>("AppendToFile")).get_value_or(true);
						std::string format = boost::optional<std::string>(it->second.get_optional<std::string>("Format")).get_value_or("[%datetime%] %level% -> %log%");
						int maxFileSize = boost::optional<int>(it->second.get_optional<int>("MaxFileSize")).get_value_or(2 * 1024 * 1024);

						ConfigHolder c(toFile, toStdOut, enabled, appendToFile, format, maxFileSize);

						if(p.second == "GENERAL")
							log_config->get(p.first).general = c;
						else if(p.second == "ERROR")
							log_config->get(p.first).error = c;
						else if(p.second == "WARNING")
							log_config->get(p.first).warning = c;
						else if(p.second == "INFO")
							log_config->get(p.first).info = c;
						else if(p.second == "DEBUG")
							log_config->get(p.first).debug = c;
						else
							log_config->get(p.first).general = c;
					}
				}
			}
		}
		else
			CERR("File '" << filename << "' does not exist. Skipping configuration.");
	}
}


inline el::internal::Writer LOG(el::uint level = el::Level::General, std::string logger_name = "default")
{
	if(el::internal::ready())
	{
		return el::internal::write(logger_name, level);
	}
	else
	{
		CERR("The logging system is not initialized. Initializing default config.");
		el::initialize();
		return el::internal::write(logger_name, level);
	}
}
