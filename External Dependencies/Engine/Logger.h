#ifndef LOGGER_H
#define LOGGER_H
#include "ExportHeader.h"
#include "MyTypeDefs.h"

enum Severity {Warning, Error, Info, Severe};



#define USE_LOGGER

#ifdef USE_LOGGER
class ENGINE_SHARED Logger
{
	static Logger instance;
	Logger();
	Logger(const Logger&);
	Logger& operator =(const Logger&);
public:
	~Logger();
	static Logger& getInstance() {return instance;}
	static void Log(const char* message, const char* fileName, int lineNumber,Severity level);
};

#define LOGI(msg) Logger::Log(msg,__FILE__,__LINE__,Info);
#define LOGS(msg) Logger::Log(msg,__FILE__,__LINE__,Severe);
#define LOGE(msg) Logger::Log(msg,__FILE__,__LINE__,Error);
#define LOGW(msg) Logger::Log(msg,__FILE__,__LINE__,Warning);
#else


class Logger
{
	static Logger instance;
	Logger() {}
	Logger(const Logger&);
	Logger& operator =(const Logger&);
public:
	static void init(char* fileName){}
	static void shutdown(){}
	static void Log(const char* message, const char* fileName, int lineNumber,Severity level){}
};

#define LOGI(msg) 
#define LOGS(msg) 
#define LOGE(msg) 
#define LOGW(msg) 
#endif

#endif