#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <istream>
#include <iostream>
#include <vector>
#include "image.h"

using namespace std;
bool plain;

void syntax() {
	cout << "bayer_decode inputfile.PGM <output prefix> \n";
	EXIT_FAILURE;
}


bool writePGM(string filename, image<uint8_t>& img, bool plain) {
	ofstream os(filename, ios::binary);
	if (!os)return false;

	os << "P5\n" << img.width() << "\n" << img.height() << "\n" << "255\n";

		for (const auto& x : img) {
			os.put(x);
		}
}


bool writePPM(string filename, image<rgb>& img) {
	ofstream os(filename, ios::binary);
	if (!os)return false;

	os << "P6\n" << img.width() << "\n" << img.height() << "\n" << "255\n";
	
	for (size_t i = 0; i < img.height(); ++i) {
		for (size_t j = 0; j < img.width(); ++j) {
			os.put(img(j, i)[0]);
			os.put(img(j, i)[1]);
			os.put(img(j, i)[2]);
		}
	}

}



image<uint8_t> riduzione(image<uint16_t>& img) {
	image<uint8_t> origin(img.width(), img.height());
	for (size_t i = 0; i < img.height(); ++i) {
		for (size_t j = 0; j < img.width(); ++j) {
			origin(j,i) = img(j,i) >> 8;
		}
	}
	return origin;
}




image<uint16_t> readPGM(string filename) {
	ifstream ifs(filename, ios::binary);

	if (!ifs) {
		cout << "Error: can't open input file\n";
		exit(2);
	}
	string magic;
	getline(ifs, magic);
	if (magic == "P2")
		plain = true;
	else if (magic == "P5")
		plain = false;
	string comment;
	while (ifs.peek() == '#')
		getline(ifs, comment);

	size_t w, h, n;
	ifs >> w >> h >> n;
	cout << "Width: " << w << "\n";
	cout << "Height: " << h << "\n";

	char last = ifs .get();
	if (last != '\n') {
		exit(3);
	}
	if (plain) {
		image<uint16_t> out(w, h);
		for (auto& x : out) {
		ifs >> x;
		}
		return out;
		}
	else {
		image<uint16_t> out(w, h);
		for (auto& x : out) {
			x = ifs.get() << 8;
			x += ifs.get();
		}
		return out;
	}
}

uint8_t satura(double n) {
	double ret = round(n);
	ret = ret > 255 ? 255 : ret;
	ret = ret < 0 ? 0 : ret;
	return uint8_t(ret);
}



uint8_t ric_G(uint8_t x1, uint8_t x3, uint8_t x5, uint8_t x7, uint8_t x9, uint8_t g2, uint8_t g4, uint8_t g6, uint8_t g8) {
	double ret;

	// Calcolo H e V
	size_t H = abs(g4 - g6) + abs(2 * x5 - x3 - x7);
	size_t V = abs(g2 - g8) + abs(2 * x5 - x1 - x9);

	// Calcolo G
	if (H < V)
		ret = (g4 + g6) / 2.0 + (2 * x5 - x3 - x7) / 4.0;
	else if (H>V)
		ret = (g2 + g8) / 2.0 + (2 * x5 - x1 - x9) / 4.0;
	else
		ret = (g2 + g4 + g6 + g8) / 4.0 + (4 * x5 - x1 - x3 - x7 - x9) / 8.0;

	return satura(ret);
}


