#include "AudioSystem.h"
#include <assert.h>

bool AudioSystem::m_Instantiated = false;

AudioSystem::AudioSystem(void)
{
	assert(!m_Instantiated);
	m_Instantiated = true;
	m_BGMChannel = nullptr;
}


AudioSystem::~AudioSystem(void)
{
}

int AudioSystem::Init()
{
	unsigned int v_Version;
	int v_NumDrivers;
	FMOD_SPEAKERMODE v_SpeakerMode;
	FMOD_CAPS v_Caps;
	char name[256];

	// create the FMOD system interface
	m_Result = FMOD::System_Create(&m_System);
	ErrorCheck(m_Result);

	// check libraries and DLL versions match
	m_Result = m_System->getVersion(&v_Version);
	ErrorCheck(m_Result);

	if (v_Version < FMOD_VERSION)
	{
		std::cout << "Error - FMOD version incompatible. current version: " << v_Version << ". \nRequired version: " << FMOD_VERSION << std::endl;
		return 0;
	}

	// check system for sound cards
	m_Result = m_System->getNumDrivers(&v_NumDrivers);
	ErrorCheck(m_Result);

	// if sound cards not detected disable sound output
	if(v_NumDrivers < 1)
	{
		m_Result = m_System->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ErrorCheck(m_Result);
	}
	// if a sound card is detected
	else
	{
		// get the default sound cards capabilities
		m_Result = m_System->getDriverCaps(0, &v_Caps, 0, &v_SpeakerMode);
		ErrorCheck(m_Result);

		// set the speaker mode to match the default detected in the control panel
		m_Result = m_System->setSpeakerMode(v_SpeakerMode);
		ErrorCheck(m_Result);

		// if hardware acceleraion is disabled, increase the software buffer size
		if(v_Caps &FMOD_CAPS_HARDWARE_EMULATED)
		{
			m_Result = m_System->setDSPBufferSize(1024, 20);
			ErrorCheck(m_Result);
		}

		// query sound card information
		m_Result = m_System->getDriverInfo(0, name, 256, 0);
		ErrorCheck(m_Result);
		// apparently SigmaTel brand sound devices crackle if the format is PCM 16-bit
		// so it is changed to PCM Float
		if(strstr(name, "SigmaTel"))
		{
			m_Result = m_System->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			ErrorCheck(m_Result);
		}
	}
	
		// Initialize FMOD system
		m_Result = m_System->init(200, FMOD_INIT_NORMAL, 0);

		// if speaker mode is not supported by the sound card switch it to stereo
		if(m_Result == FMOD_ERR_OUTPUT_CREATEBUFFER)
		{
			m_Result = m_System->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
			ErrorCheck(m_Result);

			m_Result = m_System->init(200, FMOD_INIT_NORMAL, 0);
		}
		ErrorCheck(m_Result);
		
		
	// setup master channels
	m_System->createChannelGroup(NULL, &m_EffectsChannel);
	m_System->createChannelGroup(NULL, &m_MusicChannel);

		

		return 1;
}


void AudioSystem::ErrorCheck(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		std::cout << "FMOD Error (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		std::cin.get();
		exit(-1);
	}
}

void AudioSystem::loadSound(FMOD::System* system, const char* filepath, GameSounds refName)
{
	Sound* s =  new Sound(system, filepath, m_EffectsChannel);
	effect[refName] = s;// Sound(filepath, m_EffectsChannel);
	
}

void AudioSystem::loadMusicTrack(FMOD::System* system, const char* filepath, GameSounds refName)
{
	MusicTrack* mt =  new MusicTrack(system, filepath, m_MusicChannel);
	music[refName] = mt;
}

void AudioSystem::setEffectsVolume(float volume)
{
	m_EffectsChannel->setVolume(volume);
}

void AudioSystem::setMusicVolume(float volume)
{
	m_MusicChannel->setVolume(volume);
}

void AudioSystem::Update()
{
	m_System->update();
}

	
////////////////////////////////// Sound Class /////////////////////////////////////////

Sound::Sound()
{
	channel = nullptr;
	channelGroup = nullptr;
	m_System = nullptr;
	resource = nullptr;
}

Sound::Sound(FMOD::System* system, const char* filepath, FMOD::ChannelGroup* cGroup)
{
	m_System = system;
	FMOD::Sound *s;
	m_System->createSound(filepath, FMOD_SOFTWARE, 0, &s);
	resource = s;
	// set channel group
	channelGroup = cGroup;
	channel = nullptr;
}

Sound& Sound::operator=(const Sound& rhs)
{
	this->channelGroup = rhs.channelGroup;
	this->m_System = rhs.m_System;
	this->resource = rhs.resource;
	return *this;
}

