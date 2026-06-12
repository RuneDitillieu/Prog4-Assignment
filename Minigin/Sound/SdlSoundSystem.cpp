#include "SdlSoundSystem.h"
#include <stdexcept>
#include <cassert>

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

    bool playResult = MIX_PlayTrack(m_pTrack, false);
    assert(playResult || "Failed to play sound");

    // counts as not using variable in release
    playResult;
}


// SOUND SYSTEM

void dae::SdlSoundSystem::Init()
{
    bool initResult = SDL_Init(SDL_INIT_AUDIO);
    assert(initResult || "Couldn't initialize SDL audio");

    initResult = MIX_Init();
    assert(initResult || "Couldn't initialize MIX");

    SDL_AudioSpec spec{};
    spec.freq = 48000;
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;

    m_pDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    assert(m_pDevice != 0 || "Failed to open audio device");

    m_pMixer = MIX_CreateMixerDevice(m_pDevice, &spec);
    assert(m_pMixer || "Failed to create mixer device");

    m_thread = std::jthread{ &SdlSoundSystem::AudioThreadLoop, this, m_stopToken };
}

dae::SdlSoundSystem::~SdlSoundSystem()
{
    // stop thread
    std::unique_lock<std::mutex> lock{ m_mutex };
    m_stopSource.request_stop();
    m_conditionVar.notify_all();
    lock.unlock();

    // de-initialize
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

void dae::SdlSoundSystem::Play(const SoundId id, const float volume)
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    m_soundQueue.emplace(id, volume);
    m_conditionVar.notify_all();
}

void dae::SdlSoundSystem::AddSound(SoundId id, std::string path, float volume)
{
    m_AudioClips[id] = std::make_unique<AudioClip>(path, volume);
}

void dae::SdlSoundSystem::MuteUnmuteSound()
{
    m_isMuted = !m_isMuted;
}

void dae::SdlSoundSystem::AudioThreadLoop(std::stop_token stopToken)
{
    while (!stopToken.stop_requested())
    {
        // lock thread
        std::unique_lock<std::mutex> lock{ m_mutex };

        while (!stopToken.stop_requested() && m_soundQueue.empty())
        {
            m_conditionVar.wait(lock);
        }
        while (!m_soundQueue.empty())
        {
            // grab sound request from the queue
            auto [id, volume] { m_soundQueue.front() };
            m_soundQueue.pop();
            lock.unlock();

            // play/load sound
            if (m_isMuted) return;

            assert(m_AudioClips.contains(id) || "Invalid SoundId");

            if (!m_AudioClips[id]->IsLoaded())
            {
                m_AudioClips[id]->Load(m_pMixer);
            }

            m_AudioClips[id]->Play(volume);

            lock.lock();
        }

        // extra unlock for safety
        // would sometimes not unlock upon quitting the program
        lock.unlock();
    }
}

