#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
	: _last(steady_clock::now())
{
}

float Timer::Mark()
{
	auto _current = steady_clock::now();
	const duration<float> _frametime = _current - _last;
	_last = _current;
	return _frametime.count();
}

float Timer::Peek() const
{
	return duration<float>(steady_clock::now() - _last).count();

}
