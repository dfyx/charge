#ifndef CHARGE_FIELD_H
#define CHARGE_FIELD_H

#include "actor.h"
#include <vector>
#include <Box2D/Box2D.h>

namespace Charge
{
	class Field
	{
	private:
		std::vector<class Actor*> actors;
		b2World *world;
	public:
		Field();
		~Field();
		void addActor(class Actor* actor);
		const std::vector<class Actor*> getActors() const;

		void step(float timestep);

		virtual bool containsActor(class Actor *actor) const = 0;
	};
};

#endif // CHARGE_FIELD_H