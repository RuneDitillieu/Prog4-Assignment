#include "SdlSoundSystem.h"
#include <algorithm>
#include <stdexcept>

// AUDIO CLIPS

dae::AudioClip::AudioClip(std::string path, float volume)
    : m_Path(path), m_Volume(volume)
{ }

void dae::AudioClip::Load(MIX_Mixer* mixer)
{
    if (m_IsLoaded)
        return;

    m_pAudio = MIX_LoadAudio(mixer, m_Path.c_str(), 0);
    m_pTrack = MIX_CreateTrack(mixer);

    MIX_SetTrackAudio(m_pTrack, m_pAudio);

    m_IsLoaded = true;
}

void dae::AudioClip::Play(float volume)
{
    if (!m_pTrack || !m_pAudio)
        return;

    if (volume < 0)
        volume = m_Volume;

    MIX_SetTrackGain(m_pTrack, volume);

    if (!MIX_PlayTrack(m_pTrack, false))
        throw std::runtime_error(SDL_GetError());
}


// SOUND SYSTEM

void dae::SdlSoundSystem::Init()
{
    if (!SDL_Init(SDL_INIT_AUDIO))
        throw std::runtime_error(SDL_GetError());
    if (!MIX_Init())
        throw std::runtime_error(SDL_GetError());

    SDL_AudioSpec spec{};
    spec.freq = 48000;
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;

    m_pDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);

    if (m_pDevice == 0)
        throw std::runtime_error(SDL_GetError());

    m_pMixer = MIX_CreateMixerDevice(m_pDevice, &spec);

    if (!m_pMixer)
        throw std::runtime_error(SDL_GetError());
}

dae::SdlSoundSystem::~SdlSoundSystem()
{
    if (m_pMixer)
    {
        m_pMixer = nullptr;
    }

    if (m_pDevice != 0)
    {
        SDL_CloseAudioDevice(m_pDevice);
        m_pDevice = 0;
    }
}

void dae::SdlSoundSystem::Play(const dae::SoundId id, const float volume)
{
    if (m_isMuted) return;

    if (!m_AudioClips.contains(id))
    {
        throw std::runtime_error("Invalid sound_id");
    }

    if (!m_AudioClips[id]->IsLoaded())
    {
        m_AudioClips[id]->Load(m_pMixer);
    }

    m_AudioClips[id]->Play(volume);
}

void dae::SdlSoundSystem::AddSound(SoundId id, std::string path, float volume)
{
    m_AudioClips[id] = std::make_unique<AudioClip>(path, volume);
}

void dae::SdlSoundSystem::MuteUnmuteSound()
{
    m_isMuted = !m_isMuted;
}


