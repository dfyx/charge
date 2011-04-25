#include <iostream>
#include <cstdio>
#include <cmath>
#include <glut_wrap.h>

#include "field.h"
#include "staticobstacle.h"
#include "dynamicobstacle.h"
#include "player.h"
#include "circularfield.h"
#include "loader.h"

using namespace std;
using namespace Charge;

CircularField *field;
Player *player1, *player2;

static float playerColors[][3] =
{{0.5f, 0.5f, 0.5f},
 {1.0f, 0.0f, 0.0f},
 {0.0f, 0.0f, 1.0f},
 {0.0f, 1.0f, 0.0f},
 {1.0f, 1.0f, 0.0f}};

void SetupRenderer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
#ifdef GL_MULTISAMLE
	glEnable(GL_MULTISAMPLE);
#endif
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
#endif GL_MULTISAMPLE
	}
	else
	{
		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);
	}

	glPopMatrix();
}

void HandleInput(unsigned char key, int x, int y)
{
	if(key == 'a')
	{
		field->reactToPlayer(1);
	}
	else if(key == 'l')
	{
		field->reactToPlayer(2);
	}
}

void setChargeColor(float charge)
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

void renderPlayer(Player *player)
{
	setChargeColor(player->getCharge());
	DrawCircle(player->getPosition(), player->getRadius());
	
	glColor3fv(playerColors[player->getOwner()]);
	DrawCircle(player->getPosition(), player->getRadius() / 2, true);
}

void renderStatic(StaticObstacle *obstacle)
{
	setChargeColor(obstacle->getCharge());
	DrawCircle(obstacle->getPosition(), obstacle->getRadius(), true);
}

void renderDynamic(DynamicObstacle *obstacle)
{
	setChargeColor(obstacle->getCharge());
	DrawCircle(obstacle->getPosition(), obstacle->getRadius());
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

	glutSwapBuffers();
}

int main(int argc, char** argv)
{	
	Loader loader;
	field = (CircularField*) loader.loadField("test.charge");

	// Setup OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
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