#pragma once
#include "SoundSystem.h"
#include "SDL3_mixer/SDL_mixer.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace dae
{
	struct AudioClip
	{
	public:
		AudioClip(std::string path);
		~AudioClip() = default;

		void Load(MIX_Mixer* mixer);
		void Play(float volume);

		bool IsLoaded() const { return m_IsLoaded; }

	private:
		MIX_Audio* m_pAudio = nullptr;
		MIX_Track* m_pTrack = nullptr;

		float m_Volume = 10.f;

		std::string m_Path;
		bool m_IsLoaded = false;
	};

	class SdlSoundSystem final : public SoundSystem
	{
	public:

		void Init() override;

		~SdlSoundSystem();

		void Play(const SoundId id, const float volume) override;
		void AddSound(SoundId id, std::string path) override;

	private:
		std::unordered_map<SoundId, std::unique_ptr<AudioClip>> m_AudioClips{};

		SDL_AudioDeviceID m_pDevice = 0;
		MIX_Mixer* m_pMixer = nullptr;
	};
}