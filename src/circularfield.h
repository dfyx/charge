#ifndef CHARGE_CIRCULARFIELD_H
#define CHARGE_CIRCULARFIELD_H

#include "field.h"

namespace Charge
{
    class CircularField : public Field
    {
    private:
        float radius;
    public:
        CircularField(float radius);

        const float getRadius() const;

        virtual bool containsActor(Actor *actor) const;
    };
};

#endif // CHARGE_CIRCULARFIELD_H
