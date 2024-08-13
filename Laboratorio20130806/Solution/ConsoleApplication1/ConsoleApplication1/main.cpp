#include <fstream>
#include <vector>
#include <iostream>
#include "basic_types.h"
#include "pgm.h"
#include "bit.h"
using namespace std;

template <typename T>
istream& read(istream& it,T& val) {
	return it.read(reinterpret_cast<char*>(&val), sizeof(val));
}



void calculate(size_t& variabile,ifstream& it)
{

	bool ascii;
	bool shor;
	bool lon;
	uint16_t type;
	read(it, type);

	type == 1 ? ascii = true : ascii = false;
	type == 3 ? shor = true : shor = false;
	type = 4 ? lon = true : lon = false;

	uint32_t count; // 1
	read(it, count);

	while (count-- > 0) {
		if (shor) {
			short value;
			read(it, value);
			variabile = value;
		}
		else if (lon) {
			long value;
			read(it, value);
			variabile = value;
		}
	}
}
int main(int argc, char *argv[]) {

	char *output_filename = argv[2];

	// Parsing della command line
	if (argc != 3) exit(2);
	ifstream it(argv[1], ios::binary);
	if (!it) { exit(1); }
	// Lettura del TIFF
	uint16_t val;
	uint8_t car;
	read(it, car); //II
	read(it, car);
	read(it, val); // 42
	uint32_t off;	
	read(it, off); 

	it.seekg(off, it.beg);
	uint16_t ndirectory;
	read(it, ndirectory); // Number of directory

	size_t w;
	size_t h;
	size_t bitxpixel;
	size_t compression;
	size_t photmetric;
	size_t x_resolution;
	size_t y_resolution;
	size_t resolutionunit;
	size_t stripOffset; // Sono tanti strip in base a quante row nel nostro caso solo 1 strip
	size_t RowsStip;
	string desciprtion;
	size_t StripBytecount;

	bool ascii=false;
	bool shor=false;
	bool lon=false;
	vector<uint16_t> tags;
	uint16_t tag;
	read(it, tag);
	while (true) {
		switch (tag)
		{
		case 256: {
			calculate(w, it);
			cout << "width: " << w << "\n";
			break;
		
		}
		case 257: {
			calculate(h, it);
			cout << "height: " << h << "\n";
			break;
		}
		case 258: {
			calculate(bitxpixel, it);
			cout << "Bit Per Pixel: " << bitxpixel << "\n";
			break;

		}
		case 259: {
			calculate(compression, it);
			cout << "Compression: " << compression << "\n";
			break;
		}
		case 262: {
			calculate(photmetric, it);
			cout << "Photometric: " << photmetric << "\n";
			break;

		}
		case 273: {
			calculate(stripOffset, it);
			cout << "Strip Per Offset: " << stripOffset << "\n";
			break;

		}
		case 278: {
			calculate(RowsStip, it);
			cout << "Row Strip: " << RowsStip << "\n";
			break;
		}
		case 279: {
			calculate(StripBytecount, it);
			cout << "String Bute count: " << StripBytecount << "\n";
			break;
		}
		default:
			break;
		}
		if (tag == 279) break;
		read(it, tag);
	}



	// Creazione dell'immagine
	image<uint8_t> img(w, h);
	it.seekg(stripOffset, it.beg);
	for (int i = 0; i < img.height(); ++i) {
		for (int j = 0; j < img.width(); ++j) {
			img(j, i) = it.get();
		}
	}

	
	// Inserimento dei dati nell'immagine

	// Salvataggio in PGM
	SavePGM_bin(output_filename, img);
}