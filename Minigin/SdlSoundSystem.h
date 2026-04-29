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

	class SdlSoundSystem : public SoundSystem
	{
	public:
		SDL_AudioDeviceID m_pDevice = 0;
		MIX_Mixer* m_pMixer = nullptr;

		void Init() override;

		~SdlSoundSystem();

		void Play(const SoundId id, const float volume) override;
		void AddSound(SoundId id, std::string path) override;

		enum class Sound
		{
			Coin,
			Fall,
			Jump1,
			Jump2,
			Jump3,
			Jump4
		};

	private:
		std::unordered_map<SoundId, std::unique_ptr<AudioClip>> m_AudioClips{};

		const float m_MinVolume{ 0.f };
		const float m_MaxVolume{ 100.f };
	};
}