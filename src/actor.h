#ifndef CHARGE_ACTOR_H
#define CHARGE_ACTOR_H

#include "field.h"
#include <Box2D/Box2D.h>

namespace Charge
{
	enum ActorType
	{
		TYPE_UNKNOWN,
		TYPE_STATIC,
		TYPE_DYNAMIC,
		TYPE_PLAYER
	};
	
	class Actor
	{
	protected:
		float charge;
		b2BodyDef bodyDef;
		b2CircleShape shape;
		b2Body *body;
	public:
		friend class Field;
		Actor();

		const b2Vec2 getPosition() const;
		const b2Vec2 getVelocity() const;
		const float getCharge() const;
		const float getRadius() const;

		const b2BodyDef getBodyDef() const;
		const b2CircleShape getShape() const;
		
		virtual void reactToPlayer(unsigned int id) {};
		
		virtual const ActorType getType() const;
	};
};

#endif // CHARGE_ACTOR_H