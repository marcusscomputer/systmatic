
// This handles reading from, and writing to, binary files. Ultimately it is a
// utalitarian class that is used in quite a number of operations within
// the Reason Engine

#ifndef SYSTM_FILEIO
#define SYSTM_FILEIO

#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <zlib.h>

namespace systm
{
	class fileio
	{
	public:
		// The file to write out to
		std::ofstream out;
		// The file to read in from
		std::ifstream in;

		// Check if a file exists prior to opening it or writing to it
		bool doesFileExist(std::string filename);

		// Compresses a file
		void compressFile(std::string source, std::string dest);
		// Decompresses a file
		void decompressFile(std::string source, std::string dest);

		// Open a binary file for writing
		void openFileForWriting(std::string fn);

		// Open a binary file for reading
		void openFileForReading(std::string fn);
		
		// Reads a plain text file back to back and provides everything in a
		// std::string object
		std::string readPlainText(std::string fn);

		// Jumps to the specified position in the file opened for reading. We mostly
		// use this to quickly jump to positions in an octree file and read only
		// what is needed for rendering.
		void jumpToPositionInFile(int seekpos);

		// Jumps back to the very beginning of the file
		void jumpToBeginningOfFile();

		// Close the file to write
		void closeFileForWriting();
		// And for reading
		void closeFileForReading();

		// Writes all kinds of things we need
		void writeShort(short s);
		void writeInt(int i);
		void writeUInt(unsigned int i);
		void writeFloat(float f);
		void writeDouble(double d);
		void writeString(std::string str);
		void writeStringAsPlainText(std::string str);
		void writeBinary(unsigned char c);

		// Reads back the same data types
		short readShort();
		int readInt();
		float readFloat();
		double readDouble();
		std::string readString();

		// Deletes the specified file
		void deleteFile(std::string filename);

		// Get current position in file stream
		int CurrentStreamPosition();
	};
}
#endif
