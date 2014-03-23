#include "AudioSystem.h"
#include <assert.h>

AudioSystem::AudioSystem(void)
{
	assert(!m_Instantiated);
	m_Instantiated = true;
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
	if(v_NumDrivers == 0)
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
			m_Result = m_System->setDSPBufferSize(1024, 10);
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
		m_Result = m_System->init(100, FMOD_INIT_NORMAL, 0);

		// if speaker mode is not supported by the sound card switch it to stereo
		if(m_Result == FMOD_ERR_OUTPUT_CREATEBUFFER)
		{
			m_Result = m_System->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
			ErrorCheck(m_Result);

			m_Result = m_System->init(100, FMOD_INIT_NORMAL, 0);
		}
		ErrorCheck(m_Result);
		
		InitializeSounds();
		return 1;
}

void AudioSystem::InitializeSounds()
{
	m_System->createChannelGroup(NULL, &m_EffectsChannel);
	m_System->createChannelGroup(NULL, &m_MusicChannel);

	m_System->createSound("Sounds/ambience01.wav", FMOD_DEFAULT, 0, &m_Sound.sound);
	ErrorCheck(m_System->playSound(FMOD_CHANNEL_FREE, m_Sound.sound, true, &m_SoundChannel));
	m_Sound.channel->setChannelGroup(m_MusicChannel);
	m_Sound.isEffect = false;	
	m_Sound.looping = false;
	m_SoundList.push_back(m_Sound);		
		
	m_System->createSound("Sounds/boom8.wav", FMOD_DEFAULT, 0, &m_Sound.sound);
	m_Sound.isEffect = true;
	m_Sound.looping = false;
	m_SoundList.push_back(m_Sound);

	m_System->createSound("Sounds/boom4.wav", FMOD_DEFAULT, 0, &m_Sound.sound);
	m_Sound.isEffect = true;
	m_Sound.looping = false;
	m_SoundList.push_back(m_Sound);

	m_System->createSound("Sounds/tank_movement.wav", FMOD_DEFAULT, 0, &m_Sound.sound);
	m_Sound.isEffect = true;
	m_Sound.looping = false;
	m_SoundList.push_back(m_Sound);

	
}

// each sound needs to be in a struct
//bool playing
//FMOD::Channel
//bool isEffect
//FMOD::Sound
void AudioSystem::PlaySound(Sound sound)
{
	
	
	// Start sound paused to obtain channel then assign sound to channel group
	if (sound.looping == true)	
	{
		//set looping
		ErrorCheck(m_System->playSound(FMOD_CHANNEL_FREE, sound.sound, true, &sound.channel));
		sound.channel->setMode(FMOD_LOOP_NORMAL);
		sound.channel->setLoopCount(-1);
		sound.channel->setPaused(false);
		
	
	}
	
	// Assign channel to channel group
	if(sound.isEffect == true)
	{
		if(sound.looping == false){
		ErrorCheck(m_System->playSound(FMOD_CHANNEL_FREE, sound.sound, true, &sound.channel));
		sound.channel->setChannelGroup(m_EffectsChannel);
		//if(sound.looping == true)
		//{
			//ErrorCheck(m_System->playSound(FMOD_CHANNEL_FREE, sound.sound, false, &sound.channel));
			//sound.channel->setMode(FMOD_LOOP_NORMAL);
			//sound.channel->setLoopCount(-1);
		//}
		}
		sound.channel->setPaused(false);
		
	}	
	
	
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

void AudioSystem::LoadSound(const char fileName, FMOD::Sound * nsound)
{
	m_System->createSound(&fileName, FMOD_DEFAULT, 0, &nsound);
}

void AudioSystem::Update()
{
	m_System->update();
}

	