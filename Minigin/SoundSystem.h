#pragma once
#include <string>

namespace dae
{
	using SoundId = unsigned short;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Init() = 0;
		virtual void Play(const SoundId id, const float volume) = 0;
		virtual void AddSound(SoundId id, std::string path) = 0;
	};

	class NullSoundSystem final : public SoundSystem
	{
		void Play(const SoundId, const float) override {}
		void AddSound(SoundId, std::string) {}
		void Init() override {}
	};
}