#include "pch.h"
#include "CollisionManager.h"

namespace Library
{
	const float CollisionManager::MinimumRotationError = 5.0f;

	//the vec4 is encoded as follows : (x position, y position, width, height)
	bool CollisionManager::IsColliding(glm::vec4 object1, glm::vec4 object2,
										float object1Rotation, float object2Rotation)
	{
		//decide what collider to give the objects
		//if either of them arent axis aligned, give them a circle collider
		if (CheckForAxisAlignment(object1Rotation,90) && CheckForAxisAlignment(object1Rotation,(int)CollisionAxisAlignment::BothAxis))
		{
			return CheckBoxCollision(object1, object2, object1Rotation,object2Rotation);
		}
		else
		{
			return CheckCircleCollision(object1, object2);
		}
	}

	//check box collision using Axis Aligned Bounding Box Algorithm
	bool CollisionManager::CheckBoxCollision(glm::vec4 object1, glm::vec4 object2,
											float object1Rotation, float object2Rotation)
	{		
		//calculate the half width and half height for each object
		//if the object if rotated by 90 or 270, we take width as height and height as width
		bool isO1AxisAligned = CheckForAxisAlignment(object1Rotation, (int)CollisionAxisAlignment::XAxis);
		float o1Width = (isO1AxisAligned ? object1.z : object1.w);
		float o1Height = (isO1AxisAligned ? object1.w : object1.z);
		float o1HalfWidth = o1Width / 2;
		float o1HalfHeight = o1Height / 2;

		bool isO2AxisAligned = CheckForAxisAlignment(object2Rotation, (int)CollisionAxisAlignment::XAxis);
		float o2Width = (isO2AxisAligned ? object2.z : object2.w);
		float o2Height = (isO2AxisAligned ? object2.w : object2.z);
		float o2HalfWidth = o2Width / 2;
		float o2HalfHeight = o2Height / 2;

		//checking that the top right corner of object 1 against the bottom left corner of object 2
		//and top right corner of object 2 against the bottom left corner of object 1
		if (object1.x + o1HalfWidth >= object2.x - o2HalfWidth && object1.y + o1HalfHeight >= object2.y - o2HalfHeight &&
			object2.x + o2HalfWidth >= object1.x - o1HalfWidth && object2.y + o2HalfHeight >= object1.y - o1HalfHeight)
		{
			return true; //collision happened
		}

		return false; //no collision
	}

	bool CollisionManager::CheckCircleCollision(glm::vec4 object1, glm::vec4 object2)
	{
		//selecting the bigger of the x and y scales as the radius
		float obj1Radius = (object1.z > object1.w ? object1.z : object1.w);
		float obj2Radius = (object2.z > object2.w ? object2.z : object2.w);

		float distanceBetween = std::sqrtf(obj1Radius*obj1Radius + obj2Radius*obj2Radius);

		if (distanceBetween < obj1Radius + obj2Radius)
		{
			return true; //collision happened
		}

		return false; //no collision
	}

	bool CollisionManager::CheckForAxisAlignment(float rotation, int angle)
	{
		//check for perfect axis alignment
		if ((int)rotation % angle == 0)
		{
			return true;
		}
		//very slight error from being axis aligned
		else
		{
			int mod = (int)rotation % angle;

			//check if within the + MinimumRotationError to - MinimumRotationError
			if (mod < MinimumRotationError || (angle-mod) < MinimumRotationError)
			{
				return true;
			}
		}

		//not axis-aligned
		return false;
	}
}