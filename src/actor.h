#ifndef CHARGE_ACTOR_H
#define CHARGE_ACTOR_H

#include "field.h"
#include <Box2D/Box2D.h>

namespace Charge
{
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
	};
};

#endif // CHARGE_ACTOR_H