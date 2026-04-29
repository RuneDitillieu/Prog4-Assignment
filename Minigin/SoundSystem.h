#pragma once

namespace dae
{
	using SoundId = unsigned short;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void play(const SoundId id, const float volume) = 0;
	};
}