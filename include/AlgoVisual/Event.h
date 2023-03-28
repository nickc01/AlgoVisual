#pragma once
#include <map>
#include <functional>


template<typename R, typename... Args>
class Event {

	unsigned long idCounter = 0;

	std::map<unsigned long, std::function<R(Args...)>> functions;
public:

	unsigned long add(std::function<R(Args...)>&& func)
	{
		auto id = ++idCounter;
		functions.emplace(id,std::move(func));
		return id;
	}

	bool remove(unsigned long id)
	{
		if (functions.find(id) != functions.end())
		{
			functions.erase(id);
			return true;
		}
		return false;
	}

	void call(Args... args)
	{
		for (auto& fn : functions)
		{
			std::get<1>(fn)(args...);
		}
	}
};