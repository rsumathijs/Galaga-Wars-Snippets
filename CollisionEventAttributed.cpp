#include "pch.h"
#include "CollisionEventAttributed.h"
#include "CollisionPoller.h"

namespace Library
{
	RTTI_DEFINITIONS(CollisionEventAttributed)

	CollisionEventAttributed::CollisionEventAttributed(const std::string& sendTo, EntityType collidedWith, Entity* source, Entity* collidedEntity)
		:EventMessageAttributed(), mCollidedWith(collidedWith), mSource(source), mCollidedEntity(collidedEntity)
	{
		SetSubtype(sendTo);
	}


	EntityType CollisionEventAttributed::CollidedWith() const
	{
		return mCollidedWith;
	}

	Entity* CollisionEventAttributed::Source() const
	{
		return mSource;
	}

	Entity* CollisionEventAttributed::CollidedEntity() const
	{
		return mCollidedEntity;
	}

	CollisionEventAttributed::CollisionEventAttributed(const CollisionEventAttributed& rhs)
		:EventMessageAttributed(rhs), mSource(rhs.mSource), mCollidedWith(rhs.mCollidedWith), mCollidedEntity(rhs.mCollidedEntity)
	{
	}

	CollisionEventAttributed& CollisionEventAttributed::operator=(const CollisionEventAttributed& rhs)
	{
		if (*this != rhs)
		{
			EventMessageAttributed::operator=(rhs);

			mSource = rhs.mSource;
			mCollidedWith = rhs.mCollidedWith;
			mCollidedEntity = rhs.mCollidedEntity;
		}

		return *this;
	}
}