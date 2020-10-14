
#include <database/database.h>
#include <database/databasedefines.h>
#include <string>
#include <vector>

#ifndef SYSTMPACK_PACKAGEDB
#define SYSTMPACK_PACKAGEDB

namespace systmatic
{
	class packagedb
	{
	public:
		// Creates a empty new local package database if it does not exist.
		// Does not take any arguments.
		void createdb();

	private:
		// The database object to open, read and store databases
		systm::database _packagedb;
	};
}

#endif
