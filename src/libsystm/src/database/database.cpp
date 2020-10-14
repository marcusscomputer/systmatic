
#include <database/database.h>

// Set up DB
void systm::database::setUpDatabase(string dbname, vector<string> fieldnames, vector<int> datatypes)
{
	// Set the name
	DatabaseName = dbname;

	// Acquire the field names
	int i;
	for (i=1; i<=fieldnames.size(); i++)
	{ FieldNames.push_back(fieldnames[i-1]); }

	// Acquire the field data types
	for (i=1; i<=datatypes.size(); i++)
	{ RowDatatypes.push_back(datatypes[i-1]); }
}


// Adds a pre-created row
void systm::database::addRow(databaserow row)
{
	// Check if the row has the same amount of fields that we expect there to be
	if (row.RowFields.size() == FieldNames.size())
	{ Rows.push_back(row); }
}


// Deletes the specified row
void systm::database::deleteRow(int rowNumber)
{
	// Create a new vector with the specified row omitted
	vector<databaserow> rows;

	// Go through existing rows and add them to the above vector
	// (except rowNumber)
	int i;
	for (i=1; i<=Rows.size(); i++)
	{
		if (i!=rowNumber)
		{ rows.push_back(Rows[i-1]); }
	}

	// Now clear the real vector
	Rows.clear();

	// And add the rows we got to the real rows
	for (i=1; i<=rows.size(); i++)
	{ Rows.push_back(rows[i-1]); }

	// Clear the tmp vector
	rows.clear();
}


// Updates the content of a specific field in a specific row
void systm::database::updateFieldInRow(int rowNumber, int fieldNumber, string content)
{
	// Make sure the row and field exist
	if (rowNumber <= Rows.size() && fieldNumber <= FieldNames.size())
	{
		// When updating a field, it is probably not a null field
		Rows[rowNumber].RowFields[fieldNumber].NullField = false;

		// It can be either one of these... so make sure they exist
		int newInt = 0;
		short newShort = 0;
		float newFloat = 0.0f;
		double newDouble = 0.0f;
		string newString = content;

		// Find the datatype for the field
		int datatype = RowDatatypes[fieldNumber];

		// Convert the field
		if (datatype == SYSTM_DB_INT)
		{
			newInt = atoi(content.c_str());
			Rows[rowNumber].RowFields[fieldNumber].fieldInt = newInt;
		}
		if (datatype == SYSTM_DB_SHORT)
		{
			newShort = atoi(content.c_str());
			Rows[rowNumber].RowFields[fieldNumber].fieldShort = newShort;
		}
		if (datatype == SYSTM_DB_FLOAT)
		{
			newFloat = atof(content.c_str());
			Rows[rowNumber].RowFields[fieldNumber].fieldFloat = newFloat;
		}
		if (datatype == SYSTM_DB_DOUBLE)
		{
			newDouble = atof(content.c_str());
			Rows[rowNumber].RowFields[fieldNumber].fieldDouble = newDouble;
		}
		if (datatype == SYSTM_DB_STRING)
		{ Rows[rowNumber].RowFields[fieldNumber].fieldString = content; }
	}
}


// Defines a field to be a null field or not
void systm::database::fieldIsNullField(int rowNumber, int fieldNumber, bool isNull)
{
	// Make sure the row and field exist
	if (rowNumber <= Rows.size() && fieldNumber <= FieldNames.size())
	{ Rows[rowNumber].RowFields[fieldNumber].NullField = isNull; }
}


// Creates an empty db to be filled later
void systm::database::createDatabase(string filename, string dbname, vector<string> fieldnames, vector<int> datatypes)
{
    setUpDatabase(dbname, fieldnames, datatypes);
    saveDatabase(filename);
}


