#include "AssetManager.h"


AssetManager::AssetManager(void)
{

	m_AudioSystem = new AudioSystem();
	if(m_AudioSystem->Init() == 1)
		printf("initialization ok");
	m_AudioSystem->loadMusicTrack(m_AudioSystem->getSystem(), "Sounds/Music/Carmack.mp3", TRACK1);
	m_AudioSystem->loadMusicTrack(m_AudioSystem->getSystem(), "Sounds/Music/Shapeshifter.mp3", TRACK2);
	m_AudioSystem->loadMusicTrack(m_AudioSystem->getSystem(), "Sounds/Music/WarGod.mp3", TRACK3);
	m_AudioSystem->loadMusicTrack(m_AudioSystem->getSystem(), "Sounds/Music/DraggedThroughHellfire.mp3", TRACK4);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/FX72.mp3", TANKAUTOGUN);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/cannon.mp3", TANKCANNON);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/wallcollision.mp3", TANKCOLLISION);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/FX79.mp3", PROJECTILECOLLISION);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/boom3.mp3", PROJECTILEWITHENEMY);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/explosion4.mp3", EXPLOSION);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/tank_movement.mp3", TANKMOVEMENT);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/ricochet.mp3", RICOCHET);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/itempickup.mp3", PICKUP);
	m_AudioSystem->loadSound(m_AudioSystem->getSystem(), "Sounds/FX/charge.mp3", WEAPONCHARGE);
	//m_TextureManager = new TextureManager();
	//m_MeshManager = new MeshManager();
}


AssetManager::~AssetManager(void)
{
}

void AssetManager::updateSubSystems()
{
	m_AudioSystem->Update();
}