#ifndef MAINCANVAS_H
#define MAINCANVAS_H

#include "glew.h" // Don't move this to the source file. glew.h must be included before gl.h
#include <QGLWidget>
#include <QGLShaderProgram>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include "circularfield.h"
#include "model.h"
#include "light.h"

namespace Charge
{

    class MainCanvas : public QGLWidget
    {
        Q_OBJECT
        CircularField *field;

        QTimer *timer;
        QTime *cameraTimer;

        Model *playerModel, *dynamicModel, *staticModel, *fieldModel;
        QGLShaderProgram *defaultShaderProgram, *playerShaderProgram, *lightShaderProgram, *ambientShaderProgram;

        QList<Light> lights;

        // Buffers for deferred shading
        GLuint frameBuffer, depthBuffer, diffuseBuffer, specularBuffer, positionBuffer, normalBuffer;

        const static unsigned int TIMESTEP = 10;
    public:
        explicit MainCanvas(CircularField *field, QWidget *parent = 0);
        ~MainCanvas();

        void initializeGL();
        void paintGL();
        void resizeGL(int width, int height);

    private:
        void renderPlayer(class Player *player);
        void renderStatic(class StaticObstacle *obstacle);
        void renderDynamic(class DynamicObstacle *obstacle);

        void drawCircle(b2Vec2 position, float radius, bool filled = false);
        QColor getChargeColor(float charge);

        void geometryRenderPass();
        void shadingRenderPass();

        void setBufferUniforms(QGLShaderProgram *shaderProgram);

        void setupProjectionCamera(int width, int height);
        void setupFrameBuffer();

    signals:

    public slots:
        void updateScene();
    };

};

#endif // MAINCANVAS_H