image<rgb> primapassata(image<uint8_t> img) {
	image<rgb> imgRGB(img.width(), img.height());
	for (size_t i = 0; i < img.height(); i=i+2) {
		for (size_t j = 0; j < img.width(); j=j+2) {
			imgRGB(j, i)[0] = img(j, i);
			imgRGB(j+1, i)[1] = img(j + 1, i);
			imgRGB(j, i + 1)[1] = img(j, i + 1);
			imgRGB(j+1, i + 1)[2] = img(j+1, i + 1);
		}
	}

	uint8_t deltaH = 0;
	uint8_t deltaV = 0;
	int caso;
	uint8_t x1, x3, x5, x7, x9, g2, g4, g6, g8;
	for (int y = 0; y < img.height(); ++y) {
		for (int x = 0; x < img.width(); ++x) {
			if (y % 2 && x % 2) { // riga dispari - colonna dispari B
				x1 = imgRGB(x, y - 2 < 0 ? y + 2 : y - 2)[2];
				x3 = imgRGB(x - 2 < 0 ? x + 2 : x - 2, y)[2];
				x5 = imgRGB(x, y)[2];
				x7 = imgRGB(x + 2 >= img.width() ? x - 2 : x + 2, y)[2];
				x9 = imgRGB(x, y + 2 >= img.height() ? y - 2 : y + 2)[2];
				g2 = imgRGB(x, y - 1 < 0 ? y + 1 : y - 1)[1];
				g4 = imgRGB(x - 1 < 0 ? x + 1 : x - 1, y)[1];
				g6 = imgRGB(x + 1 >= img.width() ? x - 1 : x + 1, y)[1];
				g8 = imgRGB(x, y + 1 >= img.height() ? y - 1 : y + 1)[1];
				imgRGB(x, y)[1] = ric_G(x1, x3, x5, x7, x9, g2, g4, g6, g8);
			}
			else if (x % 2 == 0 && y % 2 == 0) { // riga pari - colonna pari R
				x1 = imgRGB(x, y - 2 < 0 ? y + 2 : y - 2)[0];
				x3 = imgRGB(x - 2 < 0 ? x + 2 : x - 2, y)[0];
				x5 = imgRGB(x, y)[0];
				x7 = imgRGB(x + 2 >= img.width() ? x - 2 : x + 2, y)[0];
				x9 = imgRGB(x, y + 2 >= img.height() ? y - 2 : y + 2)[0];
				g2 = imgRGB(x, y - 1 < 0 ? y + 1 : y - 1)[1];
				g4 = imgRGB(x - 1 < 0 ? x + 1 : x - 1, y)[1];
				g6 = imgRGB(x + 1 >= img.width() ? x - 1 : x + 1, y)[1];
				g8 = imgRGB(x, y + 1 >= img.height() ? y - 1 : y + 1)[1];
				imgRGB(x, y)[1] = ric_G(x1, x3, x5, x7, x9, g2, g4, g6, g8);
			}
		}
	}
	return imgRGB;
}

uint8_t ric_G2(uint8_t x1, uint8_t x3, uint8_t x5, uint8_t x7, uint8_t x9, uint8_t g1, uint8_t g3, uint8_t g5, uint8_t g7, uint8_t g9) {
	double ret;

	// Calcolo H e V
	size_t N = abs(x1 - x9) + abs(2* g5 -g1 -g9);
	size_t P = abs(x3 - x7) + abs(2*g5 - g3 -g7);

	// Calcolo G
	if (N < P)
		ret = (x1 + x9) / 2.0 + (2* g5 - g1 -g9) / 4.0;
	else if (N>P)
		ret = (x3 +x7) / 2.0 + (2*g5 - g3 -g7) / 4.0;
	else
		ret = (x1 + x3 + x7+x9) / 4.0 + ( g5-g1+g5-g3+g5-g7+g5-g9) / 8.0;

	return satura(ret);
}






