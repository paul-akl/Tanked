#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include "I_Audio.h"

class Sound;
class MusicTrack;

class AudioSystem : public I_Audio
{
public:
	AudioSystem(void);
	~AudioSystem(void);
	int Init();
	FMOD::System* getSystem(){return m_System;}
	virtual void loadSound(FMOD::System* system, const char* filepath, GameSounds refName);
	virtual void loadMusicTrack(FMOD::System* system, const char* filepath, GameSounds refName);
	virtual void setMusicVolume(float volume);
	virtual void setEffectsVolume(float volume);
	virtual FMOD::ChannelGroup* effectChannel(){return m_EffectsChannel;}
	virtual FMOD::ChannelGroup* musicChannel(){return m_MusicChannel;}
	virtual Sound& Effect(GameSounds refName){return *effect[refName];}
	virtual MusicTrack& Music(GameSounds refName){return *music[refName];}
	
	virtual void Update();

private:
	static bool m_Instantiated;
	void SetupChannels();
	void ErrorCheck(FMOD_RESULT result);
	void InitializeSounds();
	bool playing;
	FMOD::System * m_System;
	FMOD_RESULT m_Result;
	FMOD::Channel * m_BGMChannel;
	FMOD::Channel * m_SoundChannel;
	FMOD::ChannelGroup * m_MusicChannel;
	FMOD::ChannelGroup * m_EffectsChannel;
	std::vector<Sound> m_SoundList;
	std::vector<MusicTrack> m_MusicList;
	std::unordered_map<GameSounds,Sound*> effect;
	std::unordered_map<GameSounds,MusicTrack*> music;
	
};