#pragma once
#include "Singleton.h"
#include <SDL3/SDL.h>

namespace dae
{
	class DeltaTime final : public Singleton<DeltaTime>
	{
	public:
		void CalculateDeltaTime()
		{
			//static uint64_t previousTime{ SDL_GetPerformanceCounter() };
			const uint64_t currentTime{ SDL_GetPerformanceCounter() };
			m_deltaTime = static_cast<float>(currentTime - m_prevTime) / static_cast<float>(SDL_GetPerformanceFrequency());
			m_prevTime = currentTime;
		}
		float GetDeltaTime() { return m_deltaTime; }

	private:
		float m_deltaTime{};
		uint64_t m_prevTime{ SDL_GetPerformanceCounter() };
	};
}