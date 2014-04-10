# pragma once
#include <string>
#include <fmod.hpp>
#include <fmod_errors.h>
enum GameSounds
{
	TRACK1 = 0, TRACK2, TRACK3, TRACK4, TANKAUTOGUN, TANKCANNON,
	TANKCOLLISION, TANKMOVEMENT, PROJECTILECOLLISION, EXPLOSION,
	RICOCHET, PICKUP, PROJECTILEWITHENEMY, WEAPONCHARGE
};

class Sound;
class MusicTrack;

class I_Audio
{
public:
	virtual ~I_Audio(){;}
	virtual void loadSound(FMOD::System* system, const char* filepath, GameSounds refName) =0;
	virtual void loadMusicTrack(FMOD::System* system, const char* filepath, GameSounds refName) =0;
	virtual void setMusicVolume(float volume) =0;
	virtual void setEffectsVolume(float volume) =0;
	virtual FMOD::ChannelGroup* effectChannel() =0;
	virtual FMOD::ChannelGroup* musicChannel() =0;
	virtual Sound& Effect(GameSounds refName) =0;
	virtual MusicTrack& Music(GameSounds refName) =0;
	virtual void Update() =0;
	
};

class Sound
{
public:
	Sound::Sound();
	Sound(FMOD::System* system, const char* filepath, FMOD::ChannelGroup* cGroup);
	Sound& operator=(const Sound& rhs);
	int operator<(const Sound& rhs) const;
	~Sound(void);
	void play();
	void play(float volume);
	FMOD::Channel* playLooping(float volume);
	void Stop();
	FMOD::Channel* getChannel();
	//possibly need stop
private:
	FMOD::Channel* channel;
	FMOD::ChannelGroup* channelGroup;
	FMOD::System* m_System;
	FMOD::Sound* resource;
};

class MusicTrack
{
public:
	MusicTrack::MusicTrack();
	MusicTrack(FMOD::System* system, const char* filepath, FMOD::ChannelGroup* cGroup);
	MusicTrack& operator=(const MusicTrack& rhs);
	int operator<(const MusicTrack& rhs) const;
	~MusicTrack();
	void play();
	void play(float volume);
	FMOD::Channel* Start(bool paused = false);
	void Stop();
	bool TogglePause();
	bool GetPaused();
	void SetPaused(bool pause);
	void SetVolume(float volume);
	// Retrieve the sounds channel
	FMOD::Channel* getChannel();

private:
	FMOD::System* m_System;
	FMOD::Channel* channel;
	FMOD::ChannelGroup* channelGroup;
	FMOD::Sound* resource;
};