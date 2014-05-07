#include "ParticleSystem.h"
#include "DeferredRenderer.h"
#include "TransformNode.h"

ParticleSystem::ParticleSystem(void)
{
	m_BufferSet = false;
	m_BaseLifeTime = 0.0f;
	m_NumWaves = 1;
	m_ParticleType = NOEFFECT;
}
void ParticleSystem::init()
{
	m_Colours.resize(m_MaxParticles,glm::vec4(0.0f));
	if(!m_BufferSet)
	{
		m_ParticleBuffers = new GLuint[NUM_PARTICLE_BUFFERS];
		glGenVertexArrays(1, &m_ParticleHandle);
		glBindVertexArray(m_ParticleHandle);
		//now we populate the VAO with Vertex Buffer data, just as if we were creating a mesh, except we are using points.
		glGenBuffers(1, &m_ParticleBuffers[PARTICLE_POSITION]);
		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[PARTICLE_POSITION]);
		//as this data can change dynamically, we specify this to opengl. Unfortunately, storage size cannot be dynamic, 
		//so we create the storage at maximum size, regardless of how many particles we actually have.
		glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(glm::vec3), m_Positions.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)PARTICLE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(PARTICLE_POSITION);

		//populate the colour vbo
		glGenBuffers(1, &m_ParticleBuffers[PARTICLE_COLOUR]);
		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[PARTICLE_COLOUR]);
		//as this data can change dynamically, we specify this to opengl. Unfortunately, storage size cannot be dynamic, 
		//so we create the storage at maximum size, regardless of how many particles we actually have.
		glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(glm::vec4), m_Colours.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)PARTICLE_COLOUR, 4, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(PARTICLE_COLOUR);


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
		m_BufferSet = true;
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
	for (size_t i = 0; i < m_Positions.size(); i++)
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
				m_CurrentColour = m_StartingColour;
			}
			else
			{
				deactivate();
				reset();
				return;
			}
		}
	}
}
void ParticleSystem::interpolateColor(float p_DeltaTimeS)
{
	m_CurrentColour += m_DeltaColour * p_DeltaTimeS;
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
	m_Velocities = m_BaseVelocities;
	m_CurrentColour = m_StartingColour;
	std::fill(m_LifeTimes.begin(),m_LifeTimes.end(),m_BaseLifeTime);
	std::fill(m_Positions.begin(),m_Positions.end(),glm::vec3(0.0f));
	updateBuffers();
}
void ParticleSystem::updateBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[PARTICLE_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(glm::vec3), m_Positions.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[PARTICLE_COLOUR]);
	glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(glm::vec4), m_Colours.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[PARTICLE_TTL]);
	glBufferData(GL_ARRAY_BUFFER, m_MaxParticles*sizeof(GLfloat), m_LifeTimes.data(), GL_DYNAMIC_DRAW);
	glBindVertexArray(0);
}
void ParticleSystem::setColourTransition(glm::vec4 p_StartColour,glm::vec4 p_EndColour)
{
	m_StartingColour = p_StartColour;
	m_CurrentColour = p_StartColour;
	m_EndColour = p_EndColour;
	m_DeltaColour = (m_EndColour - m_StartingColour) * (1.0f / m_BaseLifeTime);
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
		if(m_Parent!=nullptr)
			m_LocalTransform->rotate(-m_Parent->getOrientation(),glm::vec3(0.0f,1.0f,0.0f));
		m_LocalTransform->scale(m_Scale);
		m_LocalTransform->update(p_DeltaTimeS);
	}
	SceneNode::update(p_DeltaTimeS);
	updateParticles(p_DeltaTimeS);
	interpolateColor(p_DeltaTimeS);
	updateBuffers();
}
void ParticleSystem::render(Renderer* p_Renderer)
{
	p_Renderer->begin();
	if(m_LocalTransform!=nullptr)
		m_LocalTransform->render(p_Renderer);
	p_Renderer->render(this);

	/*if(m_Diffuse!=nullptr)
		m_Diffuse->render(p_Renderer);*/
	//m_Emissive->Render(p_Renderer);
	//p_Renderer->end();
}
void ParticleSystem::setVectorBias(glm::vec3& p_Bias)
{
	size_t end = m_Velocities.size();
	for(size_t i = 0; i < end;i++)
	{
		m_Velocities[i]+=p_Bias;
	}
}
ParticleSystem::~ParticleSystem(void)
{
	release();
	//clear up opengl buffers & vectors
}
