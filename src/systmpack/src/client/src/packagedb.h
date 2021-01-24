
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
		// The database consists of two database files: the package database
		// itself which contains info such as the ID, name, description,
		// dependencies and so on. The second database contains the files
		// inside each package - ID of package and one file in each row with
		// absolute paths.
		// Does not take any arguments.
		void createdb();

	private:
		// The database object to open, read and store databases
		systm::database _packagedb;
		// The database containing the file names for each package
		systm::database _pkgfilesdb;
	};
}

#endif
