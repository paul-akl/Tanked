#pragma once
#include "ParticleSystem.h"
#include <list>
class ParticleSystemFactory
{
public:
	ParticleSystemFactory(void);
	void init();
	ParticleSystem* getParticleSystem(ParticleType p_Type);
	~ParticleSystemFactory(void);
private:
	//initialisation functions & data
	static const int MAX_SYSTEMS = 128;
	static const int MAX_PARTICLES_PER_SYSTEM = 512;
	static const int MAX_PARTICLE_TYPES = 4;
	int m_SparkCount;
	int m_WaveCount;
	int m_ExpCount;
	int m_BoxExpCount;
	void initSparkEffect(ParticleSystem* p_ParticleSystem);
	void initExplosionEffect(ParticleSystem* p_ParticleSystem);
	void initBoxExplosionEffect(ParticleSystem* p_ParticleSystem);
	void initWaveEffect(ParticleSystem* p_ParticleSystem);
	ParticleSystem* getInstanceFromPool();
	std::list<ParticleSystem*> m_ParticleSystems;
	//default settings
	std::vector<std::vector<glm::vec3>> m_BaseVelocities;
	//one for each particle type
	std::vector<TextureNode*> m_DiffuseTextures;
	std::vector<TextureNode*> m_EmissiveTextures;
};

