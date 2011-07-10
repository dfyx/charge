#include "staticobstacle.h"

namespace Charge
{
    StaticObstacle::StaticObstacle(b2Vec2 position, float charge, float radius)
    {
        bodyDef.position = position;
        shape.m_radius = radius;
        this->charge = charge;
    }

    const ActorType StaticObstacle::getType() const
    {
        return TYPE_STATIC;
    }
};
