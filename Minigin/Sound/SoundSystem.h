#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <string>

namespace dae
{
	using SoundId = unsigned short;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Init() = 0;
		virtual void Play(const SoundId id, const float volume = -1.f) = 0;
		virtual void AddSound(SoundId id, std::string path, float defaultVolume) = 0;
		virtual void MuteUnmuteSound() = 0;
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		void Play(const SoundId, const float) override {}
		void AddSound(SoundId, std::string, float) override {}
		void Init() override {}
		void MuteUnmuteSound() override {}
	};
}

#endif // !SOUNDSYSTEM_H