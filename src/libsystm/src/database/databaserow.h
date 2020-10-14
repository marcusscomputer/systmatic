
// Describes one single row of our database format.

#ifndef SYSTM_DATABASEROW
#define SYSTM_DATABASEROW

#include <cstdlib>
#include <vector>
#include <database/databasefield.h>

using namespace std;
using namespace systm;

namespace systm
{
	class databaserow
	{
	public:
		// The single fields in this row
		vector <databasefield> RowFields;

		// Defines a specific field to be null or not
		void FieldIsNullField(int fld, bool isNull);

		// Adds a field
		void AddField(databasefield fld); 
	};
}

#endif
