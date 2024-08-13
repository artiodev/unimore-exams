#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "pgm.h"

int compress(const string& sInputFileName, const string& sOutputFileName) {
	image<byte> img;
	if (!readP2(sInputFileName, img) && !readP5(sInputFileName, img)) {
		cout << "Error: cannot open file \"" << sInputFileName << "\"\n";
		return EXIT_FAILURE;
	}

	// TO DO

	return EXIT_SUCCESS;
}

int decompress(const string& sInputFileName, const string& sPrefix) {
	ifstream is(sInputFileName, ios::binary);
	if (!is) {
		cout << "Error: cannot open file \"" << sInputFileName << "\"\n";
		return EXIT_FAILURE;
	}

	// TO DO

	unsigned int w = 10, h = 10; // Valori a caso! Va cambiato!
	image<byte> img(w,h);

	// TO DO

	string complete_output_name = sPrefix+"qualcosa.pgm";  // Valori a caso! Va cambiato!
	writeP5(complete_output_name, img);

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

	// TO DO - argomenti della linea di comando
	
	string input = "esempio.pgm", output = "esempio.mlt", prefix = "image"; // Valori a caso! Va cambiato!

	compress(input, output);

	decompress(output, prefix);
}