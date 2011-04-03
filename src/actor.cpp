#include "actor.h"

namespace Charge
{
	Actor::Actor()
	{
		body = NULL;
		bodyDef.userData = (void*) this;
	}

	const b2Vec2 Actor::getPosition() const
	{
		if(body == NULL)
		{
			return bodyDef.position;
		}
		else
		{
			return body->GetWorldCenter();
		}
	}

	const b2Vec2 Actor::getVelocity() const
	{
		if(body == NULL)
		{
			return bodyDef.linearVelocity;
		}
		else
		{
			return body->GetLinearVelocity();
		}
	}

	const float Actor::getCharge() const
	{
		return charge;
	}

	const float Actor::getRadius() const
	{
		return shape.m_radius;
	}

	const b2BodyDef Actor::getBodyDef() const
	{
		return bodyDef;
	}

	const b2CircleShape Actor::getShape() const
	{
		return shape;
	}
	
	const ActorType Actor::getType() const
	{
		return TYPE_UNKNOWN;
	}
};