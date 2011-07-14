#include "light.h"
#include <cmath>

namespace Charge
{
    Light::Light(const QVector3D &position, const QColor &color, float intensity)
    {
        // Get matrix
        glPushMatrix();
        glTranslatef(position.x(), position.y(), position.z());
        glGetFloatv(GL_MODELVIEW_MATRIX, matrixData);
        glPopMatrix();

        this->color = color;
        this->intensity = intensity;
    }

    void Light::draw(QGLShaderProgram *shaderProgram)
    {
        glPushMatrix();
        glLoadMatrixf(matrixData);

        // Render lights
        float radius = sqrt(intensity / LIGHTTHRESHOLD);
        GLUquadricObj *quadric = gluNewQuadric();
        gluQuadricDrawStyle(quadric, GLU_FILL);
        shaderProgram->setUniformValue("lightColor", color.redF(), color.greenF(), color.blueF(), intensity);
        gluSphere(quadric, radius, 16, 8);

        glPopMatrix();
    }
}
