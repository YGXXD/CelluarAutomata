#ifndef TIMER_H
#define TIMER_H

#include <SDL_timer.h>

class Timer 
{
public:
	Timer():IsPause(true), DeltaSeconds(0), SecondsPerCount(1.0 / (double)SDL_GetPerformanceFrequency()), BaseTick(0), StopTick(0), PauseTick(0), CurrTick(0), PrevTick(0) { };
	~Timer() = default;

	void Reset()
	{
		IsPause = false;
		PauseTick = 0;
		BaseTick = SDL_GetPerformanceCounter();
		PrevTick = BaseTick;
	}

	void Start()
	{
		if(IsPause)
		{
			uint64_t StartTick = SDL_GetPerformanceCounter();

			StopTick += StartTick - PauseTick;
			PrevTick = StartTick;
			PauseTick = 0;
			IsPause = false;
		}
	}

	void Tick()	
	{
		if(IsPause)
		{
			DeltaSeconds = 0;
			return;
		}
		CurrTick = SDL_GetPerformanceCounter();
		DeltaSeconds = (float)(CurrTick - PrevTick) * SecondsPerCount;
		PrevTick = CurrTick;

		if(DeltaSeconds < 0)
		{
			DeltaSeconds = 0;
		}
	}

	void Pause()
	{
		if(!IsPause)
		{
			IsPause = true;	
			PauseTick = SDL_GetPerformanceCounter(); 
		}
	}
	
	inline float GetDeltaSeconds() const { return DeltaSeconds; }
	inline float GetFramesPerSecond() const { return 1 / DeltaSeconds; }
	inline float GetTotalSeconds() const 
	{
	   	return IsPause ? (PauseTick - StopTick - BaseTick) * SecondsPerCount : (CurrTick - StopTick - BaseTick) * SecondsPerCount;
	}
	inline bool IsPaused() const { return IsPause; }

private:
	bool IsPause;
	double DeltaSeconds;
	double SecondsPerCount;

	uint64_t BaseTick;
	uint64_t StopTick;
	uint64_t PauseTick;
	uint64_t CurrTick;
	uint64_t PrevTick;
	
};

#endif
