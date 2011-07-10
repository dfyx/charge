#ifndef CHARGE_LOADER_H
#define CHARGE_LOADER_H

#include "field.h"
#include <QString>
#include <QStringList>

namespace Charge
{
    class ChargeLoader
    {
    public:
        Field *loadField(QString filename);
    protected:
        void initFieldType(QStringList values, Field **field);
        void addPlayer(QStringList values, Field *field);
        void addStatic(QStringList values, Field *field);
        void addDynamic(QStringList values, Field *field);
    };
};

#endif // CHARGE_LOADER_H
