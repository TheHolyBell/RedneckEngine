#pragma once

template<typename T>
class Singleton
{
protected:
	Singleton() {}
	virtual ~Singleton() {}
	
	Singleton(const Singleton& rhs) = delete;
	Singleton& operator=(const Singleton& rhs) = delete;

	Singleton(Singleton&& rhs) = delete;
	Singleton& operator=(Singleton&& rhs) = delete;

public:
	T& Get()
	{
		static T _instance;
		return _instance;
	}
};