#pragma once
#include "CollisionConstants.h"
#include "enemy_bullet.h"
#include <forward_list>
#include <mutex>

namespace Library
{
	class EventQueue;
	class WorldState;
	class Entity;

	/*! @brief the class that polls to check if collision happened between registered entities
	and sends events in the event of collision
	*/
	class CollisionPoller
	{
	public:
		/*! @brief Gets an instance of the poller
			@return address of the instance of the poller
		*/
		static CollisionPoller* GetInstance();

		/*! @brief Creates a new instance of the poller, if one doesn't exist. Ideally should be called only once.
		*/
		static void CreateInstance(EventQueue* queue);

		/*! Delete the instance of the poller. Ideally should be called once.
		*/
		static void DeleteInstance();

		/*! @brief Registers the entity with the poller
			@param the address of the entity
			@param if it is a enemy or player
		*/
		void AddEntity(Entity* entity, EntityType type);
		
		/*! @brief Unregisters the entity from the poller
			@param the address of the entity
			@param if it is a enemy or player
		*/
		void RemoveEntity(Entity* entity, EntityType type);

		/*! @brief Default constructor to release members */
		~CollisionPoller() = default;

		/*!	@brief Check collision between the bullet and the other entities
			@param the bullet to check for
		*/
		bool CheckCollision(bullet& bullet);

		/*! @brief Check if any player collided with enemy or other player
			@param state of the world
		*/
		void Update(WorldState& state); 
		
		/*! Const strings to represent the subtypes */
		static const std::string PlayerSubType;
		static const std::string EnemySubType;
		static const std::string BulletSubType;

	private:
		/*! @brief Private default constructor */
		CollisionPoller();

		/*! @brief Helper method to send out a collision event 
			@param subtype of the message
			@param what type of entity it collided with
			@param who to send it to
			@param what it collided with
		*/
		void DispatchCollisionEvent(const std::string& sendTo, EntityType collidedWith, Entity* source, Entity* collidedEntity);

		std::forward_list<Entity*> mPlayersList;
		std::forward_list<Entity*> mEnemyList;

		static CollisionPoller* mInstance;
		static EventQueue* mEventQueue;
	};
}
