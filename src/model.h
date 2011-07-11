#ifndef MODEL_H
#define MODEL_H

#include "glew.h"
#include <QMap>
#include <QString>

namespace Charge
{
    class Model
    {
        struct ModelInfo
        {
            GLuint count, vertexBuffer, normalBuffer, texCoordBuffer, indexBuffer, numFaces;
        };

        GLuint vertexBuffer, normalBuffer, texCoordBuffer, indexBuffer, numFaces;
        QMap<QString, ModelInfo> cache;
        QString filename;
    public:
        Model(QString filename);
        ~Model();

        void draw();
    };
}

#endif // MODEL_H
