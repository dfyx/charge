#include "player.h"

namespace Charge
{
	Player::Player(unsigned int owner, b2Vec2 position, float charge, float radius)
	{
		bodyDef.position = position;
		bodyDef.type = b2_dynamicBody;
		shape.m_radius = radius;
		this->charge = charge;
		this->owner = owner;
	}

	void Player::reactToPlayer(unsigned int id)
	{
		if(owner == id)
		{
			charge = -charge;
		}
	}
	
	const ActorType Player::getType() const
	{
		return TYPE_PLAYER;
	}
};