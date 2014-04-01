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
};
enum ParticleBufferSet
{
	PARTICLE_POSITION,
	PARTICLE_VELOCITY,
	PARTICLE_COLOUR,
	PARTICLE_TTL,
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
	void release();
	void reset();
	virtual ~ParticleSystem(void);
protected:
	void updateBuffers();
	void updateParticles(float p_DeltaTimeS);
	std::vector<glm::vec3> m_Positions;
	float m_Altitude;
	glm::vec3 m_ForceVector;
	std::vector<glm::vec3> m_BaseVelocities;
	std::vector<glm::vec3> m_Velocities;
	std::vector<glm::vec3> m_Forces;
	std::vector<float> m_LifeTimes;
	float m_BaseLifeTime;
	int m_ParticlesPerEmission;
	int m_PointSize;
	int m_MaxParticles;
	bool m_Repeating;
	bool m_GradiantColour;
	friend class ParticleSystemFactory;
	ParticleType m_ParticleType;
	glm::vec4 m_StartingColour;
	glm::vec4 m_EndColour;
	glm::vec4 m_DeltaColour;
	GLuint* m_ParticleBuffers;
	GLuint m_ParticleHandle;
	bool m_BufferSet;
};

