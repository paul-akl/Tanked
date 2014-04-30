#include "ParticleSystemFactory.h"
#include "TransformNode.h"
#include <sstream>
ParticleSystemFactory::ParticleSystemFactory(void)
{

}
void ParticleSystemFactory::init()
{
	//load up all pointsprite texture nodes
	//create MAX SYSTEMS particle systems
	for (int i = 0; i < MAX_SYSTEMS; i ++)
	{
		ParticleSystem* PSys = new ParticleSystem();
		TransformNode* ts = new TransformNode();
		ts->activate();
		ts->reset();
		PSys->addTransform(ts);
		PSys->setBoundingRadius(15.0f);
		PSys->setScale(glm::vec3(15.0f));
		PSys->setMaxParticles(MAX_PARTICLES_PER_SYSTEM);
		m_ParticleSystems.push_front(PSys);
	}
	//compute beginning velocities for all particle effect types
	for (int i = 0; i < MAX_PARTICLE_TYPES; i ++)
	{
		switch(i){
		case SPARK:
			{
				for(int j = 0; j < MAX_PARTICLES_PER_SYSTEM; j++)
				{
					//generate 3 floats between -1 and 1
					float tx, ty, tz;
					tx = ((std::rand() % 100)/50.0f)-1.0f;
					ty = ((std::rand() % 100)/50.0f)-1.0f;
					tz = ((std::rand() % 100)/50.0f)-1.0f;
					glm::vec3 v(tx,ty,tz);
					m_BaseVelocities[i].push_back(glm::normalize(v));
				}
			}
			break;
		case WAVE:
			{
				for(int j = 0; j < MAX_PARTICLES_PER_SYSTEM; j++)
				{
					//generate 3 floats between -1 and 1
					float tx, ty, tz;
					tx = ((std::rand() % 100)/50.0f)-1.0f;
					//As we are computing a wave effect, the particles should move mostly parallel to the xz plane,
					//so we reduce its range to 10% of normal
					ty = (((std::rand() % 100)/50.0f)-1.0f)*0.10f;
					tz = ((std::rand() % 100)/50.0f)-1.0f;
					glm::vec3 v(tx,ty,tz);
					m_BaseVelocities[i].push_back(glm::normalize(v));
				}
			}
			break;
		case EXPLOSION:
			{
				for(int j = 0; j < MAX_PARTICLES_PER_SYSTEM; j++)
				{
					//generate 3 floats between -1 and 1
					float tx, ty, tz;
					tx = ((std::rand() % 100)/50.0f)-1.0f;
					ty = ((std::rand() % 100)/50.0f)-1.0f;
					tz = ((std::rand() % 100)/50.0f)-1.0f;
					glm::vec3 v(tx,ty,tz);
					m_BaseVelocities[i].push_back(glm::normalize(v));
				}
			}
			break;
		case BOX_EXPLOSION:
			{
				for(int j = 0; j < MAX_PARTICLES_PER_SYSTEM; j++)
				{
					//generate 3 floats between -1 and 1
					float tx, ty, tz;
					tx = ((std::rand() % 100)/50.0f)-1.0f;
					ty = ((std::rand() % 100)/50.0f)-1.0f;
					tz = ((std::rand() % 100)/50.0f)-1.0f;
					glm::vec3 v(tx,ty,tz);
					//not normalised, we want this in the shape of a box
					m_BaseVelocities[i].push_back(v);
				}
			}
			break;
		};
	}
	m_SparkCount=0;
	m_WaveCount=0;
	m_ExpCount=0;
	m_BoxExpCount=0;
}
ParticleSystem* ParticleSystemFactory::getParticleSystem(ParticleType p_Type)
{
	//get a generic particle system
	ParticleSystem* tmp = getInstanceFromPool();
	//if valid(effect) 
	if(tmp!=nullptr)
	{
		//	switch (p_Type)
		switch(p_Type)
		{
		case SPARK:
			{
				//		init spark
				//set per type data
				initSparkEffect(tmp);
				//		set unique data
				m_SparkCount++;
				//			set name (type + count)
				std::stringstream ss;
				ss<<"Spark"<<m_SparkCount<<std::endl;
				tmp->setName(ss.str());
				//		activate
				tmp->activate();
			}break;
		case WAVE:
			{
				//		init spark
				initWaveEffect(tmp);
				//		set unique data
				m_WaveCount++;
				//			set name (type + count)
				std::stringstream ss;
				ss<<"Wave"<<m_WaveCount<<std::endl;
				tmp->setName(ss.str());
				//		activate
				tmp->activate();
			}break;
		case EXPLOSION:
			{
				//		init spark
				initExplosionEffect(tmp);
				//		set unique data
				m_ExpCount++;
				//			set name (type + count)
				std::stringstream ss;
				ss<<"Explosion"<<m_ExpCount<<std::endl;
				tmp->setName(ss.str());
				//		activate
				tmp->activate();
			}break;
		case BOX_EXPLOSION:
			{
				//		init spark
				initBoxExplosionEffect(tmp);
				m_BoxExpCount++;
				//			set name (type + count)
				std::stringstream ss;
				ss<<"BoxExplosion"<<m_BoxExpCount<<std::endl;
				tmp->setName(ss.str());
				//		activate
				tmp->activate();
			}break;
			//	end switch
		}
	}
	//end if
	//	return effect
	return tmp;
}
ParticleSystem* ParticleSystemFactory::getInstanceFromPool()
{
	//look for a particle system that isn't in use
	for (std::list<ParticleSystem*>::iterator it = m_ParticleSystems.begin();it!=m_ParticleSystems.end();it++)
	{
		if(!(*it)->isActive())
		{
			(*it)->reset();
			return (*it);
		}
	}
	return nullptr;
}
void ParticleSystemFactory::initSparkEffect(ParticleSystem* p_ParticleSystem)
{
	//copy per type data into instance
	if(p_ParticleSystem->m_ParticleType != SPARK)
	{
		p_ParticleSystem->m_BaseVelocities.clear();
		for(size_t i = 0; i < MAX_PARTICLES_PER_SYSTEM; i++)
		{
			p_ParticleSystem->m_BaseVelocities.push_back(m_BaseVelocities[SPARK][i]);
		}
		p_ParticleSystem->m_ParticleType = SPARK;
		// start at yellowish white and proceed to red.
		p_ParticleSystem->setColourTransition(glm::vec4(1.0f,1.0f,0.8f,1.0f),glm::vec4(1.0f,0.0f,0.0f,0.0f));
	}
	else
	{
		p_ParticleSystem->reset();
		p_ParticleSystem->resetForces();
	}
	p_ParticleSystem->setRepeatingMode(false);
}
void ParticleSystemFactory::initExplosionEffect(ParticleSystem* p_ParticleSystem)
{
	//copy per type data into instance
	if(p_ParticleSystem->m_ParticleType != EXPLOSION)
	{
		p_ParticleSystem->m_BaseVelocities.clear();
		for(size_t i = 0; i < MAX_PARTICLES_PER_SYSTEM; i++)
		{
			p_ParticleSystem->m_BaseVelocities.push_back(m_BaseVelocities[EXPLOSION][i]);
		}
		p_ParticleSystem->m_ParticleType = EXPLOSION;
		// start at yellowish white and proceed to red.
		p_ParticleSystem->setColourTransition(glm::vec4(1.0f,0.85f,0.0f,1.0f),glm::vec4(0.58f,0.35f,0.0f,0.0f));
	}
	else
	{
		p_ParticleSystem->reset();
		p_ParticleSystem->resetForces();
	}
	p_ParticleSystem->setRepeatingMode(false);
}
void ParticleSystemFactory::initBoxExplosionEffect(ParticleSystem* p_ParticleSystem)
{
	//copy per type data into instance
	if(p_ParticleSystem->m_ParticleType != BOX_EXPLOSION)
	{
		p_ParticleSystem->m_BaseVelocities.clear();
		for(size_t i = 0; i < MAX_PARTICLES_PER_SYSTEM; i++)
		{
			p_ParticleSystem->m_BaseVelocities.push_back(m_BaseVelocities[BOX_EXPLOSION][i]);
		}
		p_ParticleSystem->m_ParticleType = BOX_EXPLOSION;
		// start at yellowish white and proceed to red.
		p_ParticleSystem->setColourTransition(glm::vec4(1.0f,0.85f,0.0f,1.0f),glm::vec4(0.58f,0.35f,0.0f,0.0f));
	}
	else
	{
		p_ParticleSystem->reset();
		p_ParticleSystem->resetForces();
	}
	p_ParticleSystem->setRepeatingMode(false);
}
void ParticleSystemFactory::initWaveEffect(ParticleSystem* p_ParticleSystem)
{
	//copy per type data into instance
	if(p_ParticleSystem->m_ParticleType != WAVE)
	{
		p_ParticleSystem->m_BaseVelocities.clear();
		for(size_t i = 0; i < MAX_PARTICLES_PER_SYSTEM; i++)
		{
			p_ParticleSystem->m_BaseVelocities.push_back(m_BaseVelocities[WAVE][i]);
		}
		p_ParticleSystem->m_ParticleType = WAVE;
		// start at yellowish white and proceed to red.
		p_ParticleSystem->setColourTransition(glm::vec4(1.0f,0.85f,0.0f,1.0f),glm::vec4(0.58f,0.35f,0.0f,0.0f));
	}
	else
	{
		p_ParticleSystem->reset();
		p_ParticleSystem->resetForces();
	}
	p_ParticleSystem->setRepeatingMode(true);
	
}
ParticleSystemFactory::~ParticleSystemFactory(void)
{

}
