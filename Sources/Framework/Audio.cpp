#include "Audio.h"

#include "Resources.h"
#include "Debug.h"

namespace
{
    /**
     * @brief 
     */
    ALCdevice* device  = nullptr;

    /**
     * @brief 
     */
    ALCcontext* context = nullptr;
}

void AudioSystem::Initialize()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        Logger::Error("OpenAL: Failed to open default device.");
        throw std::runtime_error("OpenAL: Failed to open default device.");
    }

    context = alcCreateContext(device, nullptr);
    if (!context || !alcMakeContextCurrent(context))
    {
        Logger::Error("OpenAL: Failed to create/make current context.");
        throw std::runtime_error("OpenAL: Failed to create/make current context.");
    }

    Logger::Info("Audio System Initialized (OpenAL).");
}

void AudioSystem::Quit()
{
    if (context)
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        context = nullptr;
    }
    if (device)
    {
        alcCloseDevice(device);
        device = nullptr;
    }
}

AudioSource::AudioSource(Object* const owner) noexcept
    : Component(owner)
    , playOnAwake(false)
    , isLooping(false)
    , volume(1.0f)
    , pitch(1.0f)
{
    alGenSources(1, &sourceID);
    alSourcef(sourceID, AL_PITCH, pitch);
    alSourcef(sourceID, AL_GAIN, volume);
    alSource3f(sourceID, AL_POSITION, 0, 0, 0);
    alSource3f(sourceID, AL_VELOCITY, 0, 0, 0);
    alSourcei(sourceID, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);
}

AudioSource::~AudioSource() noexcept
{
    OnDestroy();
}

void AudioSource::Awake()
{
    if (playOnAwake)
    {
        Play();
    }
}

void AudioSource::OnDestroy()
{
    Stop();
    if (sourceID != 0)
    {
        alDeleteSources(1, &sourceID);
        sourceID = 0;
    }
}

void AudioSource::Update()
{
    if (sourceID == 0)
    {
        return;
    }

    const glm::vec3 position = transform->GetPosition();
    alSource3f(sourceID, AL_POSITION, position.x, position.y, position.z);
}

void AudioSource::Play()
{
    if (sourceID != 0 && currentClip)
    {
        alSourcePlay(sourceID);
    }
}

void AudioSource::Stop()
{
    if (sourceID != 0)
    {
        alSourceStop(sourceID);
    }
}

void AudioSource::Pause()
{
    if (sourceID != 0)
    {
        alSourcePause(sourceID);
    }
}

AudioListener::AudioListener(Object* const owner) noexcept
    : Component(owner)
{
}

AudioListener::~AudioListener() noexcept
{
}

void AudioListener::Update()
{
    if (!IsEnabled())
    {
        return;
    }

    const glm::vec3 position = transform->GetPosition();
    const glm::vec3 forward  = transform->GetForward();
    const glm::vec3 up       = transform->GetUp();

    alListener3f(AL_POSITION, position.x, position.y, position.z);

    const float orientation[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
    alListenerfv(AL_ORIENTATION, orientation);
}