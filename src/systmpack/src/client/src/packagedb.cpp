
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
	int t5 = SYSTM_DB_ARRAY;
	fieldtypes.push_back(t1);
	fieldtypes.push_back(t2);
	fieldtypes.push_back(t3);
	fieldtypes.push_back(t4);
	fieldtypes.push_back(t5);

	// Create an empty database with this field and row
	// data inside it
	_packagedb.createDatabase("./systmpackdb", "systmatic Package DB", fieldnames, fieldtypes);
	
	// -------------------------------------------------------------------------
	
	// The field names for the file path database
	std::vector<std::string> pathdbNames;
	
	// Names for the file path database
	std::string fpdb1 = "id"; 		// ID of the package
	std::string fpdb2 = "file";		// A single file path, absolute
	pathdbNames.push_back(fpdb1);
	pathdbNames.push_back(fpdb2);
	
	// And the types for this DB
	std::vector<int> fpdb_types;
	int fptype1 = SYSTM_DB_INT;
	int fptype2 = SYSTM_DB_STRING;
	fpdb_types.push_back(fptype1);
	fpdb_types.push_back(fptype2);
	
	// And now create this empty database with the headers
	_pkgfilesdb.createDatabase("./systmpackfiles", "systmatic Package File Paths", pathdbNames, fpdb_types);	
}
