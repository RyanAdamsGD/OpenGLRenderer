#include "Logger.h"
#include <fstream>

static std::ofstream outFile;

Logger Logger::instance;
#if defined USE_LOGGER

Logger::Logger()
{
	outFile.open("log.html");
	if(!outFile.good())
		return;
	outFile<<"<html>\n"<<"<body>\n"<<"<ul>\n"<<std::endl;
	outFile.close();
}

Logger::~Logger()
{
	outFile.open("log.html",std::ios::app | std::ios::out);
	outFile<<"</ul>\n"<<"</body>\n"<<"</html>";
	if(!outFile.good())
		return;
	outFile.close();
}

void Logger::Log(const char* message, const char* fileName, int lineNumber,Severity level)
{
	outFile.open("log.html",std::ios::app | std::ios::out);
	if(!outFile.good())
		return;
	outFile<< "<li style=\"background-color: ";
		switch(level)
		{
		case Severe:
			outFile<<"red";
			break;
		case Error:
			outFile<<"red";
			break;
		case Info:
			outFile<<"white";
			break;
		case Warning:
			outFile<<"yellow";
			break;
		}
	outFile<<"\">"<< message<<" "<<fileName <<": "<< lineNumber << "</li>\n" << std::endl;
	outFile.close();
}
#endif