// Loads a database from file
void systm::database::loadDatabase(string filename)
{
	// File IO object
	fileio *FileIO = new fileio();

	// Open DB
	FileIO->openFileForReading(filename);

	// Read header and version
	string hdrString = FileIO->readString();
	float hdrVersion = FileIO->readFloat();

	// Check if the header works out
	string hdrStringCheck = "SYSTMDB";
	if (hdrVersion == 1) // String check to be implemented
	{
        // Then this is a valid database
        DatabaseFileName = filename;

		// Read database name
		DatabaseName = FileIO->readString();

		// Get number of field names
		int amountOfFields = FileIO->readInt();

		// Read in the field names themselves
		int i, j;
		for (i=1; i<=amountOfFields; i++)
		{ FieldNames.push_back(FileIO->readString()); }

		// Acquire the data types of each field
		for (i=1; i<=amountOfFields; i++)
		{ RowDatatypes.push_back(FileIO->readInt()); }

		// Number of rows in total
		int numberOfRows = FileIO->readInt();

		// Go through the remainder of the file to acquire the content
		// of each row and their fields
		for (i=1; i<=numberOfRows; i++)
		{
			// The row to be constructed
			databaserow row;

			for (j=1; j<=FieldNames.size(); j++)
			{
				// The field to be added to the row
				databasefield fld;

				// OK find out if the field has content (null field)
				int isNullField = FileIO->readInt();

				// If this is a null field, set the field to be so and add it
				// to the row
				if (isNullField == 1)
				{ fld.NullField = true; row.RowFields.push_back(fld); }

				// If we have data, read according to the data type, set the
				// value into the field, and add the field to the row
				if (isNullField == 0)
				{
					// Not a null field
					fld.NullField = false;

					if (RowDatatypes[j-1] == SYSTM_DB_INT)
					{ int fldval = FileIO->readInt(); fld.fieldInt = fldval; }
					if (RowDatatypes[j-1] == SYSTM_DB_SHORT)
					{ short fldval = FileIO->readInt(); fld.fieldShort = fldval; }
					if (RowDatatypes[j-1] == SYSTM_DB_FLOAT)
					{ float fldval = FileIO->readFloat(); fld.fieldFloat = fldval; }
					if (RowDatatypes[j-1] == SYSTM_DB_DOUBLE)
					{ double fldval = FileIO->readDouble(); fld.fieldDouble = fldval; }
					if (RowDatatypes[j-1] == SYSTM_DB_STRING)
					{ string fldval = FileIO->readString(); fld.fieldString = fldval; }

					// Add the field
					row.RowFields.push_back(fld);
				}
			}

			// Add the completed row
			Rows.push_back(row);
		}

		// Magic number check
		int Magic = (Rows.size() * RowDatatypes.size());
		int MagicInFile = FileIO->readInt();

		// If the magic does not stack up, clear all data
		if (Magic != MagicInFile)
		{ Rows.clear(); }
	}

	// Close the pipe
	FileIO->closeFileForReading();

	// That's it
}


// Saves the database to the specified file. Use when everything is done!
void systm::database::saveDatabase(string filename)
{
	// File IO object// File IO object
	fileio *FileIO = new fileio();

	// Open the DB
	FileIO->openFileForWriting(filename);

	// Write a header
	FileIO->writeString("SYSTMDB");
	// Write a version
	FileIO->writeFloat(1.0);

	// Write database name
	FileIO->writeString(DatabaseName);

	// Write number of field names
	FileIO->writeInt(FieldNames.size());

	// The field names themselves
	int i, j;
	for (i=1; i<=FieldNames.size(); i++)
	{ FileIO->writeString(FieldNames[i-1]); }

	// The datatypes in each field
	for (i=1; i<=RowDatatypes.size(); i++)
	{ FileIO->writeInt(RowDatatypes[i-1]); }

	// Write number of rows
	FileIO->writeInt(Rows.size());

	// Go through the rows and write the content, depending on if the field is
	// a null field or not
	for (i=1; i<=Rows.size(); i++)
	{
		for (j=1; j<=Rows[i-1].RowFields.size(); j++)
		{
			// Write down an int to signalize if the field has data or not
			int isNull = 0;
			if (Rows[i-1].RowFields[j-1].NullField == true)  { isNull = 1; }
			if (Rows[i-1].RowFields[j-1].NullField == false) { isNull = 0; }
			FileIO->writeInt(isNull);

			// Write actual data if field is not null
			if (isNull == 0)
			{
				if (RowDatatypes[j-1] == SYSTM_DB_INT) // int
				{ FileIO->writeInt(Rows[i-1].RowFields[j-1].fieldInt); }
				if (RowDatatypes[j-1] == SYSTM_DB_SHORT) // short
				{ FileIO->writeInt(Rows[i-1].RowFields[j-1].fieldShort); }
				if (RowDatatypes[j-1] == SYSTM_DB_FLOAT) // float
				{ FileIO->writeFloat(Rows[i-1].RowFields[j-1].fieldFloat); }
				if (RowDatatypes[j-1] == SYSTM_DB_DOUBLE) // double
				{ FileIO->writeDouble(Rows[i-1].RowFields[j-1].fieldDouble); }
				if (RowDatatypes[j-1] == SYSTM_DB_STRING) // string
				{ FileIO->writeString(Rows[i-1].RowFields[j-1].fieldString); }
			}
		}
	}

	// Write a magic number for verification
	int Magic = (Rows.size() * RowDatatypes.size());
	FileIO->writeInt(Magic);


	// Close DB
	FileIO->closeFileForWriting();

	// That's it
}


