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

        cameraTimer = new QTime();
    }

    MainCanvas::~MainCanvas()
    {
        delete playerModel;
        delete timer;
    }

    void MainCanvas::initializeGL()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    #ifdef GL_MULTISAMLE
        glEnable(GL_MULTISAMPLE);
    #endif
        timer->start(TIMESTEP);
        cameraTimer->start();

        glewInit();

        playerModel = new Model("data/models/player.obj");
    }

    void MainCanvas::resizeGL(int width, int height)
    {
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //float radius = field->getRadius() * 1.2f;
        gluPerspective(45.0f, double(width) / double(height), 1.0, 100.0f);
    }

    void MainCanvas::paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);
        glShadeModel(GL_SMOOTH);


        // Offset camera

        glTranslatef(0.0f, 0.0f, -field->getRadius() * 3.0f);
        glRotatef(50.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(cameraTimer->elapsed() / 500.0f, 0.0f, 1.0f, 0.0f);

        GLfloat specular[] = {1.0f, 1.0f, 1.0f , 1.0f};
        float lightpos[] = {0.5f, 0.0f, 2.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, specular);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);


        // Draw field
        glColor3f(0.1f, 0.1f, 0.1f);
        GLUquadricObj *quadric = gluNewQuadric();
        gluQuadricDrawStyle(quadric, GLU_FILL);

        glPushMatrix();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        gluCylinder(quadric, field->getRadius() * 1.1f, field->getRadius() * 1.1f, 4.0f, 40, 1);
        gluDisk(quadric, 0.0f, field->getRadius() * 1.1f, 40, 1);
        glPopMatrix();

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
        //setChargeColor(player->getCharge());
        //drawCircle(player->getPosition(), player->getRadius());

        glColor3fv(playerColors[player->getOwner()]);
        glPushMatrix();
        glTranslatef(player->getPosition().x, 0.0f, player->getPosition().y);
        float radius = player->getRadius();
        glScalef(radius, radius, radius);
        playerModel->draw();

        // Temporary charge indicator
        GLUquadricObj *quadric = gluNewQuadric();
        gluQuadricDrawStyle(quadric, GLU_FILL);

        // Double tessellation for slices or it would look really ugly
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glTranslatef(0.0f, 1.5f, 0.0f);
        setChargeColor(player->getCharge());
        gluSphere(quadric, 0.4, 16, 8);
        glPopAttrib();
        gluDeleteQuadric(quadric);
        glPopMatrix();
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
        glTranslatef(position.x, 0.01f, position.y);

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
            glVertex3f(cos(i * 2.0f * M_PI / segments) * radius, 0.0f, sin(i * 2.0f * M_PI / segments) * radius);
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
            glColor3f(1.0f, 0.5f, 0.5f);
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
