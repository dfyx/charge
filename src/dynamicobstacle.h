#ifndef CHARGE_DYNAMICOBSTACLE_H
#define CHARGE_DYNAMICOBSTACLE_H

#include "actor.h"

namespace Charge
{
    const float CHARGE_DYNAMICOBSTACLE_DEFAULT_RADIUS = 0.05f;

    class DynamicObstacle : public Actor
    {
    public:
        DynamicObstacle(b2Vec2 position, float charge, float radius = CHARGE_DYNAMICOBSTACLE_DEFAULT_RADIUS);
        virtual const ActorType getType() const;
    };
};

#endif // CHARGE_DYNAMICOBSTACLE_H
