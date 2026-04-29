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
			static uint64_t prevTime{ SDL_GetPerformanceCounter() };
			const uint64_t currentTime{ SDL_GetPerformanceCounter() };
			m_deltaTime = static_cast<float>(currentTime - prevTime) / static_cast<float>(SDL_GetPerformanceFrequency());
			prevTime = currentTime;
		}
		float GetDeltaTime() { return m_deltaTime; }

	private:
		float m_deltaTime;
	};
}