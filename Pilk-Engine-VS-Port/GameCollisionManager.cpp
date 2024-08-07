#pragma once

#include "PilkEngineCommon.h"
#include "CollisionManager.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Timer.h"

/*

Game specific collision manager:
by Matthew Liney

This class contains the responses for all of the collisions detected in the systems (see Systems.h).
The collisions go through the process collisions method, each response is divided into the type of collision
that has occured, AABB_AABB_COLLISION_RIGHT collisions are responded to in RespondAABBLeftRight, for example.


*/

class GameCollisionManager : public CollisionManager
{
public:

	using CollisionManager::CollisionManager;
	Timer* player1DamageTimer = new Timer();
	Timer* player2DamageTimer = new Timer();
	bool has_won = false;

	void ProcessCollisions() override
	{
		for (Collision* col : m_collisionManifold)
		{
			if (col->m_collisionType == AABB_AABB_COLLISION_RIGHT || col->m_collisionType == AABB_AABB_COLLISION_LEFT)
			{
				RespondAABBAABBLeftRight(col);
			}
			else if (col->m_collisionType == AABB_AABB_COLLISION_TOP || col->m_collisionType == AABB_AABB_COLLISION_BOTTOM)
			{
				RespondAABBAABBUpDown(col);
			}
			else if (col->m_collisionType == AABB_POINT)
			{
				RespondAABBPoint(col);
			}
			else if (col->m_collisionType == AABB_SPHERE_POINT)
			{
				RespondPointSphere(col);
			}
		}
	}

	/*
	
	Collision edge case responses:
	by Matthew Liney

	Edge cases can occur in collisions which require a special response. CollisionEdgeCase is called in the AABB response methods,
	and will leave the method early if an edge case is satisfied. Bullets and enemies both have AABB hitboxes, so if a bullet hits
	an enemy, it needs a specialised reponse. Here, under bullet cases, it calls the specialised method to respond, instead of the
	defauly AABB response. This method is used to win the game after touching the gem, ignoring the collisions between enemies, 
	and so on.

	*/

	bool CollisionEdgeCase(Collision* p_col)
	{
		bool edge = false;
		Entity* e1 = p_col->m_entity1;
		Entity* e2 = p_col->m_entity2;

		// bullet cases
		if (e1->GetName() == "Bullet" || e2->GetName() == "Bullet")
		{
			if (e1->GetName() == "Block" || e2->GetName() == "Block" || e1->GetName() == "Goal" || e2->GetName() == "Goal")
			{
				BulletWall(e1, e2);
				edge = true;
			}
			else if (e1->GetName() == "GroundEnemy" || e2->GetName() == "GroundEnemy" || e1->GetName() == "FlyingEnemy" || e2->GetName() == "FlyingEnemy")
			{
				BulletEnemy(e1, e2);
				edge = true;
			}
			else if (e1->GetName() == "Player2" || e2->GetName() == "Player2" || e1->GetName() == "Player1" || e2->GetName() == "Player1")
			{
				edge = true;
			}
		}
		// enemy cases
		else if (e1->GetName() == "GroundEnemy" || e2->GetName() == "GroundEnemy" || e1->GetName() == "FlyingEnemy" || e2->GetName() == "FlyingEnemy")
		{
			if (e1->GetName() == "Player1" || e2->GetName() == "Player1")
			{
				EnemyPlayer(e1,e2);
				edge = true;
			}
			else if (e1->GetName() == "Player2" || e2->GetName() == "Player2")
			{
				EnemyPlayer(e1,e2);
				edge = true;
			}
			if (e1->GetName() == e2->GetName())
			{
				edge = true;
			}
		}
		else if (e1->GetName() == "Player2" || e2->GetName() == "Player2" || e1->GetName() == "Player1" || e2->GetName() == "Player1")
		{
			if (e1->GetName() == "Goal" || e2->GetName() == "Goal")
			{
				edge = true;
				has_won = true;
			}
		}

		return edge;
	}

	bool HasWon()
	{
		return has_won;
	}