int Sound::operator<(const Sound& rhs) const
{
	return 0;
}

Sound::~Sound()
{
	channelGroup = nullptr;
	m_System = nullptr;
	resource->release();
	resource = nullptr;
}

void Sound::play()
{
	FMOD::Channel *channel;

	m_System->playSound(FMOD_CHANNEL_FREE, resource, true, &channel);

	// Assign channel group
	channel->setChannelGroup(channelGroup);

	channel->setPaused(false);
}

void Sound::play(float volume)
{
	FMOD::Channel *channel;

	m_System->playSound(FMOD_CHANNEL_FREE, resource, true, &channel);

	channel->setVolume(volume);

	// Assign channel group
	channel->setChannelGroup(channelGroup);

	channel->setPaused(false);
}

///////////////////////// return type to be channel*
FMOD::Channel* Sound::playLooping(float volume)
{
	//FMOD::Channel* channel;
	// may have to use the sound channel
	m_System->playSound(FMOD_CHANNEL_FREE, resource, true, &channel);
	// Assign channel group
	channel->setChannelGroup(channelGroup);

	channel->setMode(FMOD_LOOP_NORMAL);
	channel->setLoopCount(-1);
	
	channel->setVolume(volume);

	channel->setPaused(false);
	return channel;
}

void Sound::Stop()
{
	channel->stop();
	channel = NULL;
}

FMOD::Channel *Sound::getChannel()
{
	return channel;
}


////////////////////////// MusicTrack Class //////////////////////////////////


MusicTrack::MusicTrack()
{
	channelGroup = nullptr;
	m_System = nullptr;
	resource = nullptr;
	channel = nullptr;
}

MusicTrack::MusicTrack(FMOD::System* system, const char* filepath, FMOD::ChannelGroup* cGroup)
{
	m_System = system;
	FMOD::Sound *s;
	m_System->createSound(filepath, FMOD_SOFTWARE, 0, &s);
	resource = s;
	
	// set channel group
	channelGroup = cGroup;
	channel = nullptr;
	
}

MusicTrack& MusicTrack::operator=(const MusicTrack& rhs)
{
	this->channelGroup = rhs.channelGroup;
	this->channel =rhs.channel;
	this->m_System = rhs.m_System;
	this->resource = rhs.resource;
	return *this;
}

int MusicTrack::operator<(const MusicTrack& rhs) const
{
	return 0;
}

MusicTrack::~MusicTrack()
{
	channelGroup = nullptr;
	m_System = nullptr;
	resource->release();
	resource = nullptr;
	channel = nullptr;
}

void MusicTrack::play(float volume)
{
	FMOD_RESULT result;
		result = m_System->playSound(FMOD_CHANNEL_FREE, resource, true, &channel);
			if(result != FMOD_OK)
	{
		std::cout << "FMOD Error (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		std::cin.get();
		exit(-1);
	}
			
		channel->setVolume(volume);

		// Assign channel group 
		channel->setChannelGroup(channelGroup);

		// set looping
		channel->setLoopCount(-1);
		channel->setMode(FMOD_LOOP_NORMAL);

		// Flush buffer to ensure loop logic is executed
		channel->setPosition(0, FMOD_TIMEUNIT_MS);

		// Set paused or not as applicable
		//if (!paused)
			channel->setPaused(false);
}

FMOD::Channel* MusicTrack::Start(bool paused)
	{
		
		FMOD_RESULT result;
		result = m_System->playSound(FMOD_CHANNEL_FREE, resource, true, &channel);
			if(result != FMOD_OK)
	{
		std::cout << "FMOD Error (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		std::cin.get();
		exit(-1);
	}
			
		// Assign channel group 
		channel->setChannelGroup(channelGroup);

		// set looping
		channel->setLoopCount(-1);
		channel->setMode(FMOD_LOOP_NORMAL);

		// Flush buffer to ensure loop logic is executed
		channel->setPosition(0, FMOD_TIMEUNIT_MS);

		// Set paused or not as applicable
		if (!paused)
			channel->setPaused(paused);

		return channel;
	}

// Stop music and free the channel
void MusicTrack::Stop()
{
	channel->stop();
	channel = NULL;
}

// Get the channel being used
FMOD::Channel *MusicTrack::getChannel()
{
	return channel;
}

// Pause/unpause
bool MusicTrack::TogglePause()
{
	bool isPaused;
	channel->getPaused(&isPaused);
	channel->setPaused(!isPaused);
	return !isPaused;
}

bool MusicTrack::GetPaused()
{
	bool paused;
	channel->getPaused(&paused);
	return paused;
}

void MusicTrack::SetPaused(bool paused)
{
	channel->setPaused(paused);
}

void MusicTrack::SetVolume(float volume)
{
	channel->setVolume(volume);
}
