#include "maincanvas.h"
#include "player.h"
#include "dynamicobstacle.h"
#include "staticobstacle.h"

#include <vector>

using namespace std;

static float playerColors[][3] =
{{0.5f, 0.5f, 0.5f},
 {1.0f, 0.0f, 0.0f},
 {0.0f, 0.0f, 1.0f},
 {0.0f, 1.0f, 0.0f},
 {1.0f, 1.0f, 0.0f}};

namespace Charge
{
    MainCanvas::MainCanvas(CircularField *field, QWidget *parent) :
        QGLWidget(parent)
    {
        this->field = field;

        timer = new QTimer(this);
        timer->setSingleShot(false);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    }

    MainCanvas::~MainCanvas()
    {
        delete timer;
    }

    void MainCanvas::initializeGL()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    #ifdef GL_MULTISAMLE
        glEnable(GL_MULTISAMPLE);
    #endif
        timer->start(TIMESTEP);
    }

    void MainCanvas::resizeGL(int width, int height)
    {
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float radius = field->getRadius() * 1.2f;
        if(width > height)
        {
            float ratio = float(width) / float(height);
            glOrtho(-radius * ratio, radius * ratio, -radius, radius, -1.0, 1.0);
        }
        else
        {
            float ratio = float(height) / float(width);
            glOrtho(-radius, radius, -radius * ratio, radius * ratio, -1.0, 1.0);
        }
    }

    void MainCanvas::paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Draw field
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(b2Vec2(0.0f, 0.0f), field->getRadius());

        // Draw actors
        vector<Actor*> actors = field->getActors();
        vector<Actor*>::iterator iter;
        for(iter = actors.begin(); iter != actors.end(); iter++)
        {
            switch((*iter)->getType())
            {
                case TYPE_PLAYER:
                    renderPlayer((Player*) *iter);
                    break;
                case TYPE_STATIC:
                    renderStatic((StaticObstacle*) *iter);
                    break;
                case TYPE_DYNAMIC:
                    renderDynamic((DynamicObstacle*) *iter);
                    break;
            }
        }
    }

    void MainCanvas::updateScene()
    {
        field->step(float(TIMESTEP) / 1000.0f);
        updateGL();
    }

    void MainCanvas::renderPlayer(Player *player)
    {
        setChargeColor(player->getCharge());
        drawCircle(player->getPosition(), player->getRadius());

        glColor3fv(playerColors[player->getOwner()]);
        drawCircle(player->getPosition(), player->getRadius() / 2, true);
    }

    void MainCanvas::renderStatic(StaticObstacle *obstacle)
    {
        setChargeColor(obstacle->getCharge());
        drawCircle(obstacle->getPosition(), obstacle->getRadius(), true);
    }

    void MainCanvas::renderDynamic(DynamicObstacle *obstacle)
    {
        setChargeColor(obstacle->getCharge());
        drawCircle(obstacle->getPosition(), obstacle->getRadius());
    }

    void MainCanvas::drawCircle(b2Vec2 position, float radius, bool filled)
    {
        glPushMatrix();
        glTranslatef(position.x, position.y, 0.0f);

        unsigned int segments;
        if(radius < 1)
        {
            segments = ceil(100 * sqrt(radius));
        }
        else
        {
            segments = ceil(100 * radius);
        }
        if(segments < 10)
        {
            segments = 10;
        }

        if(filled)
        {
    #ifdef GL_MULTISAMPLE
            glEnable(GL_MULTISAMPLE);
    #endif
            glBegin(GL_POLYGON);
        }
        else
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glBegin(GL_LINE_LOOP);
        }
        for(unsigned int i = 0; i < segments; i++)
        {
            glVertex2f(cos(i * 2.0f * M_PI / segments) * radius, sin(i * 2.0f * M_PI / segments) * radius);
        }
        glEnd();

        if(filled)
        {
    #ifdef GL_MULTISAMPLE
            glDisable(GL_MULTISAMPLE);
    #endif
        }
        else
        {
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
        }

        glPopMatrix();
    }

    void MainCanvas::setChargeColor(float charge)
    {
        if(charge > 0)
        {
            glColor3f(1.0f, 0.3f, 0.3f);
        }
        else if(charge < 0)
        {
            glColor3f(0.5f, 0.5f, 1.0f);
        }
        else
        {
            glColor3f(0.5f, 0.5f, 0.5f);
        }
    }
}