	void RespondAABBAABBLeftRight(Collision* p_col)
	{
		if (CollisionEdgeCase(p_col))
		{
			return;
		}

		Entity* e1 = p_col->m_entity1;
		Entity* e2 = p_col->m_entity2;

		ComponentTransform* pos1 = e1->GetComponent<ComponentTransform>();
		ComponentTransform* pos2 = e2->GetComponent<ComponentTransform>();

		ComponentCollisionAABB* col1 = e1->GetComponent<ComponentCollisionAABB>();
		ComponentCollisionAABB* col2 = e2->GetComponent<ComponentCollisionAABB>();

		vec3 new_pos = vec3(pos2->m_translation.x, pos1->m_translation.y, pos1->m_translation.z);

		if (p_col->m_collisionType == AABB_AABB_COLLISION_RIGHT)
		{
			new_pos.x += ((col1->GetWidth() / 2) + (col2->GetWidth() / 2));

			if (p_col->m_entity1->GetName() == "GroundEnemy" || p_col->m_entity1->GetName() == "FlyingEnemy")
			{
				ComponentPhysics* phys = p_col->m_entity1->GetComponent<ComponentPhysics>();
				phys->SetVelX(-phys->GetVelocity().x);

				vec3 newscale = pos1->m_scale;
				newscale.x = -newscale.x;
				pos1->UpdateScale(newscale);

			}
		}
		else if (p_col->m_collisionType == AABB_AABB_COLLISION_LEFT)
		{
			new_pos.x -= ((col1->GetWidth()/2) + (col2->GetWidth()/2));
			if (p_col->m_entity1->GetName() == "GroundEnemy" || p_col->m_entity1->GetName() == "FlyingEnemy")
			{
				ComponentPhysics* phys = p_col->m_entity1->GetComponent<ComponentPhysics>();
				phys->SetVelX(-phys->GetVelocity().x);

				vec3 newscale = pos1->m_scale;
				newscale.x = -newscale.x;
				pos1->UpdateScale(newscale);
			}
		}

		pos1->UpdateTranslation(new_pos);

	}

	void RespondAABBAABBUpDown(Collision* p_col)
	{
		if (CollisionEdgeCase(p_col))
		{
			return;
		}

		Entity* e1 = p_col->m_entity1;
		Entity* e2 = p_col->m_entity2;

		ComponentTransform* pos1 = e1->GetComponent<ComponentTransform>();
		ComponentTransform* pos2 = e2->GetComponent<ComponentTransform>();

		ComponentCollisionAABB* col1 = e1->GetComponent<ComponentCollisionAABB>();
		ComponentCollisionAABB* col2 = e2->GetComponent<ComponentCollisionAABB>();

		vec3 new_pos = vec3(pos1->m_translation.x, pos2->m_translation.y, pos1->m_translation.z);

		if (p_col->m_collisionType == AABB_AABB_COLLISION_TOP)
		{
			new_pos.y += ((col1->GetHeight() / 2) + (col2->GetHeight() / 2));
		}
		else
		{
			new_pos.y -= ((col1->GetHeight() / 2) + (col2->GetHeight() / 2));
		}

		pos1->UpdateTranslation(new_pos);
	}

	void BulletWall(Entity* p_e1, Entity* p_e2)
	{
		if (p_e1->GetName() == "Bullet")
		{
			p_e1->Delete(true);
		}
		else
		{
			p_e2->Delete(true);
		}
	}

	void BulletEnemy(Entity* p_e1, Entity* p_e2)
	{
		if (p_e1->GetName() == "Bullet")
		{
			p_e1->Delete(true);
			
			ComponentPhysics* phys = p_e2->GetComponent<ComponentPhysics>();
			phys->SetVelZ(4.0f);
			phys->SetVelX(0.0f);
			phys->SetVelY(10.0f);
			phys->SetGravity(vec3(0.0f, -0.3f, 0.0f));

			ComponentCollisionAABB* aabb = p_e2->GetComponent<ComponentCollisionAABB>();
			ComponentCollisionSphere* sp = p_e2->GetComponent<ComponentCollisionSphere>();
			ComponentCollisionPoint* point = p_e2->GetComponent<ComponentCollisionPoint>();

			if (sp != nullptr)
			{
				sp->m_is_active = false;
			}
			if (aabb != nullptr)
			{
				aabb->m_is_active = false;
			}
			if (point != nullptr)
			{
				point->m_is_active = false;
			}
		}
		else
		{
			p_e2->Delete(true);
			//SoundEngine->play2D("resources/sounds/batsound.mp3", false);
		}
	}

