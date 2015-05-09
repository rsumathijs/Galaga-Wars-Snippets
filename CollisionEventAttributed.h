#pragma once
#include "EventMessageAttributed.h"
#include "CollisionConstants.h"

namespace Library
{
	class Entity;

	/*! @brief The payload for collision events
	*/
	class CollisionEventAttributed : public EventMessageAttributed
	{
		RTTI_DECLARATIONS(CollisionEventAttributed, EventMessageAttributed);

	public:
		/*! @brief Constructor to initialise members 
			@param subtype of the message
			@param what type of entity it collided with
			@param who to send it to
			@param what it 
		*/
		CollisionEventAttributed(const std::string& sendTo, EntityType collidedWith, Entity* source, Entity* collidedEntity);

		/*! @brief Default destructor */
		~CollisionEventAttributed()=default;

		/*! @brief Getter for what entity was collided with
			@return entity type of collided object
		*/
		EntityType CollidedWith() const;
		
		/*! @brief Getter for the source of the collision
			@return the source collided object
		*/
		Entity* Source() const;

		/*! @brief Getter for the address of entity that was collided with
			@return address of collided object
		*/
		Entity* CollidedEntity() const;

		/*! @brief Copy constructor to copy the collision event attributed
			@param what to copy  from
		*/
		CollisionEventAttributed(const CollisionEventAttributed& rhs);
		
		/*! @brief Assignment operator to copy the collision event attributed
			@param what to copy from
			@return the address of the copied object
		*/
		CollisionEventAttributed& operator=(const CollisionEventAttributed& rhs);

	private:
		EntityType mCollidedWith;
		Entity* mSource;
		Entity* mCollidedEntity;
	};
}
