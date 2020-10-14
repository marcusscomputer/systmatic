
// This class represents a custom database format used in the Reason Engine. It
// is small and quite simple to use. This format is used for a number of things,
// mostly for models.

#ifndef RE_DATABASE
#define RE_DATABASE

#include <cstdlib>
#include <string>
#include <vector>
#include <fileio/fileio.h>
#include <database/databaserow.h>
#include <database/databasedefines.h>

using namespace std;
using namespace systm;

namespace systm
{
	class database
	{
	public:
		// Name of the database
		string DatabaseName;
		// Name of all fields in this database
		vector<string> FieldNames;
		// The database rows
		vector<databaserow> Rows;
		// Definitions of the single datatypes in each field
		vector<int> RowDatatypes;

		// The filename from which this database came from, if opened from file
		string DatabaseFileName;

		// Sets up this database with a name, the field names, and the data
		// types each row should hold
		void setUpDatabase(string dbname, vector<string> fieldnames, vector<int> datatypes);

		// Adds a row to this database
		void addRow(databaserow row);

		// Deletes the row of the specified index
		void deleteRow(int rowNumber);

		// Updates the content of a specific field, in the specified row. For
		// simplycity, the content is a string which we process to the correct
		// field type for you.
		void updateFieldInRow(int rowNumber, int fieldNumber, string content);

		// Defines if a field has data or not
		void fieldIsNullField(int rowNumber, int fieldNumber, bool isNull);

		// Loads a database from a file
		void loadDatabase(string filename);
		// Creates an empty database
		void createDatabase(string filename, string dbname, vector<string> fieldnames, vector<int> datatypes);
		// Saves the database to a file
		void saveDatabase(string filename);

		// Clears the database in RAM, basically frees up used space and lets
		// you load another database with the same object
		void closeDatabase(bool save);

		// Acquires the amount of rows in DB
		int numberOfRows();

		// Find something in all rows, in the specified column. Looks for the
		// EXACT value match. The value can also be an int, float or double, but
		// must be encoded as a string, like "30.99", "64", or "19.645773373"
		vector<databaserow> findRowsWithValueInColumn(string value, string colname);
		
		// Find something in all rows, in the specified column. Returns all
		// rows where the specified string occurs, but not exact word match
		vector<databaserow> findRowsWithStringInColumn(string value, string colname);
	};
}

#endif