	void EnemyPlayer(Entity* p_e1, Entity* p_e2)
	{	
		if (p_e1->GetName() == "Player1" && player1DamageTimer->GetElapsedTime() > 1.0f) 
		{
			ComponentProperties* prop = p_e1->GetComponent<ComponentProperties>();
			prop->m_health -= 1.0f;
			SoundEngine->play2D("resources/sounds/damagesound.mp3", false);
			std::cout << "p1 ouch!" << std::endl;
			player1DamageTimer->Restart();

			std::vector<Entity*> player1hearts = m_entityManager->FindEntities("P1Heart");

			std::cout << player1hearts.size() << std::endl;

			if (player1hearts.size() > 0)
			{
				ComponentPhysics* phys = player1hearts[0]->GetComponent<ComponentPhysics>();
				phys->SetVelZ(0.0f);
				phys->SetVelX(0.0f);
				phys->SetVelY(5.0f);
				phys->SetGravity(vec3(0.0f, -0.3f, 0.0f));
				player1hearts[0]->SetName("thomas beet");
				//m_systemManager->RemoveEntity(player1hearts[0]);
				//m_entityManager->RemoveEntity(player1hearts[0]->GetID());
				//player1hearts[0]->Delete(true);
			}
		}

		if (p_e1->GetName() == "Player2" && player2DamageTimer->GetElapsedTime() > 1.0f)
		{
			ComponentProperties* prop = p_e1->GetComponent<ComponentProperties>();
			prop->m_health -= 1.0f;
			SoundEngine->play2D("resources/sounds/damagesound.mp3", false);
			std::cout << "p2 ouch!" << std::endl;
			player2DamageTimer->Restart();

			std::vector<Entity*> player2hearts = m_entityManager->FindEntities("P2Heart");

			if (player2hearts.size() > 0)
			{
				ComponentPhysics* phys = player2hearts[0]->GetComponent<ComponentPhysics>();
				phys->SetVelZ(0.0f);
				phys->SetVelX(0.0f);
				phys->SetVelY(10.0f);
				phys->SetGravity(vec3(0.0f, -0.3f, 0.0f));
				player2hearts[0]->SetName("thomas beet");
				//m_systemManager->RemoveEntity(player2hearts[0]);
				//m_entityManager->RemoveEntity(player2hearts[0]->GetID());
				//player2hearts[0]->Delete(true);
			}
		}
	}

	/*
	
	Point and sphere collisions:
	by Matthew Liney

	Points and spheres are not used for more than one purpose. Therefore, unlike AABB repsonses, they only have
	one specific response. AABB point is used for the platforming logic to deactive gravity if a player is touching
	the floor (otherwise, the player would grind to a halt as gravity accelerates them towards the ground, even when
	they are touching it). The sphere point collision is only used for the bat enemies, moving them towards the player
	if they collide.
	
	*/

	void RespondAABBPoint(Collision* p_col)
	{
		ComponentCollisionPoint* point = p_col->m_entity1->GetComponent<ComponentCollisionPoint>();
		if (point != nullptr && (p_col->m_entity1->GetName() == "GroundEnemy"))
		{
			if (point->m_is_active == false)
			{
				return;
			}
		}

		ComponentPhysics* phys1 = p_col->m_entity1->GetComponent<ComponentPhysics>();
		phys1->SetGravity(vec3(0, 0, 0));
		phys1->SetCurrentGravity(vec3(0, 0, 0));
		phys1->SetVelY(0);

		ComponentProperties* prop = p_col->m_entity1->GetComponent<ComponentProperties>();
		prop->m_hasJumped = false;
	}

	void RespondPointSphere(Collision* p_col)
	{
		ComponentTransform* trans1 = p_col->m_entity1->GetComponent<ComponentTransform>();

		ComponentPhysics* phys2 = p_col->m_entity2->GetComponent<ComponentPhysics>();
		ComponentTransform* trans2 = p_col->m_entity2->GetComponent<ComponentTransform>();

		vec3 pos1 = trans1->m_translation;
		vec3 pos2 = trans2->m_translation;
		vec3 distance = pos2 - pos1;
		distance = glm::normalize(distance) * 2.0f;

		phys2->SetVelVector(-distance);
	}
};