#include "maincanvas.h"
#include "player.h"
#include "dynamicobstacle.h"
#include "staticobstacle.h"
#include <QMessageBox>

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
        glDeleteTextures(1, &diffuseBuffer);
        glDeleteTextures(1, &specularBuffer);
        glDeleteTextures(1, &positionBuffer);
        glDeleteTextures(1, &normalBuffer);
        glDeleteRenderbuffers(1, &depthBuffer);
        glDeleteFramebuffers(1, &frameBuffer);

        delete playerModel;
        delete dynamicModel;
        delete staticModel;
        delete fieldModel;

        delete defaultShaderProgram;
        delete playerShaderProgram;
        delete lightShaderProgram;
        delete ambientShaderProgram;

        delete timer;
    }

    void MainCanvas::initializeGL()
    {
        timer->start(TIMESTEP);
        cameraTimer->start();

        glewInit();

        // Load models
        playerModel = new Model("data/models/player.obj");
        playerModel->loadTexture("diffuseTexture", "data/textures/player/diffuse.png");
        playerModel->loadTexture("specularTexture", "data/textures/player/specular.png");

        dynamicModel = new Model("data/models/dynamic.obj");
        dynamicModel->loadTexture("diffuseTexture", "data/textures/dynamic/diffuse.png");
        dynamicModel->loadTexture("diffuseTexture", "data/textures/dynamic/specular.png");

        staticModel = new Model("data/models/static.obj");
        staticModel->loadTexture("diffuseTexture", "data/textures/static/diffuse.png");
        staticModel->loadTexture("diffuseTexture", "data/textures/static/specular.png");

        fieldModel = new Model("data/models/field.obj");
        fieldModel->loadTexture("diffuseTexture", "data/textures/field/diffuse.png");
        fieldModel->loadTexture("diffuseTexture", "data/textures/field/specular.png");

        // Load shaders
        defaultShaderProgram = new QGLShaderProgram(context());
        defaultShaderProgram->addShaderFromSourceFile(QGLShader::Vertex, "data/shaders/vertex/default.glsl");
        defaultShaderProgram->addShaderFromSourceFile(QGLShader::Fragment, "data/shaders/fragment/default.glsl");
        defaultShaderProgram->link();

        playerShaderProgram = new QGLShaderProgram(context());
        playerShaderProgram->addShaderFromSourceFile(QGLShader::Vertex, "data/shaders/vertex/default.glsl");
        playerShaderProgram->addShaderFromSourceFile(QGLShader::Fragment, "data/shaders/fragment/player.glsl");
        playerShaderProgram->link();

        lightShaderProgram = new QGLShaderProgram(context());
        lightShaderProgram->addShaderFromSourceFile(QGLShader::Vertex, "data/shaders/vertex/light.glsl");
        lightShaderProgram->addShaderFromSourceFile(QGLShader::Fragment, "data/shaders/fragment/light.glsl");
        lightShaderProgram->link();

        ambientShaderProgram = new QGLShaderProgram(context());
        ambientShaderProgram->addShaderFromSourceFile(QGLShader::Vertex, "data/shaders/vertex/ambient.glsl");
        ambientShaderProgram->addShaderFromSourceFile(QGLShader::Fragment, "data/shaders/fragment/ambient.glsl");
        ambientShaderProgram->link();

        // Setup the frame buffer
        glGenFramebuffers(1, &frameBuffer);
        glGenRenderbuffers(1, &depthBuffer);
        glGenTextures(1, &diffuseBuffer);
        glGenTextures(1, &specularBuffer);
        glGenTextures(1, &positionBuffer);
        glGenTextures(1, &normalBuffer);

        setupFrameBuffer();
    }

    void MainCanvas::setupFrameBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        // Add depth buffer
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width(), height());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Setup diffuse buffer
        glBindTexture(GL_TEXTURE_2D, diffuseBuffer);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuseBuffer, 0);

        // Setup specular buffer
        glBindTexture(GL_TEXTURE_2D, specularBuffer);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, specularBuffer, 0);

        // Setup position buffer
        glGenTextures(1, &positionBuffer);
        glBindTexture(GL_TEXTURE_2D, positionBuffer);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width(), height(), 0, GL_RGBA, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, positionBuffer, 0);

        // Setup normal buffer
        glGenTextures(1, &normalBuffer);
        glBindTexture(GL_TEXTURE_2D, normalBuffer);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width(), height(), 0, GL_RGB, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, normalBuffer, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(4, drawBuffers);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Frame buffer error");
            msgBox.setText("Could not initialize frame buffer");
            msgBox.exec();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void MainCanvas::setupProjectionCamera(int width, int height)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, double(width) / double(height), 1.0, 100.0f);
    }

    void MainCanvas::resizeGL(int width, int height)
    {
        glViewport(0, 0, width, height);

        setupProjectionCamera(width, height);
        setupFrameBuffer();
    }

    void MainCanvas::paintGL()
    {
        lights.clear();
        geometryRenderPass();

        shadingRenderPass();
    }

    void MainCanvas::geometryRenderPass()
    {
        setupProjectionCamera(width(), height());

        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

        // Add main light
        lights.push_back(Light(QVector3D(0.0f, 1.0f, 0.0f), Qt::white, 0.5f));

        // Draw field
        glColor3f(0.1f, 0.1f, 0.1f);

        glPushMatrix();
        glScalef(field->getRadius(), field->getRadius(), field->getRadius());
        defaultShaderProgram->bind();
        fieldModel->draw(defaultShaderProgram);
        defaultShaderProgram->release();
        glPopMatrix();

        // Draw actors
        QList<Actor*> actors = field->getActors();
        QList<Actor*>::iterator iter;
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
                default:
                    break;
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void MainCanvas::shadingRenderPass()
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);

        // Bind buffers
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularBuffer);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, positionBuffer);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, normalBuffer);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDepthMask(GL_FALSE);

        // Render lights
        lightShaderProgram->bind();
        setBufferUniforms(lightShaderProgram);
        lightShaderProgram->setUniformValue("screenSize", width(), height());

        QList<Light>::iterator iter;
        for(iter = lights.begin(); iter != lights.end(); iter++)
        {
            (*iter).draw(lightShaderProgram);
        }
        lightShaderProgram->release();

        // Add ambient light
        ambientShaderProgram->bind();
        setBufferUniforms(ambientShaderProgram);
        ambientShaderProgram->setUniformValue("ambientColor", 0.05f, 0.05f, 0.05f, 1.0f);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, 0.0f);
        glEnd();
        ambientShaderProgram->release();

        glBindTexture(GL_TEXTURE_2D, 0);

        glPopAttrib();
    }

    void MainCanvas::setBufferUniforms(QGLShaderProgram *shaderProgram)
    {
        shaderProgram->setUniformValue("diffuseBuffer", 0);
        shaderProgram->setUniformValue("specularBuffer", 1);
        shaderProgram->setUniformValue("positionBuffer", 2);
        shaderProgram->setUniformValue("normalBuffer", 3);
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

        playerShaderProgram->bind();
        playerModel->draw(playerShaderProgram);
        playerShaderProgram->release();

        // Charge light
        lights.push_back(Light(QVector3D(0.0f, 1.5f, 0.0f), getChargeColor(player->getCharge()), fabs(player->getCharge())));

        /*// Temporary charge indicator
        GLUquadricObj *quadric = gluNewQuadric();
        gluQuadricDrawStyle(quadric, GLU_FILL);

        // Double tessellation for slices or it would look really ugly
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glTranslatef(0.0f, 1.5f, 0.0f);
        gluSphere(quadric, 0.4, 16, 8);
        glPopAttrib();
        gluDeleteQuadric(quadric);*/
        glPopMatrix();
    }

    void MainCanvas::renderDynamic(DynamicObstacle *obstacle)
    {
        glPushMatrix();
        glTranslatef(obstacle->getPosition().x, 0.0f, obstacle->getPosition().y);
        float radius = obstacle->getRadius();
        glScalef(radius, radius, radius);

        defaultShaderProgram->bind();
        dynamicModel->draw(defaultShaderProgram);
        defaultShaderProgram->release();

        if(obstacle->getCharge() != 0)
        {
            // Charge light
            lights.push_back(Light(QVector3D(0.0f, 1.5f, 0.0f), getChargeColor(obstacle->getCharge()), fabs(obstacle->getCharge())));

            /*// Temporary charge indicator
            GLUquadricObj *quadric = gluNewQuadric();
            gluQuadricDrawStyle(quadric, GLU_FILL);

            // Double tessellation for slices or it would look really ugly
            glPushAttrib(GL_ENABLE_BIT);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 1.5f, 0.0f);
            gluSphere(quadric, fabs(obstacle->getCharge()), 16, 8);
            glPopAttrib();
            gluDeleteQuadric(quadric);*/
        }

        glPopMatrix();
    }

    void MainCanvas::renderStatic(StaticObstacle *obstacle)
    {
        glPushMatrix();
        glTranslatef(obstacle->getPosition().x, 0.0f, obstacle->getPosition().y);
        float radius = obstacle->getRadius();
        glScalef(radius, radius, radius);

        defaultShaderProgram->bind();
        staticModel->draw(defaultShaderProgram);
        defaultShaderProgram->release();

        if(obstacle->getCharge() != 0)
        {
            // Charge light
            lights.push_back(Light(QVector3D(0.0f, 1.5f, 0.0f), getChargeColor(obstacle->getCharge()), fabs(obstacle->getCharge())));

            /*// Temporary charge indicator
            GLUquadricObj *quadric = gluNewQuadric();
            gluQuadricDrawStyle(quadric, GLU_FILL);

            // Double tessellation for slices or it would look really ugly
            glPushAttrib(GL_ENABLE_BIT);
            glDisable(GL_LIGHTING);
            glTranslatef(0.0f, 1.5f, 0.0f);
            gluSphere(quadric, fabs(obstacle->getCharge()), 16, 8);
            glPopAttrib();
            gluDeleteQuadric(quadric);*/
        }

        glPopMatrix();
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
        }
        else
        {
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
        }

        glPopMatrix();
    }

    QColor MainCanvas::getChargeColor(float charge)
    {
        QColor result;
        if(charge > 0)
        {
            result.setRgbF(1.0f, 0.5f, 0.5f);
        }
        else if(charge < 0)
        {
            result.setRgbF(0.5f, 0.5f, 1.0f);
        }
        else
        {
            result.setRgbF(0.5f, 0.5f, 0.5f);
        }

        return result;
    }
}
