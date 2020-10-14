
// Describes one single database field of our own custom format.

#ifndef _REASON_DATABASEFIELD
#define _REASON_DATABASEFIELD

#include <cstdlib>
#include <string>

using namespace std;

namespace systm
{
	class databasefield
	{
	public:
		// Whether or not this is a null field
		bool NullField;

		// The content type
		int ContentType;

		// The single content types
		int fieldInt;
		short fieldShort;
		float fieldFloat;
		double fieldDouble;
		string fieldString;
	};
}

#endif
