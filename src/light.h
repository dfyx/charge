#ifndef LIGHT_H
#define LIGHT_H

#include "glew.h"
#include <QVector3D>
#include <QColor>
#include <QGLShaderProgram>

namespace Charge
{
    class Light
    {
        float matrixData[16];
        QColor color;
        float intensity;

        const static float LIGHTTHRESHOLD = 1.0 / 255.0;
    public:
        Light(const QVector3D &position, const QColor &color, float intensity);

        void draw(QGLShaderProgram *shaderProgram);
    };
}

#endif // LIGHT_H
