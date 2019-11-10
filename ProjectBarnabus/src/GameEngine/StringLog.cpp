#include "StringLog.h"

StringLog::StringLog(std::string info, Priority prior)
{
	message = info;
	priority = prior;
}

std::string StringLog::GetMessage()
{
	return message;
}

StringLog::Priority StringLog::GetPriority()
{
	return priority;
}