#include "CollisionResponse.h"
#include <iostream>
#include "TestTankNode.h"
CollisionResponse::CollisionResponse(void)
{
}

void CollisionResponse::processResult(CollisionPair* p_Pair)
{
	switch(p_Pair->getResultType())
	{
	case PLAYERVSSCENERY:
		{
			if(p_Pair->m_Collidable_A->getType() == PLAYER)
			{
				glm::vec3 normal = glm::vec3(p_Pair->m_CollisionNormal.x,0.0f,p_Pair->m_CollisionNormal.z);
				glm::vec3 velocity = p_Pair->m_Collidable_A->getVelocity();
				glm::vec3 reflection = glm::reflect(velocity,normal);
				p_Pair->m_Collidable_A->setPosition(p_Pair->m_Collidable_A->getLocation()+normal*(p_Pair->m_Penetration));
				p_Pair->m_Collidable_A->stop();
				p_Pair->m_Collidable_A->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);
				std::cout<<"PLAYERvsSCENERY"<<std::endl;
			}
			else
			{
				glm::vec3 normal = glm::vec3(p_Pair->m_CollisionNormal.x,0.0f,p_Pair->m_CollisionNormal.z);
				glm::vec3 reflection = glm::reflect(p_Pair->m_Collidable_B->getVelocity(),glm::normalize(-normal))*0.8f;
				p_Pair->m_Collidable_B->setPosition(p_Pair->m_Collidable_B->getLocation()+normal*(p_Pair->m_Penetration));
				p_Pair->m_Collidable_B->stop();
				p_Pair->m_Collidable_B->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);
				std::cout<<"SCENERYvsPlayer"<<std::endl;

			}
		}break;
	case PLAYERVSCOLLECTABLE:
		{
			if(p_Pair->m_Collidable_A->getType()==COLLECTABLE)
			{
				if(p_Pair->m_Collidable_A->isActive())
				{
					//p_Pair->m_Collidable_A->deactivate();
					TestTankNode* tank = (TestTankNode*)p_Pair->m_Collidable_B;
					tank->AddOffensiveUpgrade((OffensiveUpgrade*)p_Pair->m_Collidable_A);
					//p_Pair->m_Collidable_B->stop();
					std::cout<<"PLAYERvsCollectable"<<std::endl;
				}
			}
			else
			{
				if(p_Pair->m_Collidable_B->isActive())
				{
					//p_Pair->m_Collidable_A->deactivate();
					TestTankNode* tank = (TestTankNode*)p_Pair->m_Collidable_A;
					tank->AddOffensiveUpgrade((OffensiveUpgrade*)p_Pair->m_Collidable_B);
					//p_Pair->m_Collidable_B->stop();
					std::cout<<"CollectablevsPlayer"<<std::endl;
				}
			}
		}break;
	}
	
}
CollisionResponse::~CollisionResponse(void)
{
}
