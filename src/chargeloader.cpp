#include "chargeloader.h"

#include "circularfield.h"
#include "player.h"
#include "staticobstacle.h"
#include "dynamicobstacle.h"

#include <QFile>

namespace Charge
{
    Field *ChargeLoader::loadField(QString filename)
    {
        Field *field = NULL;
        QFile file(filename);

        if(!file.open(QIODevice::ReadOnly))
        {
            return field;
        }

        while(file.peek(1).size() > 0)
        {
            QStringList values = QString(file.readLine()).split(' ');

            if(values[0] == "type")
            {
                initFieldType(values, &field);
            }
            else if(values[0] == "player")
            {
                addPlayer(values, field);
            }
            else if(values[0] == "static")
            {
                addStatic(values, field);
            }
            else if(values[0] == "dynamic")
            {
                addDynamic(values, field);
            }
        }

        file.close();

        return field;
    }

    void ChargeLoader::initFieldType(QStringList values, Field **field)
    {
        if(*field != NULL || values.size() != 3)
        {
            return;
        }

        if(values[1] == "circle")
        {
            float radius = values[2].toFloat();
            *field = new CircularField(radius);
            (*field)->setCleanup(true);
        }
    }

    void ChargeLoader::addPlayer(QStringList values, Field *field)
    {
        if(field == NULL || values.size() != 6)
        {
            return;
        }

        unsigned int id = values[1].toInt();
        float x = values[2].toFloat();
        float y = values[3].toFloat();
        float charge = values[4].toFloat();
        float radius = values[5].toFloat();
        field->addActor(new Player(id, b2Vec2(x, y), charge, radius));
    }

    void ChargeLoader::addStatic(QStringList values, Field *field)
    {
        if(field == NULL || values.size() != 5)
        {
            return;
        }

        float x = values[1].toFloat();
        float y = values[2].toFloat();
        float charge = values[3].toFloat();
        float radius = values[4].toFloat();
        field->addActor(new StaticObstacle(b2Vec2(x, y), charge, radius));
    }

    void ChargeLoader::addDynamic(QStringList values, Field *field)
    {
        if(field == NULL || values.size() != 5)
        {
            return;
        }

        float x = values[1].toFloat();
        float y = values[2].toFloat();
        float charge = values[3].toFloat();
        float radius = values[4].toFloat();
        field->addActor(new DynamicObstacle(b2Vec2(x, y), charge, radius));
    }
}