void systm::database::closeDatabase(bool save)
{
    if (save == true)
    { saveDatabase(DatabaseFileName); }

    Rows.clear();
}


vector<databaserow> systm::database::findRowsWithValueInColumn(string value, string colname)
{
    // The result
    vector<databaserow> rows;

    // The column index to be searched
    int colindex = -1;
    for (int i=0; i<FieldNames.size(); i++)
    {
        if (FieldNames[i].compare(colname) == 0)
        { colindex = i; break; }
    }

    // Proceed if we have a column
    if (colindex != -1)
    {
        // Look for results depending on datatype
        for (int i=0; i<Rows.size(); i++)
        {
            bool push=false;

            // The field to check
            databasefield fld = Rows[i].RowFields[colindex];
            
            if (RowDatatypes[colindex] == SYSTM_DB_INT)
            {
                int needle = stoi(value);
                if (fld.NullField == false && fld.fieldInt == needle)
                { push = true; }
            }
			if (RowDatatypes[colindex] == SYSTM_DB_SHORT)
			{
				int needle = stoi(value);
				if (fld.NullField == false && fld.fieldShort == needle)
				{ push = true; }
			}
            if (RowDatatypes[colindex] == SYSTM_DB_FLOAT)
            {
                float needle = stof(value);
                if (fld.NullField == false && fld.fieldFloat == needle)
                { push = true; }
            }
            if (RowDatatypes[colindex] == SYSTM_DB_DOUBLE)
            {
                double needle = stod(value);
                if (fld.NullField == false && fld.fieldDouble == needle)
                { push = true; }
            }
            if (RowDatatypes[colindex] == SYSTM_DB_STRING)
            {
                if (fld.NullField == false && fld.fieldString.compare(value) == 0)
                { push = true; }
            }

            if (push == true) { rows.push_back(Rows[i]); }
        }
    }
    
    // Return list
    return rows;
}


vector<databaserow> systm::database::findRowsWithStringInColumn(string value, string colname)
{
    // The result
    vector<databaserow> rows;

    // The column index to be searched
    int colindex = -1;
    for (int i=0; i<FieldNames.size(); i++)
    {
        if (FieldNames[i].compare(colname) == 0)
        { colindex = i; break; }
    }

    // Proceed if we have a column
    if (colindex != -1)
    {
        // Look for results depending on datatype
        for (int i=0; i<Rows.size(); i++)
        {
            bool push=false;

            // The field to check
            databasefield fld = Rows[i].RowFields[colindex];
            
            if (RowDatatypes[colindex] == SYSTM_DB_STRING)
            {
                if (fld.NullField == false && fld.fieldString.find(value) != string::npos)
                { push = true; }
            }

            if (push == true) { rows.push_back(Rows[i]); }
        }
    }
    
    // Return list
    return rows;
}


int systm::database::numberOfRows()
{ return Rows.size(); }
