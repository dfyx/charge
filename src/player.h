#ifndef CHARGE_PLAYER_H
#define CHARGE_PLAYER_H

#include "actor.h"

namespace Charge
{
	const float CHARGE_PLAYER_DEFAULT_CHARGE = 1.0f;
	const float CHARGE_PLAYER_DEFAULT_RADIUS = 0.1f;

	class Player : public Actor
	{
	public:
		Player(b2Vec2 position, float charge = CHARGE_PLAYER_DEFAULT_CHARGE, float radius = CHARGE_PLAYER_DEFAULT_RADIUS);

		void invertCharge();
	};
};

#endif // CHARGE_PLAYER_H