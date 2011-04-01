#include <iostream>
#include <cstdio>
#include <cmath>
#include <GL/glut.h>

#include "field.h"
#include "staticobstacle.h"
#include "dynamicobstacle.h"
#include "player.h"
#include "circularfield.h"

using namespace std;
using namespace Charge;

CircularField *field;
Player *player1, *player2;

void SetupRenderer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void ChangeSize(int width, int height)
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

void UpdateScene(int timestep)
{
	field->step(timestep / 1000.0f);
	glutPostRedisplay();
	glutTimerFunc(timestep, UpdateScene, timestep);
}

void DrawCircle(b2Vec2 position, float radius, bool filled = false)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, 0.0f);

	unsigned int segments = ceil(100 * radius);
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
		glBegin(GL_LINE_LOOP);
	}
	for(unsigned int i = 0; i < segments; i++)
	{
		glVertex2f(cos(i * 2.0f * M_PI / segments) * radius, sin(i * 2.0f * M_PI / segments) * radius);
	}
	glEnd();

	glPopMatrix();
}

void HandleInput(unsigned char key, int x, int y)
{
	if(key == 'a')
	{
		player1->invertCharge();
	}
	else if(key == 'l')
	{
		player2->invertCharge();
	}
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw field
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawCircle(b2Vec2(0.0f, 0.0f), field->getRadius());

	// Draw actors
	vector<Actor*> actors = field->getActors();
	vector<Actor*>::iterator iter;
	for(iter = actors.begin(); iter != actors.end(); iter++)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		DrawCircle((*iter)->getPosition(), (*iter)->getRadius());
		if((*iter)->getCharge() > 0)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else
		{
			glColor3f(0.0f, 0.0f, 1.0f);
		}
		DrawCircle((*iter)->getPosition(), (*iter)->getRadius() / 2, true);
	}

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	StaticObstacle s1(b2Vec2(1.0f, 0.0f), 1.0f);
	StaticObstacle s2(b2Vec2(-1.0f, 0.0f), -1.0f);
	DynamicObstacle d1(b2Vec2(0.0f, 0.0f), -1.0f);
	Player p1(b2Vec2(0.0f, 1.0f));
	Player p2(b2Vec2(0.0f, -1.0f), -CHARGE_PLAYER_DEFAULT_CHARGE);
	player1 = &p1;
	player2 = &p2;

	field = new CircularField(2.0f);
	field->addActor(&s1);
	field->addActor(&s2);
	//field->addActor(&d1);
	field->addActor(&p1);
	field->addActor(&p2);

	// Setup OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Charge");
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(10, UpdateScene, 10);
	glutKeyboardFunc(HandleInput);
	glutDisplayFunc(RenderScene);

	SetupRenderer();

	glutMainLoop();

	delete field;
	return 0;
}