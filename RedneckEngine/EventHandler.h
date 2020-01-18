#pragma once
#include <list>
#include <functional>
#include <utility>
#include <iostream>

template<typename _fn>
class EventHandler;

template<typename RetType, typename... Args>
class EventHandler<RetType(Args...)>
{
	using event_type = std::function<RetType(Args...)>;
	std::list<event_type> m_eventList;
public:
	EventHandler& operator+=(event_type event)
	{
		m_eventList.push_back(event);
		return *this;
	}

	void clear()
	{
		m_eventList.clear();
	}

	RetType Invoke(Args... args)
	{
		if (!m_eventList.empty())
			for (const auto& i : m_eventList)
				i(args...);
	}

	RetType operator()(Args... args)
	{
		Invoke(args...);
	}

	bool operator!=(std::nullptr_t)
	{
		return !m_eventList.empty();
	}

	bool operator==(std::nullptr_t)
	{
		return m_eventList.empty();
	}
};