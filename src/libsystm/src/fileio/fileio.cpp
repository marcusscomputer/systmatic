
#include <fileio/fileio.h>

bool systm::fileio::doesFileExist(std::string filename)
{
	std::ifstream f(filename.c_str());
    return f.good();
}

void systm::fileio::compressFile(std::string source, std::string dest)
{
    FILE *infile = fopen(source.c_str(), "rb");
    gzFile outfile = gzopen(dest.c_str(), "wb");
    //if (!infile || !outfile) return -1;

    char inbuffer[128];
    int num_read = 0;
    unsigned long total_read = 0, total_wrote = 0;
    while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {
       total_read += num_read;
       gzwrite(outfile, inbuffer, num_read);
    }
    fclose(infile);
    gzclose(outfile);
}

void systm::fileio::decompressFile(std::string source, std::string dest)
{
	gzFile inFileZ = gzopen(source.c_str(), "rb");
	if (inFileZ == NULL) {
		printf("Error: Failed to gzopen %s\n", source.c_str());
		exit(0);
	}
	unsigned char unzipBuffer[8192];
	unsigned int unzippedBytes;
	std::vector<unsigned char> unzippedData;
	while (true) {
		unzippedBytes = gzread(inFileZ, unzipBuffer, 8192);
		if (unzippedBytes > 0) {
			unzippedData.insert(unzippedData.end(), unzipBuffer, unzipBuffer + unzippedBytes);
		} else {
			break;
		}
	}
	gzclose(inFileZ);

	// Write the decompressed data
	openFileForWriting(dest);
	for (int i=0; i<unzippedData.size(); i++)
	{ writeBinary(unzippedData[i]); }
	closeFileForWriting();
}

void systm::fileio::openFileForWriting(std::string fn)
{ out.open (fn, std::ofstream::binary | std::ios::out); }

void systm::fileio::openFileForReading(std::string fn)
{ in.open (fn, std::ifstream::binary | std::ios::in); }

std::string systm::fileio::readPlainText(std::string fn)
{
	in.open(fn);
	std::string txt;
	while (!in.eof()) { in >> txt; }
	closeFileForReading();
	return txt;
}

void systm::fileio::jumpToPositionInFile(int seekpos)
{ in.seekg(0, in.beg+seekpos); }

void systm::fileio::jumpToBeginningOfFile()
{ in.seekg(0, in.beg); }

void systm::fileio::closeFileForWriting()
{ out.close(); }

void systm::fileio::closeFileForReading()
{ in.close(); }

void systm::fileio::writeShort(short s) { out.write((char *)&s, sizeof(short)); }
void systm::fileio::writeInt(int i) { out.write ((char *)&i, sizeof(int)); }
void systm::fileio::writeUInt(unsigned int i) { out.write ((char *)&i, sizeof(unsigned int)); }
void systm::fileio::writeFloat(float f) { out.write ((char *)&f, sizeof(float)); }
void systm::fileio::writeDouble(double d) { out.write ((char *)&d, sizeof(double)); }

void systm::fileio::writeString(std::string str)
{
	// Length of string
	int len = str.length();
	// Write length
	writeInt(len);
	// Write string itself
	//out.write( str.c_str(), str.size() );
	out.write(str.c_str(), len);
}

void systm::fileio::writeStringAsPlainText(std::string str)
{ int len = str.length(); out.write(str.c_str(), len); }

void systm::fileio::writeBinary(unsigned char c)
{ out.write(c, sizeof(unsigned char)); }

short systm::fileio::readShort()
{
	short val;
	in.read((char*)&val, sizeof(short));
}

int systm::fileio::readInt()
{
	int val;
	in.read((char*)&val, sizeof(int));
	return val;
}

float systm::fileio::readFloat()
{
	float val;
	in.read((char*)&val, sizeof(float));
	return val;
}

double systm::fileio::readDouble()
{
	double val;
	in.read((char*)&val, sizeof(double));
	return val;
}

std::string systm::fileio::readString()
{
	// Read string length:
	int len = readInt();
	// Create buffer:
	char* temp = new char[len];
	// Read string:
	in.read(temp, len);
	std::string res = temp;
	// Return it
	return res;
}

void systm::fileio::deleteFile(std::string filename)
{ remove(filename.c_str()); }


int systm::fileio::CurrentStreamPosition()
{ return in.tellg(); }
