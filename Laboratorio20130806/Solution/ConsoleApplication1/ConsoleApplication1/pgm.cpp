#include "pgm.h"

#include <fstream>
using namespace std;

void PGM_skip_whitespace(istream& is) {
	while (is.peek() == ' ' || is.peek() == '\t' || is.peek() == '\n' || is.peek() == '\r')
		is.get();
}
void PGM_skip_comment(istream& is) {
	string line;
	while (true) {
		PGM_skip_whitespace(is);
		if (is.peek() != '#')
			return;
		getline(is, line);
	}
}
bool LoadPGM_bin(const string& sFileName, image<uint8_t>& img) {
	ifstream f(sFileName.c_str(), ifstream::binary);
	if (!f)
		return false;

	string sMagicNumber;
	f >> sMagicNumber;

	if (sMagicNumber != "P5")
		return false;

	unsigned w, h, maxval;
	PGM_skip_comment(f);
	f >> w;
	PGM_skip_comment(f);
	f >> h;
	PGM_skip_comment(f);
	f >> maxval;

	if (maxval != 255)
		return false;

	// Skip single whitespace
	f.get();

	img.resize(w, h);
	f.read(img.data_ptr(), img.size());

	return f.good();
}

bool SavePGM_bin(const string& sFileName, const image<uint8_t>& img) {
	if (img.size() == 0)
		return false;

	ofstream f(sFileName.c_str(), ofstream::binary);
	if (!f)
		return false;

	f << "P5\n# Created by Costantino Grana\n" << img.width() << " " << img.height() << "\n255\n";
	f.write(img.data_ptr(), img.size());

	return f.good();
}

