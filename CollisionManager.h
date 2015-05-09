#pragma once
#include "Datum.h"

namespace Library
{
	class CollisionManager
	{
	public:
		/*!	@brief Checks if the two objects are colliding
			@param the first object (vec4)
			@param the second object (vec4)
			@param rotation of first object (in degrees)
			@param rotation of second object (in degrees)
			@returns true if the two objects are intersecting, false otherwise
		*/
		static bool IsColliding(glm::vec4 object1, glm::vec4 object2,
								float object1Rotation=0.0, float object2Rotation=0.0);

		/*!	@brief An error range for a rotation to be considered as axis aligned.
					that is, the how far the angle is allowed to be from 0, 90, 180 and 270
		*/
		static const float MinimumRotationError;

	private:
		/*! @brief Build a box collider and checks if the two objects collide
			@param the first object (vec4)
			@param the second object (vec4)
			@param rotation of first object (in degrees)
			@param rotation of second object (in degrees)
		*/
		static bool CheckBoxCollision(glm::vec4 object1, glm::vec4 object2,
									  float object1Rotation, float object2Rotation);

		/*! @brief Build a circle collider and checks if the two objects collide
			@param the first object (vec4)
			@param the second object (vec4)
		*/
		static bool CheckCircleCollision(glm::vec4 object1, glm::vec4 object2);

		/*! @brief check if the object rotation is aligned with the specified axis
			for example : if we want to check if it aligned to the x axis, the angle would be 180
			and rotations of multiples of 180 are aligned with that axis
			@param rotation of the object
			@param angle to check for alignment
		*/
		static bool CheckForAxisAlignment(float objRotation, int angle);

		/*! @brief Enum for the various alignment angles for collision detection
		*/
		enum class CollisionAxisAlignment
		{
			BothAxis = 90,
			XAxis = 180
		};

	};
}
