#include "pch.h"
#include "CollisionPoller.h"
#include "WorldState.h"
#include "EventQueue.h"
#include "World.h"
#include "CollisionManager.h"
#include "CollisionEventAttributed.h"
#include "Event.h"
#include "Player.h"
#include "enemy_bullet.h"

namespace Library
{
	CollisionPoller* CollisionPoller::mInstance = nullptr;
	EventQueue* CollisionPoller::mEventQueue = nullptr;

	const std::string CollisionPoller::PlayerSubType = "player";
	const std::string CollisionPoller::EnemySubType = "enemy";
	const std::string CollisionPoller::BulletSubType = "bullet";

	void CollisionPoller::CreateInstance(EventQueue* queue)
	{
		if (mInstance == nullptr)
		{
			mInstance = new CollisionPoller();
			mEventQueue = queue;
		}
	}

	void CollisionPoller::DeleteInstance()
	{
		delete mInstance;
		mInstance = nullptr;
	}

	CollisionPoller* CollisionPoller::GetInstance()
	{
		return mInstance;
	}

	CollisionPoller::CollisionPoller()
		:mPlayersList(), mEnemyList()
	{
	}

	//register the entity with the poller
	void CollisionPoller::AddEntity(Entity* entity, EntityType type)
	{
		switch (type)
		{
		case EntityType::Player:
			mPlayersList.push_front(entity);
			break;

		case EntityType::Enemy:
			mEnemyList.push_front(entity);
			break;

		default:
			break;
		}
	}

	//remove the entity from the poller
	void CollisionPoller::RemoveEntity(Entity* entity, EntityType type)
	{
		switch (type)
		{
		case EntityType::Player:
			mPlayersList.remove(entity);
			break;

		case EntityType::Enemy:
			mEnemyList.remove(entity);
			break;

		default:
			break;
		}
	}

	//check collision between bullet and other entities
	bool CollisionPoller::CheckCollision(bullet& firedBullet)	
	{
		for (auto player : mPlayersList)
		{
			if (CollisionManager::IsColliding(player->Position(), firedBullet.Position()))
			{
				//check bullet type
				if (firedBullet.getType() != bullet::bulletType::enemies) //if player bullet (assuming we never get hit with our own bullet)
				{
					//send event to the bullet's owner for score
					DispatchCollisionEvent(PlayerSubType, EntityType::MyBullet, firedBullet.getShotter(), player); 
				}

				//let other player know they were hit
				DispatchCollisionEvent(PlayerSubType, EntityType::OtherBullet, player, const_cast<bullet*>(&firedBullet)); 
				return true;
			}
		}

		if (firedBullet.getType() != bullet::bulletType::enemies) //if player bullet, check for collision with enemies
		{
			for (auto enemy1 : mEnemyList)
			{
				if (CollisionManager::IsColliding(enemy1->Position(), firedBullet.Position()))
				{
					//send to the bullet's owner for score
					DispatchCollisionEvent(PlayerSubType, EntityType::MyBullet, firedBullet.getShotter(), enemy1); 

					//let the enemy know it was hit
					DispatchCollisionEvent(EnemySubType, EntityType::OtherBullet, enemy1, const_cast<bullet*>(&firedBullet));
					return true;
				}
			}
		}

		return false;
	}

	//check player collision
	void CollisionPoller::Update()
	{
		for (auto player : mPlayersList)
		{
			//player vs other player
			for (auto otherPlayer : mPlayersList)
			{
				if (otherPlayer != player)
				{
					if (CollisionManager::IsColliding(player->Position(), otherPlayer->Position()))
					{
						DispatchCollisionEvent(PlayerSubType, EntityType::Player, player, otherPlayer);
						DispatchCollisionEvent(PlayerSubType, EntityType::Player, otherPlayer, player);
					}
				}
			}

			//player vs enemy
			for (auto enemy : mEnemyList)
			{
				if (CollisionManager::IsColliding(player->Position(), enemy->Position()))
				{
					DispatchCollisionEvent(PlayerSubType, EntityType::Enemy, player, enemy);
					DispatchCollisionEvent(EnemySubType, EntityType::Player, enemy, player);
				}
			}
		}
	}

	//create an event, populate the payload and dispatch it
	void CollisionPoller::DispatchCollisionEvent(const std::string& sendTo, EntityType collidedWith, Entity* source, Entity* collidedEntity)
	{
		CollisionEventAttributed sourcePayload = CollisionEventAttributed(sendTo, collidedWith, source, collidedEntity);
		auto sourceEvent = std::make_shared<Event<CollisionEventAttributed>>(sourcePayload);
		mEventQueue->Send(sourceEvent);
	}
}