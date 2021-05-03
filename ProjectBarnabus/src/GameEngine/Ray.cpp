#include "Ray.h"
#include "PhysicsContainer.h"

Ray::Ray()
{
}

Ray::Ray(glm::vec3 pos, glm::vec3 dir, float len): position(pos), direction(dir), length(len)
{
}

void Ray::SetPosition(glm::vec3 pos)
{
	position = pos;
}

glm::vec3 Ray::GetPosition()
{
	return position;
}

void Ray::SetDirection(glm::vec3 dir)
{
	direction = dir;
}

glm::vec3 Ray::GetDirection()
{
	return direction;
}

void Ray::SetLength(float len)
{
	length = len;
}

bool Ray::IsCollision(Entity& entity, glm::vec3 & poi)
{
	const auto physicsContainer = entity.GetCompatibleComponent<Physics::PhysicsContainer>();
	
	if (!physicsContainer)
	{
		return false;
	}
	else
	{
		auto boundingBoxes = physicsContainer->GetBoundingVolume()->GetBoundingBoxes();

		for (auto bb : boundingBoxes)
		{
			if (TestBoundingBoxCollision(bb))
			{
				return true;
			}
		}
	}

	return false;
}

bool Ray::TestBoundingBoxCollision(BoundingVolumes::BoundingBox bb)
{
	auto min = glm::vec4(bb.GetMinCoordinates(), 1);
	auto max = glm::vec4(bb.GetMaxCoordinates(), 1);

	float tmin = (min.x - position.x) / direction.x;
	float tmax = (max.x - position.x) / direction.x;

	if (tmin > tmax)
	{
		auto temp = tmin;
		tmin = tmax;
		tmax = temp;
	}

	float tymin = (min.y - position.y) / direction.y;
	float tymax = (max.y - position.y) / direction.y;

	if (tymin > tymax)
	{
		auto temp = tymin;
		tymin = tymax;
		tymax = temp;
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	float tzmin = (min.z - position.z) / direction.z;
	float tzmax = (max.z - position.z) / direction.z;

	if (tzmin > tzmax)
	{
		auto temp = tzmin;
		tzmin = tzmax;
		tzmax = temp;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}
	if (tzmax < tmax)
	{
		tmax = tzmax;
	}
	return true;
}
