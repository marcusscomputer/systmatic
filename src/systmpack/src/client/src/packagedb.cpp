
#include <packagedb.h>

void systmatic::packagedb::createdb()
{
	// The field names for this database
	std::vector<std::string> fieldnames;

	// The single strings for the fields
	std::string f1 = "id";
	std::string f2 = "name";
	std::string f3 = "description";
	std::string f4 = "version";
	std::string f5 = "dependencies";
	fieldnames.push_back(f1);
	fieldnames.push_back(f2);
	fieldnames.push_back(f3);
	fieldnames.push_back(f4);
	fieldnames.push_back(f5);

	std::vector<int> fieldtypes;
	int t1 = SYSTM_DB_INT;
	int t2 = SYSTM_DB_STRING;
	int t3 = SYSTM_DB_STRING;
	int t4 = SYSTM_DB_FLOAT;
	fieldtypes.push_back(t1);
	fieldtypes.push_back(t2);
	fieldtypes.push_back(t3);
	fieldtypes.push_back(t4);

	// Create an empty database with this field and row
	// data inside it
	_packagedb.createDatabase("./packagedb.systmdb", "systmaticLocalPackages", fieldnames, fieldtypes);
}
