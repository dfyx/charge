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
        bool cleanup;
    public:
        Field();
        ~Field();
        void addActor(class Actor* actor);
        const std::vector<class Actor*> getActors() const;

        void step(float timestep);
        void reactToPlayer(unsigned int id);
        void setCleanup(bool cleanup);

        virtual bool containsActor(class Actor *actor) const = 0;
    };
};

#endif // CHARGE_FIELD_H
