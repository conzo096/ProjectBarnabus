#pragma once
#include <iostream>

class StringLog
{
public:
	enum Priority
	{
		AllLogs,
		Low,
		Medium,
		Highest,
		Critical,
		NoLogs
	};

public:
	StringLog() = delete;
	StringLog(std::string info, Priority prior);

public:
	Priority GetPriority();
	std::string GetMessage();
private:
	Priority priority;
	std::string message;
};