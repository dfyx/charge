#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H

#include <QString>
#include <QVector3D>
#include <QVector2D>
#include <QStringList>
#include <QList>
#include "glew.h"

namespace Charge
{
    class ObjectLoader
    {
    public:
        ObjectLoader();

        void loadObject(QString filename, GLuint vertexBuffer, GLuint normalBuffer, GLuint texCoordBuffer, GLuint indexBuffer) const;
    protected:
        const QVector3D loadVertex(QStringList fields) const;
        const QVector2D loadTextureCoords(QStringList fields) const;
        const QVector3D loadNormal(QStringList fields) const;
        const QList<unsigned int> loadFace(QStringList fields) const;
    };
}

#endif // OBJECTLOADER_H
