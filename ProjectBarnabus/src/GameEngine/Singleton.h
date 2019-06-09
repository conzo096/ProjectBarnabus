#pragma once

#include <memory>

// Singleton is so boiler plate we can define a template
// for it.

template<typename T>
class Singleton
{
public:
	// This call needs to be inline and static
	inline static T& Get()
	{
		 static std::unique_ptr<T> instance(new T());
		// Dereference the pointer to get the data.
		 return *instance.get();
	}
};