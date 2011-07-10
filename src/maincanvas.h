#ifndef MAINCANVAS_H
#define MAINCANVAS_H

#include <QGLWidget>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include "circularfield.h"

namespace Charge
{

    class MainCanvas : public QGLWidget
    {
        Q_OBJECT
        CircularField *field;

        QTimer *timer;
        QTime *cameraTimer;

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
        void setChargeColor(float charge);

    signals:

    public slots:
        void updateScene();
    };

};

#endif // MAINCANVAS_H
