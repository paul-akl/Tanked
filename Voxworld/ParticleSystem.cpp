#include "ParticleSystem.h"
#include "DeferredRenderer.h"
#include "TransformNode.h"

ParticleSystem::ParticleSystem(void)
{
	m_BufferSet = false;
}
void ParticleSystem::init()
{
	if(!m_BufferSet)
	{
		m_ParticleBuffers = new GLuint[4];
		glGenVertexArrays(1, &m_ParticleHandle);
		glBindVertexArray(m_ParticleHandle);
		//now we populate the VAO with Vertex Buffer data, just as if we were creating a mesh, except we are using points.
		glGenBuffers(1, &m_ParticleBuffers[PARTICLE_POSITION]);
		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[PARTICLE_POSITION]);
		//as this data can change dynamically, we specify this to opengl. Unfortunately, storage size cannot be dynamic, 
		//so we create the storage at maximum size, regardless of how many particles we actually have.
		glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(glm::vec3), m_Positions.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)PARTICLE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(PARTICLE_POSITION);

		glGenBuffers(1, &m_ParticleBuffers[PARTICLE_VELOCITY]);
		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[PARTICLE_VELOCITY]);
		//as this data can change dynamically, we specify this to opengl. Unfortunately, storage size cannot be dynamic, 
		//so we create the storage at maximum size, regardless of how many particles we actually have.
		glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(glm::vec3), m_Velocities.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)PARTICLE_VELOCITY, 4, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(PARTICLE_VELOCITY);


		//this VBO is used for a simple compute shader, or for cpu processing. undecided yet
		glGenBuffers(1, &m_ParticleBuffers[PARTICLE_TTL]);
		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[PARTICLE_TTL]);
		//as this data can change dynamically, we specify this to opengl. Unfortunately, storage size cannot be dynamic, 
		//so we create the storage at maximum size, regardless of how many particles we actually have.
		glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(GLfloat), m_LifeTimes.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)PARTICLE_TTL, 1, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(PARTICLE_TTL);
		//now we are finished creating space on the GPU, so we can unbind the VBO and VAO
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//initialise particles & opengl buffers
	}
	else
		updateBuffers();
}
void ParticleSystem::updateParticles(float p_DeltaTimeS)
{
	glm::vec3 netForce(0.0f);
	for (int j = 0; j < m_Forces.size();j++)
	{
		netForce+=m_Forces[j];
	}
	for (size_t i = 0; i < m_MaxParticles; i++)
	{
		m_LifeTimes[i]-=p_DeltaTimeS;

		if(m_LifeTimes[i]>0.0f)
		{
			m_Velocities[i]+=netForce*p_DeltaTimeS;
			m_Positions[i]+=m_Velocities[i]*p_DeltaTimeS;
		}
		else
		{
			if(m_Repeating)
			{
				m_Positions[i] = glm::vec3(0.0f);
				m_Velocities[i] = m_BaseVelocities[i];
				m_LifeTimes[i] = m_BaseLifeTime;
			}
		}
	}
}
void ParticleSystem::release()
{

	glDeleteBuffers(4,m_ParticleBuffers);
	glDeleteVertexArrays(1,&m_ParticleHandle);
	delete[] m_ParticleBuffers;
	m_BufferSet = false;
}
void ParticleSystem::reset()
{
	std::fill(m_Positions.begin(),m_Positions.begin(),glm::vec3(0.0f));
	for (size_t i = 0; i < m_BaseVelocities.size();i++)
	{
		m_Velocities[i] = m_BaseVelocities[i];
	}
	std::fill(m_LifeTimes.begin(),m_LifeTimes.end(),m_BaseLifeTime);
}
void ParticleSystem::updateBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER,m_ParticleBuffers[PARTICLE_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(glm::vec3), m_Positions.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,m_ParticleBuffers[PARTICLE_VELOCITY]);
	glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(glm::vec3), m_Velocities.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[PARTICLE_TTL]);
	glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(GLfloat), m_LifeTimes.data(), GL_DYNAMIC_DRAW);
	glBindVertexArray(0);
}
void ParticleSystem::setColourTransition(glm::vec4 p_StartColour,glm::vec4 p_EndColour)
{
	m_DeltaColour = (m_EndColour - m_StartingColour);
	m_StartingColour = p_StartColour;
	m_EndColour = p_EndColour;
}
void ParticleSystem::update(float p_DeltaTimeS)
{
	//update opengl buffers
	//update vertex buffer,
	//update lifetime buffer,
	//STUFF FOR RENDERER
	//update MaxLifeTime uniform
	//update deltaColour uniform
	//update deltaTime uniform
	//update point size uniform
	//END STUFF FOR RENDERER
	if(m_LocalTransform!=nullptr)
	{
		m_LocalTransform->reset();
		m_LocalTransform->translate(m_Position+glm::vec3(0.0f,m_Altitude,0.0f));
		m_LocalTransform->scale(m_Scale);
		m_LocalTransform->update(p_DeltaTimeS);
	}
	updateParticles(p_DeltaTimeS);
	updateBuffers();
}
void ParticleSystem::render(Renderer* p_Renderer)
{
	p_Renderer->begin();
	p_Renderer->render(this);
	if(m_LocalTransform!=nullptr)
		m_LocalTransform->render(p_Renderer);
	if(m_Diffuse!=nullptr)
		m_Diffuse->render(p_Renderer);
	//m_Emissive->Render(p_Renderer);
	p_Renderer->end();
}

ParticleSystem::~ParticleSystem(void)
{
	glDeleteBuffers(4,m_ParticleBuffers);
	delete[] m_ParticleBuffers;
	//clear up opengl buffers & vectors
}
