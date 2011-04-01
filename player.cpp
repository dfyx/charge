#include "player.h"

namespace Charge
{
	Player::Player(b2Vec2 position, float charge, float radius)
	{
		bodyDef.position = position;
		bodyDef.type = b2_dynamicBody;
		shape.m_radius = radius;
		this->charge = charge;
	}

	void Player::invertCharge()
	{
		charge = -charge;
	}
};