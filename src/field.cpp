#include "field.h"

namespace Charge
{
    Field::Field()
    {
        world = new b2World(b2Vec2(0.0f, 0.0f), false);
        cleanup = false;
    }

    Field::~Field()
    {
        delete world;
        if(cleanup)
        {
            QList<Actor*>::iterator iter;
            for(iter = actors.begin(); iter != actors.end(); iter++)
            {
                delete *iter;
            }
        }
    }

    void Field::addActor(Actor* actor)
    {
        actors.push_back(actor);
        const b2BodyDef &bodyDef = actor->getBodyDef();
        b2Body *body = world->CreateBody(&bodyDef);
        const b2Shape &shape = actor->getShape();
        body->CreateFixture(&shape, 0.5f);
        actor->body = body;
    }

    const QList<Actor*> Field::getActors() const
    {
        return actors;
    }

    void Field::step(float timestep)
    {
        // Calculate forces caused by electric charge
        QList<Actor*>::iterator iter1;
        for(iter1 = actors.begin(); iter1 != actors.end(); iter1++)
        {
            Actor* actor1 = *iter1;
            QList<Actor*>::iterator iter2;

            for(iter2 = actors.begin(); iter2 != iter1; iter2++){
                Actor* actor2 = *iter2;

                b2Vec2 d = actor2->body->GetWorldCenter() - actor1->body->GetWorldCenter();
                float32 r2 = d.LengthSquared();
                if(r2 < 0.001f)
                        continue;
                b2Vec2 f = 0.001f * actor1->getCharge() * actor2->getCharge() / r2 / sqrt(r2) * d;
                actor1->body->ApplyForce(-1.0f*f, actor1->body->GetWorldCenter());
                actor2->body->ApplyForce(f      , actor2->body->GetWorldCenter());
            }
        }

        // Check for actors that left the field
        for(int i = actors.size() - 1; i >= 0; i--)
        {
            if(!containsActor(actors[i]))
            {
                delete actors[i];
                actors.removeAt(i);
            }
        }

        world->Step(timestep, 5, 5);
        world->ClearForces();
    }

    void Field::reactToPlayer(unsigned int id)
    {
        QList<Actor*>::iterator iter;
        for(iter = actors.begin(); iter != actors.end(); iter++)
        {
            (*iter)->reactToPlayer(id);
        }
    }

    void Field::setCleanup(bool cleanup)
    {
        this->cleanup = cleanup;
    }
};
