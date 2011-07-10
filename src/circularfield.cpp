#include "circularfield.h"

namespace Charge
{
    CircularField::CircularField(float radius)
    {
        this->radius = radius;
    }

    const float CircularField::getRadius() const
    {
        return radius;
    }

    bool CircularField::containsActor(Actor *actor) const
    {
        return actor->getPosition().Length() <= radius;
    }
};
