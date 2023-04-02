#ifndef TIMER_H
#define TIMER_H

#include <SDL_timer.h>

class Timer 
{
public:
	Timer():IsPause(true), DeltaSeconds(0), TotalSeconds(0), CurrTick(0), EndTick(0) { };
	~Timer() = default;

	void Start()
	{
		IsPause = false;
		CurrTick = SDL_GetPerformanceCounter();
		EndTick = SDL_GetPerformanceCounter();
	}
	void Tick()	
	{
		if(IsPause)
			return;
		CurrTick = SDL_GetPerformanceCounter();
		DeltaSeconds = (float)(CurrTick - EndTick) / (float)SDL_GetPerformanceFrequency();;
		TotalSeconds += DeltaSeconds;
		EndTick = CurrTick;
	}
	void Pause()
	{
		IsPause = true;	
	}
	
	inline float GetDeltaSeconds() const { return DeltaSeconds; };
	inline float GetFramesPerSecond() const { return 1 / DeltaSeconds; };
	inline float GetTotalSeconds() const { return TotalSeconds; };
	inline bool IsPaused() const { return IsPause; };

private:
	bool IsPause;
	float DeltaSeconds;
	float TotalSeconds;
	uint64_t CurrTick;
	uint64_t EndTick;
	
};

#endif
