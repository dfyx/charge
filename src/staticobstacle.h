#ifndef CHARGE_STATICOBSTACLE_H
#define CHARGE_STATICOBSTACLE_H

#include "actor.h"

namespace Charge
{
    const float CHARGE_STATICOBSTACLE_DEFAULT_RADIUS = 0.05f;

    class StaticObstacle : public Actor
    {
    public:
        StaticObstacle(b2Vec2 position, float charge, float radius = CHARGE_STATICOBSTACLE_DEFAULT_RADIUS);
        virtual const ActorType getType() const;
    };
};

#endif // CHARGE_STATICOBSTACLE_H
