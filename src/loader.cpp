#include "loader.h"

#include <fstream>
#include <sstream>
#include "circularfield.h"
#include "player.h"
#include "staticobstacle.h"
#include "dynamicobstacle.h"

using namespace std;

namespace Charge
{
	Field *Loader::loadField(string filename)
	{
		Field *field = NULL;
		ifstream file(filename.c_str(), ios::in);
		
		string line;
		while(getline(file, line))
		{
			stringstream linestream(line);
			string command;
			linestream >> command;
			
			if(command == "type")
			{
				initFieldType(linestream, &field);
			}
			else if(command == "player")
			{
				addPlayer(linestream, field);
			}
			else if(command == "static")
			{
				addStatic(linestream, field);
			}
			else if(command == "dynamic")
			{
				addDynamic(linestream, field);
			}
		}
		
		return field;
	}
	
	void Loader::initFieldType(stringstream &linestream, Field **field)
	{
		if(*field != NULL)
		{
			return;
		}
		
		string type;
		linestream >> type;
		if(type == "circle")
		{
			float radius;
			linestream >> radius;
			*field = new CircularField(radius);
			(*field)->setCleanup(true);
		}
	}
	
	void Loader::addPlayer(stringstream &linestream, Field *field)
	{
		if(field == NULL)
		{
			return;
		}
		
		unsigned int id;
		float x, y, charge, radius;
		linestream >> id >> x >> y >> charge >> radius;
		field->addActor(new Player(id, b2Vec2(x, y), charge, radius));
	}
	
	void Loader::addStatic(stringstream &linestream, Field *field)
	{
		if(field == NULL)
		{
			return;
		}
		
		float x, y, charge, radius;
		linestream >> x >> y >> charge >> radius;
		field->addActor(new StaticObstacle(b2Vec2(x, y), charge, radius));
	}
	
	void Loader::addDynamic(stringstream &linestream, Field *field)
	{
		if(field == NULL)
		{
			return;
		}
		
		float x, y, charge, radius;
		linestream >> x >> y >> charge >> radius;
		field->addActor(new DynamicObstacle(b2Vec2(x, y), charge, radius));
	}
}