#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <vector>
enum MAX_CHANNELS {CHANNELS = 10};

enum GameSounds
{
	BackgroundMusic = 0,
	PrimaryFire,
	SecondaryFire,
	TankMovement

};

struct Sound
{
	FMOD::Sound* sound;
	FMOD::Channel * channel;
	bool isEffect;
	bool looping;
	bool isLooping;
};
class AudioSystem
{
public:
	AudioSystem(void);
	~AudioSystem(void);
	int Init();
	void PlaySound(Sound sound);
	Sound GetSound(GameSounds i) { return m_SoundList[i];}
	void setLooping(GameSounds sound, bool loop){m_SoundList[sound].looping == loop;}
	void Update();

private:
	static bool m_Instantiated;
	void SetupChannels();
	void ErrorCheck(FMOD_RESULT result);
	void InitializeSounds();
	void LoadSound(const char fileName, FMOD::Sound * nsound);
	bool playing;
	//FMOD::Sound *sound;
	FMOD::System * m_System;
	FMOD_RESULT m_Result;
	FMOD::Channel * m_BGMChannel;
	FMOD::Channel * m_SoundChannel;
	FMOD::ChannelGroup * m_MusicChannel;
	FMOD::ChannelGroup * m_EffectsChannel;
	FMOD::Channel* m_Channels[CHANNELS];
	Sound m_Sound;
	std::vector<Sound> m_SoundList;
};

bool AudioSystem::m_Instantiated = false;