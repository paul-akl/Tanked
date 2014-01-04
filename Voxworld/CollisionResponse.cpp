#include "CollisionResponse.h"
#include <iostream>
#include "TestTankNode.h"
#include "ProjectileNode.h"
CollisionResponse::CollisionResponse(void)
{
}

void CollisionResponse::processResult(CollisionPair* p_Pair)
{
	// simple collision event handling system
	// handles collisions between player controlled objects and scenery (walls, for the moment, but could include more)
	if(p_Pair->hasCollided())
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
					if(p_Pair->m_Collidable_A->isMoving())
					{
						//p_Pair->m_Collidable_A->setPosition(p_Pair->m_Collidable_A->getLocation()-glm::normalize(p_Pair->m_Collidable_A->getVelocity())*(p_Pair->m_Penetration));
						p_Pair->m_Collidable_A->stop();
						p_Pair->m_Collidable_A->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);
					}//std::cout<<"PLAYERvsSCENERY"<<std::endl;
				}
				else
				{
					glm::vec3 normal = glm::vec3(p_Pair->m_CollisionNormal.x,0.0f,p_Pair->m_CollisionNormal.z);
					glm::vec3 reflection = glm::reflect(p_Pair->m_Collidable_B->getVelocity(),glm::normalize(-normal))*0.8f;
					if(p_Pair->m_Collidable_B->isMoving())
					{
						//p_Pair->m_Collidable_B->setPosition(p_Pair->m_Collidable_B->getLocation()-glm::normalize(p_Pair->m_Collidable_B->getVelocity())*(p_Pair->m_Penetration));
						p_Pair->m_Collidable_B->stop();
						p_Pair->m_Collidable_B->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);
					}
					//std::cout<<"SCENERYvsPLAYER"<<std::endl;

				}
			}break;
		// handles collision between player controlled objects and collectable objects, which can be stored in player controlled objects
		case PLAYERVSCOLLECTABLE:
			{
				if(p_Pair->m_Collidable_A->getType()==COLLECTABLE)
				{
					if(p_Pair->m_Collidable_A->isActive())
					{
						TestTankNode* tank = (TestTankNode*)p_Pair->m_Collidable_B;
						tank->AddOffensiveUpgrade((OffensiveUpgrade*)p_Pair->m_Collidable_A);
						//std::cout<<"PLAYERvsCOLLECTABLE"<<std::endl;
					}
				}
				else
				{
					if(p_Pair->m_Collidable_B->isActive())
					{
						TestTankNode* tank = (TestTankNode*)p_Pair->m_Collidable_A;
						tank->AddOffensiveUpgrade((OffensiveUpgrade*)p_Pair->m_Collidable_B);
						//std::cout<<"COLLECTABLEvsPLAYER"<<std::endl;
					}
				}
			}break;
			//handles collision events between Scenery and projectiles
		case PROJECTILEVSSCENERY:
			{
				glm::vec3 normal = glm::vec3(p_Pair->m_CollisionNormal.x,0.0f,p_Pair->m_CollisionNormal.z);
				if(p_Pair->m_Collidable_A->getType()==PROJECTILE)
				{
					//p_Pair->m_Collidable_A->deactivate();
					ProjectileNode* tmp = (ProjectileNode*)p_Pair->m_Collidable_A;
					//this selectes whether the angle is shallow enough for the bullet to reflect
					if(abs(glm::dot(normal,glm::normalize(tmp->getVelocity())))<0.707f)
					{
						glm::vec3 correction = -glm::normalize(p_Pair->m_Collidable_A->getVelocity())*(p_Pair->m_Penetration);
						glm::vec3 pos(p_Pair->m_Collidable_A->getLocation());
						p_Pair->m_Collidable_A->setPosition(pos+correction);
						tmp->Bounce(normal);
					}
					else
					{
						tmp->deactivate();
					}
				}
				else
				{
					//p_Pair->m_Collidable_B->deactivate();
					ProjectileNode* tmp = (ProjectileNode*)p_Pair->m_Collidable_B;
					//this selectes whether the angle is shallow enough for the bullet to reflect
					if(abs(glm::dot(normal,glm::normalize(tmp->getVelocity())))<0.707f)
					{
						glm::vec3 correction = -glm::normalize(p_Pair->m_Collidable_B->getVelocity())*(p_Pair->m_Penetration);
						glm::vec3 pos(p_Pair->m_Collidable_B->getLocation());
						p_Pair->m_Collidable_B->setPosition(pos+correction);
						tmp->Bounce(normal);
					}
					else
					{
						tmp->deactivate();
					}
				}
			}break;
			//handles collision events between enemies and projectiles
		case ENEMYVSPROJECTILE:
			{
				//TODO
			}break;
			//handles collision events between enemies and scenery
		case ENEMYVSSCENERY:
			{
				if(p_Pair->m_Collidable_A->getType() == ENEMY)
				{
					glm::vec3 normal = glm::vec3(p_Pair->m_CollisionNormal.x,0.0f,p_Pair->m_CollisionNormal.z);
					glm::vec3 velocity = p_Pair->m_Collidable_A->getVelocity();
					glm::vec3 reflection = glm::reflect(velocity,normal);
					p_Pair->m_Collidable_A->setPosition(p_Pair->m_Collidable_A->getLocation()+normal*(p_Pair->m_Penetration));
					p_Pair->m_Collidable_A->stop();
					p_Pair->m_Collidable_A->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);
				}
				else
				{
					glm::vec3 normal = glm::vec3(p_Pair->m_CollisionNormal.x,0.0f,p_Pair->m_CollisionNormal.z);
					glm::vec3 reflection = glm::reflect(p_Pair->m_Collidable_B->getVelocity(),glm::normalize(-normal))*0.8f;
					p_Pair->m_Collidable_B->setPosition(p_Pair->m_Collidable_B->getLocation()+normal*(p_Pair->m_Penetration));
					p_Pair->m_Collidable_B->stop();
					p_Pair->m_Collidable_B->setVelocity(glm::vec3(reflection.x,0.0f,reflection.z)*0.8f);

				}
			}break;
			//handles collisions between enemies and player controlled objects
		case PLAYERVSENEMY:
			{
				//TODO
			}break;
		}
	}
}
CollisionResponse::~CollisionResponse(void)
{
}
