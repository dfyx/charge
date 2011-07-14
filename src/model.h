#ifndef MODEL_H
#define MODEL_H

#include "glew.h"
#include <QMap>
#include <QString>
#include <QGLShaderProgram>

namespace Charge
{
    class Model
    {
        struct ModelInfo
        {
            GLuint count, vertexBuffer, normalBuffer, texCoordBuffer, indexBuffer, numFaces;
        };

        GLuint vertexBuffer, normalBuffer, texCoordBuffer, indexBuffer, numFaces;
        static QMap<QString, ModelInfo> cache;
        QString filename;

        QMap<QString, GLuint> textures;
    public:
        Model(QString filename);
        ~Model();

        void loadTexture(QString texname, QString filename);

        void draw(QGLShaderProgram *shaderProgram = NULL);
    };
}

#endif // MODEL_H
