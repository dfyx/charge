#ifndef CHARGE_LOADER_H
#define CHARGE_LOADER_H

#include "field.h"
#include <string>
#include <sstream>

namespace Charge
{
	class Loader
	{
	public:
		Field *loadField(std::string filename);
	protected:
		void initFieldType(std::stringstream &linestream, Field **field);
		void addPlayer(std::stringstream &linestream, Field *field);
		void addStatic(std::stringstream &linestream, Field *field);
		void addDynamic(std::stringstream &linestream, Field *field);
	};
};

#endif // CHARGE_LOADER_H