image<rgb> seconda_passata(image<rgb>& imgRGB) {
	for (int y = 0; y < imgRGB.height(); ++y) {
		for (int x = 0; x < imgRGB.width(); ++x) {

			//Interpolazione per i pixel verdi del rosso e del blu
			if (y % 2 == 0 && x%2!=0) {
				imgRGB(x, y)[0] = satura((imgRGB(x - 1 < 0 ? x + 1 : x - 1, y)[0] + imgRGB((x+1)>imgRGB.width() ? x - 1 : x + 1,y)[0])/2);
				imgRGB(x, y)[2] = satura((imgRGB(x, y - 1 < 0 ? y + 1 : y - 1)[2] + imgRGB(x, y + 1 >= imgRGB.height() ? y - 1 : y + 1)[2]) / 2);
			}
			else if (y % 2 != 0 && x % 2 == 0) {
				imgRGB(x, y)[0] = satura((imgRGB(x, y - 1 < 0 ? y + 1 : y - 1)[0] + imgRGB(x, y + 1 >= imgRGB.height() ? y - 1 : y + 1)[0]) / 2);
				imgRGB(x, y)[2] = satura((imgRGB(x - 1 < 0 ? x + 1 : x - 1, y)[2] + imgRGB(x + 1 >= imgRGB.width() ? x - 1 : x + 1, y)[2]) / 2);
			}
			
			//Interpolazione per i pixel Blu e Rossi dei restanti
			uint8_t x1, x3, x5, x7, x9, g1, g3, g5, g7, g9;

			if (y % 2 && x % 2) { // riga dispari - colonna dispari B
				x1 = imgRGB(x - 1 < 0 ? x + 1 : x - 1, y - 1 < 0 ? y + 1 : y - 1)[0];
				x3 = imgRGB(x+1>=imgRGB.width()? x-1 : x+1,y-1 < 0 ? y+1:y-1)[0];
				x5 = imgRGB(x,y)[0];
				x7 = imgRGB(x-1 < 0 ? x+1 : x-1,(y+1)>=imgRGB.height()? y-1:y+1)[0];
				x9 = imgRGB((x+1)>= imgRGB.width() ? x-1 : x+1,y+1 >= imgRGB.height() ? y-1 : y+1)[0];
				g1 = imgRGB(x + 1 >= imgRGB.width() ? x - 1 : x + 1, y - 1 < 0 ? y + 1 : y - 1)[1];
				g3 = imgRGB(x + 1 >= imgRGB.width() ? x - 1 : x + 1, y - 1 <0 ? y+1 : y-1)[1];
				g5 = imgRGB(x, y)[1];
				g7 = imgRGB(x - 1 < 0 ? x + 1 : x - 1, (y + 1) >= imgRGB.height() ? y - 1 : y + 1)[1];
				g9=  imgRGB((x + 1) >= imgRGB.width() ? x - 1 : x + 1, y + 1 >= imgRGB.height() ? y - 1 : y + 1)[1];
				imgRGB(x, y)[0] = ric_G2(x1, x3, x5, x7, x9, g1, g3, g5, g7, g9);
			}
			else if (x % 2 == 0 && y % 2 == 0) { // riga pari - colonna pari R
				x1 = imgRGB(x-1<0?x+1: x-1,y - 1<0 ? y+1 : y- 1)[2];
				x3 = imgRGB(x + 1 >= imgRGB.width() ? x - 1 : x + 1,(y-1)<0 ? y+1:y-1)[2];
				x5 = imgRGB(x, y)[2];
				x7 = imgRGB((x-1)<0 ? x+1 : x-1, y + 1 >= imgRGB.height() ? y - 1 : y + 1)[2];
				x9 = imgRGB(x + 1 >= imgRGB.width() ? x - 1 : x + 1, y + 1 >= imgRGB.height() ? y - 1 : y + 1)[2];
				g1 = imgRGB(x - 1<0 ? x + 1 : x - 1, y - 1<0 ? y + 1 : y - 1 <0 ? y+1 : y-1)[1];
				g3 = imgRGB(x + 1 >= imgRGB.width() ? x - 1 : x + 1, y - 1 < 0 ? y + 1 : y - 1)[1];
				g5 = imgRGB(x, y)[1];
				g7 = imgRGB(x - 1 < 0 ? x + 1 : x - 1, y + 1 >= imgRGB.height() ? y - 1 : y + 1)[1];
				g9 = imgRGB(x + 1 >= imgRGB.width() ? x - 1 : x + 1, y + 1 >= imgRGB.height() ? y - 1 : y + 1)[1];
				imgRGB(x, y)[2] = ric_G2(x1, x3, x5, x7, x9, g1, g3, g5, g7, g9);
			}
		
		}
	}
	return imgRGB;


}






int main(int argc, char **argv) {
	if (argc != 4) {
		syntax();
	}
	auto img16=readPGM(argv[2]);
	auto img8 =riduzione(img16);
	writePGM(argv[3],img8,plain);
	auto imgG = primapassata(img8);
	writePPM("Green.ppm", imgG);
	auto imgF = seconda_passata(imgG);
	writePPM("Final.pgm", imgF);
}