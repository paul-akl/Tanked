#pragma once
#include "scenenode.h"


//////////////////////////////////////////////////////////////////////
// A generic Particle System class for creating particle effects.	//
// Designed to be set up by a factory/manager, or extended 			//
// which will set up initial particle states						//
// example use:														//
//	shader->setLifeTimeUniform(system->getMaxLifeTime());			//
//	shader->setDeltaColourUniform(system->getDeltaColor());			//
//	shader->setDeltaTimeUniform(system->getDT());					//
//	shader->setLifeTimeUniform(system->getMaxLifeTime());			//
//	glPointSize(system->getPointSize());							//
//	glDepthMask(0);													//
//	glEnable(GL_BLEND);												//
//	glBindVertexArray(system->getParticleHandle());					//
//	glDrawArrays(GL_POINTS, 0, system->getNumParticles());			//
//	glBindVertexArray(0);											//
//	glDisable(GL_BLEND);											//
//	glDepthMask(1);													//
//////////////////////////////////////////////////////////////////////


enum ParticleType
{
	SPARK,
	WAVE,
	EXPLOSION,
	BOX_EXPLOSION,
	HOVER,
	NUM_PARTICLE_TYPES
};
enum ParticleBufferSet
{
	PARTICLE_POSITION,
	PARTICLE_VELOCITY,
	PARTICLE_COLOUR,
	PARTICLE_TTL,	// Time To Live
	NUM_PARTICLE_BUFFERS
};
class ParticleSystem :
	public SceneNode
{
public:
	ParticleSystem(void);
	void init();
	virtual void update(float p_DeltaTimeS);
	virtual void render(Renderer* p_Renderer);
	void setPointSize(int p_PointSize){m_PointSize = p_PointSize;}
	void setVectorBias(glm::vec3& p_Bias);
	void setParticlesPerEmmission(int p_Number){m_ParticlesPerEmission = p_Number;}
	void setRepeatingMode(bool p_Switch){m_Repeating = p_Switch;}
	int getPointSize(){return m_PointSize;}
	void setMaxParticles(int p_Max){m_MaxParticles = p_Max;}
	void AddForceVector(glm::vec3 p_Force){m_Forces.push_back(p_Force);}
	void resetForces(){m_Forces.clear();}
	void setAltitude(float p_Alt){m_Altitude=p_Alt;}
	void setMaxLifeTime(float p_TTL){m_BaseLifeTime=p_TTL;}
	float getMaxLifeTime(){return m_BaseLifeTime;}
	void setColourTransition(glm::vec4 p_StartColour,glm::vec4 p_EndColour);
	GLuint getParticleHandle(){return m_ParticleHandle;}
	int getNumParticles(){return m_MaxParticles;}
	glm::vec4 getCurrentColour() { return m_CurrentColour; }
	void release();
	void reset();
	virtual ~ParticleSystem(void);
protected:
	friend class ParticleSystemFactory;

	void updateBuffers();
	void updateParticles(float p_DeltaTimeS);
	void interpolateColor(float p_DeltaTimeS);

	std::vector<float> m_LifeTimes;
	ParticleType m_ParticleType;
	std::vector<glm::vec3>	m_Positions,
							m_BaseVelocities,
							m_Velocities,
							m_Forces;
	float	m_BaseLifeTime,
			m_Altitude;
	int		m_ParticlesPerEmission,
			m_PointSize,
			m_MaxParticles;
	bool	m_Repeating,
			m_GradiantColour,
			m_BufferSet;
	glm::vec4	m_StartingColour,
				m_EndColour,
				m_DeltaColour,
				m_CurrentColour;
	glm::vec3	m_ForceVector;
	GLuint		*m_ParticleBuffers,
				m_ParticleHandle;
};

