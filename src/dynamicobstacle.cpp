#include "dynamicobstacle.h"

namespace Charge
{
    DynamicObstacle::DynamicObstacle(b2Vec2 position, float charge, float radius)
    {
        bodyDef.position = position;
        bodyDef.type = b2_dynamicBody;
        shape.m_radius = radius;
        this->charge = charge;
    }

    const ActorType DynamicObstacle::getType() const
    {
        return TYPE_DYNAMIC;
    }
};
